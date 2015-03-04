/*
 * Copyright (c) 2006, 2011, Oracle and/or its affiliates. All rights reserved.
 */

#include "efb.h"
#include "efb_edid.h"


static	int	efb_initial_steps(efb_private_t *, int);
static	int	efb_read_edid_data(efb_private_t *, int, uint8_t *, uint_t *);
static	int	efb_verify_checksum(uint8_t *);
static	void 	efb_set_scl(void *, int, int);
static	void 	efb_set_sda(void *, int, int);
static	int 	efb_get_scl(void *, int);
static	int 	efb_get_sda(void *, int);
static	void 	efb_ddc_clk(void *, int, int);

#define	CLOCK_INTERVAL	((clock_t)40)	/* Per the EDID1 spec */

static const efb_i2c_functions_t efb_i2c_fcns = {
	efb_set_scl,
	efb_set_sda,
	efb_get_scl,
	efb_get_sda,
	efb_ddc_clk,
	CLOCK_INTERVAL,
	10000
};

int
efb_read_edid(efb_private_t *efb_priv, int port, uint8_t *data, uint_t *len)
{
	int	status;
	int	count;

	/* Write address to monitor */

	for (count = 3; --count >= 0; ) {
		if ((status = efb_initial_steps(efb_priv, port)) == 0) {
			break;
		}
	}

	if (status != EFB_I2C_OK) {
		return (status);
	}

	/* read data from monitor */
	status = efb_i2c_start(efb_priv, port, &efb_i2c_fcns);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	status = efb_i2c_write_byte(efb_priv, port, &efb_i2c_fcns, 0xA1);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	status = efb_read_edid_data(efb_priv, port, data, len);

	return (status);
}


static	int
efb_initial_steps(efb_private_t *efb_priv, int port)
{
	int	status;
	int	i;

	/*
	 * Why nine stops?  Presumably because if we've interrupted a
	 * data transfer, this will be guaranteed to clear it.
	 */
	for (i = 0; i < 9; ++i) {
		(void) efb_i2c_stop(efb_priv, port, &efb_i2c_fcns);
	}

	/* Ok, now try to start a transfer to the monitor */
	status = efb_i2c_start(efb_priv, port, &efb_i2c_fcns);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	status = efb_i2c_write_byte(efb_priv, port, &efb_i2c_fcns, 0xA0);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	status = efb_i2c_write_byte(efb_priv, port, &efb_i2c_fcns, 0);

	return (status);
}


static	int
efb_read_edid_data(efb_private_t *efb_priv, int port,
    uint8_t *data, uint_t *len)
{
	int	status;
	uint8_t	*initial = data;
	int	i;

	for (i = 0; i < *len - 1; ++i) {
		status = efb_i2c_read_byte(efb_priv, port, &efb_i2c_fcns, data);
		if (status != EFB_I2C_OK) {
			return (status);
		}
		++data;
	}

	/* Final byte terminates transfer, even if monitor had more */
	status = efb_i2c_read_byte_noack(efb_priv, port, &efb_i2c_fcns, data);
	if (status != EFB_I2C_OK) {
		return (status);
	}

	return (efb_verify_checksum(initial));
}


static	int
efb_verify_checksum(uint8_t *data)
{
	int	i;
	uint32_t checksum = 0;

	for (i = 0; i < 128; ++i) {
		checksum += *data++;
	}

	return (checksum & 0xff);
}

/*
 * Implement basic functions to toggle SDA and SCL lines when querying EDID
 * from the VGA/DVI monitors.
 *
 * DDC registers (RRG-215R6-01-01oem.pdf Section 2.21 on Page 156)
 */
static	void
efb_set_scl(void *s, int stream, int value)
{
	uint_t		 reg, ddc;
	efb_private_t	 *efb_priv = s;
	volatile caddr_t registers = efb_priv->registers;

	switch (stream) {

	case 0:
		reg = GPIO_DDC1;
		break;

	case 1:
		reg = GPIO_DDC2;
		break;

	case GPIO_DDC1:
	case GPIO_DDC2:
	case GPIO_DDC3:
		reg = stream;
		break;

	default:
		cmn_err(CE_NOTE, "efb i2c: unrecognized port: 0x%x\n", stream);
		return;
	}

	ddc = regr(reg);

	if (value == 0) {
		ddc = ddc & ~DDC_CLK_OUTPUT | DDC_CLK_OUT_EN;
	} else {
		ddc = ddc & ~DDC_CLK_OUT_EN | DDC_CLK_OUTPUT;
	}

	regw(reg, ddc);
}


static	void
efb_set_sda(void *s, int stream, int value)
{
	uint_t		 reg, ddc;
	efb_private_t	 *efb_priv = s;
	volatile caddr_t registers = efb_priv->registers;

	switch (stream) {

	case 0:
		reg = GPIO_DDC1;
		break;

	case 1:
		reg = GPIO_DDC2;
		break;

	case GPIO_DDC1:
	case GPIO_DDC2:
	case GPIO_DDC3:
		reg = stream;
		break;

	default:
		cmn_err(CE_NOTE, "efb i2c: unrecognized port: 0x%x\n", stream);
		return;
	}

	ddc = regr(reg);

	if (value == 0) {
		ddc = ddc & ~DDC_DATA_OUTPUT | DDC_DATA_OUT_EN;
	} else {
		ddc = ddc & ~DDC_DATA_OUT_EN | DDC_DATA_OUTPUT;
	}

	regw(reg, ddc);
}


static	int
efb_get_scl(void *s, int stream)
{
	uint_t		 reg, ddc;
	int		 rval;
	efb_private_t	 *efb_priv = s;
	volatile caddr_t registers = efb_priv->registers;

	switch (stream) {

	case 0:
		reg = GPIO_DDC1;
		break;

	case 1:
		reg = GPIO_DDC2;
		break;

	case GPIO_DDC1:
	case GPIO_DDC2:
	case GPIO_DDC3:
		reg = stream;
		break;

	default:
		cmn_err(CE_NOTE, "efb i2c: unrecognized port: 0x%x\n", stream);
		return (0);
	}

	ddc = regr(reg);

	rval = ((ddc >> DDC_CLK_INPUT_SHIFT) & 1);

	return (rval);
}


static	int
efb_get_sda(void *s, int stream)
{
	uint_t		 reg, ddc;
	int		 rval;
	efb_private_t	 *efb_priv = s;
	volatile caddr_t registers = efb_priv->registers;

	switch (stream) {

	case 0:
		reg = GPIO_DDC1;
		break;

	case 1:
		reg = GPIO_DDC2;
		break;

	case GPIO_DDC1:
	case GPIO_DDC2:
	case GPIO_DDC3:
		reg = stream;
		break;

	default:
		cmn_err(CE_NOTE, "efb i2c: unrecognized port: 0x%x\n", stream);
		return (0);
	}

	ddc = regr(reg);

	rval = ((ddc >> DDC_DATA_INPUT_SHIFT) & 1);

	return (rval);
}

static	void
efb_ddc_clk(void *efb_priv, int stream, int n)
{
	_NOTE(ARGUNUSED(efb_priv, stream))

	drv_usecwait(n * CLOCK_INTERVAL);
}
