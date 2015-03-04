/*
 * Copyright (c) 2003, 2011, Oracle and/or its affiliates. All rights reserved.
 */

/*
 * Functions used for executing the I²C protocol.
 *
 * Caller should provide a vector of function calls which can
 * be called to perform the following low-level functions:
 *
 *	set_scl()	set SCL line 0 or 1
 *	set_sda()	set SDA line 0 or 1
 *	get_scl()	return SCL line
 *	get_sda()	return SDA line
 *	ddc_clk()	wait for 'n' clock periods.  'n' is usually 1
 *			and the period is typically 2.5-40 µsec, but this
 *			value is up to the implementation.  One bus cycle
 *			is four clock periods, so the minimum allowable
 *			period is 2.5 µsec (100 kHz bus clock).
 *	clock_interval	integer field specifying the clock period in µsec.
 *	clock_stretch	integer field specifying maximum clock stretch
 *			time in µsec.  Typically 10000.
 *
 * All functions accept two arbitrary arguments which may be used
 * to pass one pointer and one integer value.  These might typically
 * be used to pass a device softc structure and a unit number.
 *
 * All functions which return an int will return 0 on success, error code
 * on failure.
 *
 * Caller should use mutex locking to ensure that two threads do not
 * attempt to simultaneously access the device.  The regular device
 * mutex should usually not be used for this purpose if avoidable, as this
 * protocol can take a considerable amount of time to execute.
 */

#ifndef	_EFB_I2C_H
#define	_EFB_I2C_H

#include <sys/types.h>


/* Error codes: */

#define	EFB_I2C_OK		0 /* No error */
#define	EFB_I2C_TIMEOUT		1
#define	EFB_I2C_NOACK		2 /* Slave fails to acknowledge a write */
#define	EFB_I2C_STRETCH		3 /* Slave fails to release clock stretch */
#define	EFB_I2C_READ_NOACK	4 /* Slave fails to release NOACK after read */


typedef	struct {
	void (*set_scl)(void *priv, int port, int data);
	void (*set_sda)(void *priv, int port, int data);
	int  (*get_scl)(void *priv, int port);
	int  (*get_sda)(void *priv, int port);
	void (*ddc_clk)(void *priv, int port, int n);
	int  clock_interval;
	int  clock_stretch;
} efb_i2c_functions_t;


extern void	efb_i2c_write_reg(void *, const efb_i2c_functions_t *,
		    uint_t,  uint_t, uint_t, uchar_t);
extern int	efb_i2c_write_byte(void *, int, const efb_i2c_functions_t *,
		    int data);
extern int	efb_i2c_check_write_ack(void *, int,
		    const efb_i2c_functions_t *);
extern int	efb_i2c_read_byte(void *, int, const efb_i2c_functions_t *,
		    uint8_t *);
extern int	efb_i2c_read_byte_noack(void *, int,
		    const efb_i2c_functions_t *, uint8_t *);
extern int	efb_i2c_stop(void *, int, const efb_i2c_functions_t *);
extern int	efb_i2c_start(void *, int, const efb_i2c_functions_t *);
extern int	efb_i2c_clk_stretch(void *, int, const efb_i2c_functions_t *);

#endif	/* _EFB_I2C_H */
