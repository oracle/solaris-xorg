/*
 * Copyright (c) 2006, 2011, Oracle and/or its affiliates. All rights reserved.
 */

#include <sys/types.h>
#include <sys/ddi.h>

#include "efb.h"
#include "efb_i2c.h"


static	int	efb_write_bit(void *, int, const efb_i2c_functions_t *, int);
static	int	efb_read_bit(void *, int, const efb_i2c_functions_t *, int *);

int
efb_i2c_stop(void *efb_priv, int port, const efb_i2c_functions_t *fcns)
{
	int	status;

	/*
	 * A word of explanation.  During data transfers in the I2C protocol,
	 * data never changes while the clock is high.  The "start" and 'stop"
	 * conditions are marked by doing just that.  The "stop" condition
	 * is signaled by raising the data line while the clock is high.
	 *
	 * This condition should be as simple to implement as simply calling
	 * set_sda(1).  The problem here is that we can't count on the
	 * initial states of either clock or data.
	 *
	 * To make this work, we need to first make sure the data line is low
	 * so we can raise it.	However, in order to make sure that lowering
	 * the data line doesn't generate a start condition, we must also make
	 * sure the clock is low too.  Thus, the sequence is:
	 *	lower clock
	 *	lower data
	 *	raise clock
	 *	raise data	<= this is the actual "stop"
	 *
	 * TODO: would it make sense to read the clock and data lines first
	 * so we can skip these steps?
	 */
	fcns->set_scl(efb_priv, port, 0);	/* make sure clock is low */
	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_sda(efb_priv, port, 0);	/* make sure data is low */
	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_scl(efb_priv, port, 1);	/* release clock */

	status = efb_i2c_clk_stretch(efb_priv, port, fcns);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_sda(efb_priv, port, 1);	/* generate the stop */
	fcns->ddc_clk(efb_priv, port, 1);

	return (EFB_I2C_OK);
}


int
efb_i2c_start(void *efb_priv, int port, const efb_i2c_functions_t *fcns)
{
	int	status;

	/*
	 * See above.  A start condition is caused by data going low
	 * while clock is high.
	 */
	fcns->set_scl(efb_priv, port, 0);	/* make sure clock is low */
	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_sda(efb_priv, port, 1);	/* make sure data is high */
	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_scl(efb_priv, port, 1);	/* release clock */

	status = efb_i2c_clk_stretch(efb_priv, port, fcns);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_sda(efb_priv, port, 0);	/* generate the start */
	fcns->ddc_clk(efb_priv, port, 1);

	return (EFB_I2C_OK);
}


static	int
efb_read_byte_data(void *efb_priv, int port,
    const efb_i2c_functions_t *fcns, uint8_t *rval)
{
	int	b0, b1, b2, b3, b4, b5, b6, b7;
	int	status;

	/* Read a byte from client without sending either ack or nack */

	if ((status = efb_read_bit(efb_priv, port, fcns, &b7)) != EFB_I2C_OK ||
	    (status = efb_read_bit(efb_priv, port, fcns, &b6)) != EFB_I2C_OK ||
	    (status = efb_read_bit(efb_priv, port, fcns, &b5)) != EFB_I2C_OK ||
	    (status = efb_read_bit(efb_priv, port, fcns, &b4)) != EFB_I2C_OK ||
	    (status = efb_read_bit(efb_priv, port, fcns, &b3)) != EFB_I2C_OK ||
	    (status = efb_read_bit(efb_priv, port, fcns, &b2)) != EFB_I2C_OK ||
	    (status = efb_read_bit(efb_priv, port, fcns, &b1)) != EFB_I2C_OK ||
	    (status = efb_read_bit(efb_priv, port, fcns, &b0)) != EFB_I2C_OK) {
		return (status);
	}

	*rval =
	    (b7 << 7) |
	    (b6 << 6) |
	    (b5 << 5) |
	    (b4 << 4) |
	    (b3 << 3) |
	    (b2 << 2) |
	    (b1 << 1) |
	    (b0 << 0);

	return (EFB_I2C_OK);
}


int
efb_i2c_read_byte(void *efb_priv, int port,
    const efb_i2c_functions_t *fcns, uint8_t *rval)
{
	int	status;

	status = efb_read_byte_data(efb_priv, port, fcns, rval);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	return (efb_write_bit(efb_priv, port, fcns, 0));	/* ack */
}


