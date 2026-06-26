/**
 * @file i2c.c
 * @brief I2C1 bus driver implementation for AM335x (BeagleBone Black).
 *
 * @details Provides functions for hardware initialization, multi-byte transmission (write),
 * and multi-byte reception (read) using a master polling mechanism.
 */

#include "soc_AM335x.h"
#include "hw_types.h"
#include "i2c.h"

/**
 * @brief Initializes the I2C1 peripheral module.
 *
 * @details This function performs the complete hardware initialization sequence:
 * - Enables the operational clock for the I2C1 module and polls until active.
 * - Configures Pinmux for SCL and SDA lines (Rx active, internal pull-up enabled, Mode 2).
 * - Temporarily disables the module to safely configure clock timings.
 * - Sets a 100 kbps speed using a prescaler (48MHz system clock to 12MHz internal) and 
 * adjusts low/high SCL tics subtracting physical hardware delays.
 * - Re-enables the module and configures the BeagleBone as the Bus Master.
 */
void i2c1_init(void){
    HWREG(SOC_CM_PER_REGS + CM_PER_I2C1_CLKCTRL) = 0x2;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_I2C1_CLKCTRL) & (0x3 << 16)) != 0x0);

    HWREG(SOC_CONTROL_REGS + conf_spi0_cs0) = (1 << 5) | (1 << 4) | (0 << 3) | 0x2; 
    HWREG(SOC_CONTROL_REGS + conf_spi0_d1) = (1 << 5) | (1 << 4) | (0 << 3) | 0x2;  

    HWREG(SOC_I2C_1_REGS + I2C_CON) &= ~(1 << 15);

    HWREG(SOC_I2C_1_REGS + I2C_PSC) = (48 / 12) - 1; 
    HWREG(SOC_I2C_1_REGS + I2C_SCLL) = 60 - 7;      
    HWREG(SOC_I2C_1_REGS + I2C_SCLH) = 60 - 5;      

    HWREG(SOC_I2C_1_REGS + I2C_CON) |= (1 << 15) | (1 << 10);
}

/**
 * @brief Transmits data bytes to a specific I2C slave device.
 *
 * @details This function handles the multi-byte transmission protocol:
 * - Loads the target peripheral address into the I2C_SA register.
 * - Configures the exact payload size in the I2C_CNT register.
 * - Issues a Start condition with Master Transmitter and Stop configuration enabled.
 * - Iterates through the buffer, polling the XRDY flag with a software timeout guard.
 * - Loads each byte into the hardware register and clears the XRDY flag.
 * - Waits for transaction finalization via the ARDY flag.
 * - Checks for a No Acknowledge (NACK) condition from the slave, handles error flag 
 * clearing, and returns the appropriate status.
 *
 * @param slave_addr 7-bit I2C address of the target slave peripheral.
 * @param buf        Pointer to the data buffer containing the bytes to send.
 * @param len        Total number of bytes to transmit.
 *
 * @return int 0 on successful transmission; -1 on timeout or if a NACK is detected.
 */
int i2c1_write(uint8_t slave_addr, uint8_t *buf, uint16_t len){
    uint32_t timeout;

    HWREG(SOC_I2C_1_REGS + I2C_SA) = slave_addr;
    HWREG(SOC_I2C_1_REGS + I2C_CNT) = len;

    HWREG(SOC_I2C_1_REGS + I2C_CON) = (1 << 15) | (1 << 10) | (1 << 9) | (1 << 1) | (1 << 0);

    for(uint16_t i = 0; i < len; i++){
        timeout = I2C_TIMEOUT;

        while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 4)) == 0) && --timeout);
        if (timeout == 0) return -1; 

        HWREG(SOC_I2C_1_REGS + I2C_DATA) = buf[i];
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 4);
    }

    timeout = I2C_TIMEOUT;
    while(((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 2)) == 0 ) && --timeout);
    if (timeout == 0) return -1;

    if (HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 1)) { 
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 1) | (1 << 2);
        return -1;
    }

    HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 2);

    return 0;
}

/**
 * @brief Receives data bytes from a specific I2C slave device.
 *
 * @details This function handles the multi-byte reception protocol:
 * - Loads the target peripheral address into the I2C_SA register.
 * - Configures the expected payload size in the I2C_CNT register.
 * - Issues a Start condition with Master Receiver and Stop configuration enabled.
 * - Iterates through the expected length, polling the RRDY flag with a software timeout guard.
 * - Extracts each arriving byte from the hardware register, stores it locally, and clears RRDY.
 * - Waits for transaction finalization via the ARDY flag.
 * - Checks for a No Acknowledge (NACK) condition from the slave, handles error flag 
 * clearing, and returns the appropriate status.
 *
 * @param slave_addr 7-bit I2C address of the target slave peripheral.
 * @param buf        Pointer to the local data buffer where received bytes will be stored.
 * @param len        Total number of bytes to read.
 *
 * @return int 0 on successful reception; -1 on timeout or if a NACK is detected.
 */
int i2c1_read(uint8_t slave_addr, uint8_t *buf, uint16_t len){
    uint32_t timeout;

    HWREG(SOC_I2C_1_REGS + I2C_SA)  = slave_addr;
    HWREG(SOC_I2C_1_REGS + I2C_CNT) = len;
    
    HWREG(SOC_I2C_1_REGS + I2C_CON) = (1 << 15) | (1 << 10) | (1 << 1) | (1 << 0);

    for (uint16_t i = 0; i < len; i++) {
        timeout = I2C_TIMEOUT;
        
        while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 3)) == 0) && --timeout); 
        if (timeout == 0) return -1;

        buf[i] = (uint8_t)HWREG(SOC_I2C_1_REGS + I2C_DATA);
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 3); 
    }

    timeout = I2C_TIMEOUT;
    while (((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 2)) == 0) && --timeout);
    if (timeout == 0) return -1;

    if (HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & (1 << 1)) { 
        HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 1) | (1 << 2);
        return -1;
    }

    HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) = (1 << 2);
    
    return 0;
}




