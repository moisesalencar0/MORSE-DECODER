/**
 * @file i2c.c
 * @brief I2C1 bus driver implementation for AM335x (BeagleBone Black).
 */

#include "soc_AM335x.h"
#include "hw_types.h"
#include "i2c.h"
#include "uart_io.h"

void I2C_Init(void) {
    HWREG(SOC_CM_PER_REGS + CM_PER_I2C1_CLKCTRL) = 0x2;
    printString("[I2C] Waiting for clock...\r\n", 28);

    while((HWREG(SOC_CM_PER_REGS + CM_PER_I2C1_CLKCTRL) & (0x3 << 16)) != 0x0);
    printString("[I2C] Clock active\r\n", 20);

    HWREG(SOC_CONTROL_REGS + conf_spi0_cs0) = (1 << 5) | (1 << 4) | (0 << 3) | 0x2;
    HWREG(SOC_CONTROL_REGS + conf_spi0_d1)  = (1 << 5) | (1 << 4) | (0 << 3) | 0x2;
    printString("[I2C] Pinmux configured\r\n", 25);

    HWREG(SOC_I2C_1_REGS + I2C_CON) &= ~(1 << 15);
    printString("[I2C] Module disabled for config\r\n", 34);

    HWREG(SOC_I2C_1_REGS + I2C_PSC)  = (48 / 12) - 1;
    HWREG(SOC_I2C_1_REGS + I2C_SCLL) = 60 - 7;
    HWREG(SOC_I2C_1_REGS + I2C_SCLH) = 60 - 5;
    printString("[I2C] Clock timing configured (100kbps)\r\n", 41);

    HWREG(SOC_I2C_1_REGS + I2C_CON) |= (1 << 15) | (1 << 10);
    printString("[I2C] Module enabled, master mode set\r\n", 39);
}

int I2C_Write(uint8_t slave_addr, uint8_t *buf, uint16_t len) {
    uint32_t timeout;

    printString("[I2C] Write start\r\n", 19);

    HWREG(SOC_I2C_1_REGS + I2C_SA)  = slave_addr;
    HWREG(SOC_I2C_1_REGS + I2C_CNT) = len;
    HWREG(SOC_I2C_1_REGS + I2C_CON) = (1 << 15) | (1 << 10) | (1 << 9) | (1 << 1) | (1 << 0);
    printString("[I2C] START condition issued\r\n", 29);

    for (uint16_t i = 0; i < len; i++) {
        timeout = I2C_TIMEOUT;
        while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 4)) == 0) && --timeout);

        if (timeout == 0) {
            printString("[I2C] Write timeout on XRDY\r\n", 29);
            return -1;
        }

        HWREG(SOC_I2C_1_REGS + I2C_DATA)          = buf[i];
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 4);
    }
    printString("[I2C] All bytes sent\r\n", 22);

    timeout = I2C_TIMEOUT;
    while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 2)) == 0) && --timeout);

    if (timeout == 0) {
        printString("[I2C] Write timeout on ARDY\r\n", 29);
        return -1;
    }

    if (HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 1)) {
        printString("[I2C] NACK received\r\n", 21);
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 1) | (1 << 2);
        return -1;
    }

    HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 2);
    printString("[I2C] Write OK\r\n", 16);
    return 0;
}

int I2C_Read(uint8_t slave_addr, uint8_t *buf, uint16_t len) {
    uint32_t timeout;

    printString("[I2C] Read start\r\n", 18);

    HWREG(SOC_I2C_1_REGS + I2C_SA)  = slave_addr;
    HWREG(SOC_I2C_1_REGS + I2C_CNT) = len;
    HWREG(SOC_I2C_1_REGS + I2C_CON) = (1 << 15) | (1 << 10) | (1 << 1) | (1 << 0);
    printString("[I2C] START condition issued\r\n", 29);

    for (uint16_t i = 0; i < len; i++) {
        timeout = I2C_TIMEOUT;
        while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 3)) == 0) && --timeout);

        if (timeout == 0) {
            printString("[I2C] Read timeout on RRDY\r\n", 28);
            return -1;
        }

        buf[i] = (uint8_t)HWREG(SOC_I2C_1_REGS + I2C_DATA);
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 3);
    }
    printString("[I2C] All bytes received\r\n", 25);

    timeout = I2C_TIMEOUT;
    while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 2)) == 0) && --timeout);

    if (timeout == 0) {
        printString("[I2C] Read timeout on ARDY\r\n", 28);
        return -1;
    }

    if (HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 1)) {
        printString("[I2C] NACK received\r\n", 21);
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 1) | (1 << 2);
        return -1;
    }

    HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 2);
    printString("[I2C] Read OK\r\n", 15);
    return 0;
}