int
efb_i2c_read_byte_noack(void *efb_priv, int port,
    const efb_i2c_functions_t *fcns, uint8_t *rval)
{
	int	status;

	status = efb_read_byte_data(efb_priv, port, fcns, rval);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	fcns->set_scl(efb_priv, port, 0);
	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_sda(efb_priv, port, 1);		/* nack */
	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_scl(efb_priv, port, 1);

	status = efb_i2c_clk_stretch(efb_priv, port, fcns);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	fcns->ddc_clk(efb_priv, port, 1);

	status = fcns->get_sda(efb_priv, port);	/* should float high */
	fcns->ddc_clk(efb_priv, port, 1);

	return (status == 1 ? EFB_I2C_OK : EFB_I2C_READ_NOACK);
}


static	int
efb_read_bit(void *efb_priv, int port,
    const efb_i2c_functions_t *fcns, int *rval)
{
	int	status;

	fcns->set_scl(efb_priv, port, 0);
	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_sda(efb_priv, port, 1);	/* release data line */
	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_scl(efb_priv, port, 1);

	status = efb_i2c_clk_stretch(efb_priv, port, fcns);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	fcns->ddc_clk(efb_priv, port, 1);

	*rval = fcns->get_sda(efb_priv, port);
	fcns->ddc_clk(efb_priv, port, 1);

	return (EFB_I2C_OK);
}


int
efb_i2c_write_byte(void *efb_priv, int port,
    const efb_i2c_functions_t *fcns, int data)
{
	int	status;
	int	i;
	int	bit;

	/* Read the bits high-to-low, write them out */
	for (i = 0; i < 8; ++i) {
		bit = (data >> 7) & 1;
		data <<= 1;

		status = efb_write_bit(efb_priv, port, fcns, bit);
		if (status != EFB_I2C_OK) {
			return (status);
		}
	}

	return (efb_i2c_check_write_ack(efb_priv, port, fcns));
}


static	int
efb_write_bit(void *efb_priv, int port,
    const efb_i2c_functions_t *fcns, int data)
{
	int	status;

	/*
	 * One clock cycle is divided into four units of clock_interval.
	 * The timing diagram looks like:
	 *
	 *	SDA _----____----___		(_=old data, -=new)
	 *	SCL __--__--__--__--
	 *	    ^^^^
	 *	    one cycle
	 *
	 * (This diagram illustrates four transfer cycles, this
	 * function only executes one.)
	 */

	fcns->set_scl(efb_priv, port, 0);
	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_sda(efb_priv, port, data);
	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_scl(efb_priv, port, 1);

	status = efb_i2c_clk_stretch(efb_priv, port, fcns);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	fcns->ddc_clk(efb_priv, port, 1);

	fcns->ddc_clk(efb_priv, port, 1);

	return (EFB_I2C_OK);
}


int
efb_i2c_check_write_ack(void *efb_priv, int port,
    const efb_i2c_functions_t *fcns)
{
	int	status;

	/*
	 * We've transfered eight bits to the slave.  Cycle the clock
	 * one more time and let the slave acknowledge by pulling the
	 * data line to zero.
	 */

	fcns->set_scl(efb_priv, port, 0);		/* clock low */
	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_sda(efb_priv, port, 1);		/* release data line */
	fcns->ddc_clk(efb_priv, port, 1);

	fcns->set_scl(efb_priv, port, 1);

	status = efb_i2c_clk_stretch(efb_priv, port, fcns);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	fcns->ddc_clk(efb_priv, port, 1);

	status = fcns->get_sda(efb_priv, port);
	fcns->ddc_clk(efb_priv, port, 1);

	return (status == 0 ? EFB_I2C_OK : EFB_I2C_NOACK);
}


void
efb_i2c_write_reg(void *efb_priv, const efb_i2c_functions_t *fcns,
    uint_t port, uint_t i2c_addr, uint_t reg_addr, uchar_t data)
{
	(void) efb_i2c_start(efb_priv, port, fcns);

	(void) efb_i2c_write_byte(efb_priv, port, fcns, i2c_addr);
	(void) efb_i2c_write_byte(efb_priv, port, fcns, reg_addr);
	(void) efb_i2c_write_byte(efb_priv, port, fcns, data);

	(void) efb_i2c_stop(efb_priv, port, fcns);
}


int
efb_i2c_clk_stretch(void *efb_priv, int port, const efb_i2c_functions_t *fcns)
{
	int	status = 0;
	int	count = fcns->clock_stretch;	/* 10,000 µsec */

	/*
	 * Slave devices can put a transfer on hold by holding the
	 * clock low.  This function waits up to 10 ms for the
	 * clock to go high before returning error.
	 */

	while (--count >= 0) {
		drv_usecwait(1);
		if (fcns->get_scl(efb_priv, port)) {
			break;
		}
	}

	status = (count <= 0) ? EFB_I2C_STRETCH : EFB_I2C_OK;

	return (status);
}
