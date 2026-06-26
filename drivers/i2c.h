#ifndef _I2C_MODULE_
#define _I2C_MODULE_

#include <stdint.h>

/* I2C1 clock offset */
#define CM_PER_I2C1_CLKCTRL 0x48

/* Pinmux offsets (CONTROL_MODULE) */
#define conf_spi0_cs0 0x95C // P9_17 -> I2C1_SCL
#define conf_spi0_d1  0x958 // P9_18 -> I2C1_SDA

/* I2C1 register offsets */
#define I2C_CON          0xA4
#define I2C_PSC          0xB0
#define I2C_SCLL         0xB4
#define I2C_SCLH         0xB8
#define I2C_SA           0xAC
#define I2C_DATA         0x9C
#define I2C_CNT          0x98
#define I2C_IRQSTATUS_RAW 0x24

/* Max wait cycles to avoid CPU stall */
#define I2C_TIMEOUT 100000UL

void I2C_Init(void);
int  I2C_Write(uint8_t slave_addr, uint8_t *buf, uint16_t len);
int  I2C_Read (uint8_t slave_addr, uint8_t *buf, uint16_t len);

#endif