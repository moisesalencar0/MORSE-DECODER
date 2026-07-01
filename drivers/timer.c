/**
 * @file timer.c
 * @brief Timer-based delay implementation using DMTimer7 (AM335x).
 */

#include "timer.h"

#include "soc_AM335x.h"
#include "hw_types.h"
#include "hw_regs.h"
#include "intc.h"


/** @brief Maximum value of 32 bit. */
#define TIMER_OVERFLOW 0xFFFFFFFF

#define SETTING  ((1<<18) | 2)

void DMTimer_Init(void) {
    HWREG(SOC_CM_PER_REGS + CM_PER_TIMER7_CLKCTRL) |= SETTING;
    while((HWREG(SOC_CM_PER_REGS + CM_PER_TIMER7_CLKCTRL) & SETTING) == 0){};

    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TSICR) |= DMTIMER_TSICR_POSTED;
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCLR)  &= ~DMTIMER_TCLR_ST;

    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCRR)   = 0x00000000;
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TLDR)   = 0x00000000;
}

static uint32_t DMTimer_WritePostedStatusGet(uint32_t baseAdd){
    return HWREG(baseAdd + DMTIMER_TWPS);
}

/**
 * @brief Waits for a pending write to complete in posted mode.
 *
 * If posted mode is active, polls @c DMTIMER_TWPS until
 * the specified register write is no longer pending.
 *
 * @param reg    Write-post bit to check (e.g. @c DMTIMER_WRITE_POST_TCRR).
 * @param baseAdd Timer base address.
 */
#define DMTimer_WaitForWrite(reg, baseAdd)                           \
    do {                                                            \
        if (HWREG(baseAdd + DMTIMER_TSICR) & DMTIMER_TSICR_POSTED) \
            while ((reg & DMTimer_WritePostedStatusGet(baseAdd)));   \
    } while (0)

/**
 * @brief Sets the timer counter register.
 *
 * @param baseAdd Timer base address.
 * @param counter Value to load into @c DMTIMER_TCRR.
 */
void DMTimer_CounterSet(uint32_t baseAdd, uint32_t counter) {
    DMTimer_WaitForWrite(DMTIMER_WRITE_POST_TCRR, baseAdd);
    HWREG(baseAdd + DMTIMER_TCRR) = counter;
}

/**
 * @brief Returns the current timer counter value.
 *
 * @param baseAdd Timer base address.
 * @return Current value of @c DMTIMER_TCRR.
 */
uint32_t DMTimer_CounterGet(uint32_t baseAdd) {
    DMTimer_WaitForWrite(DMTIMER_WRITE_POST_TCRR, baseAdd);
    return HWREG(baseAdd + DMTIMER_TCRR);
}

/**
 * @brief Starts the timer by setting the ST bit in TCLR.
 *
 * @param baseAdd Timer base address.
 */
void DMTimer_Enable(uint32_t baseAdd) {
    DMTimer_WaitForWrite(DMTIMER_WRITE_POST_TCLR, baseAdd);
    HWREG(baseAdd + DMTIMER_TCLR) |= DMTIMER_TCLR_ST;
}

/**
 * @brief Stops the timer by clearing the ST bit in TCLR.
 *
 * @param baseAdd Timer base address.
 */
void DMTimer_Disable(uint32_t baseAdd) {
    DMTimer_WaitForWrite(DMTIMER_WRITE_POST_TCLR, baseAdd);
    HWREG(baseAdd + DMTIMER_TCLR) &= ~DMTIMER_TCLR_ST;
}

volatile uint32_t timer_overflow = false;

void DMTimer_ISR(void) {
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_IRQSTATUS) = 0x2;
    timer_overflow = true;
}

/**
 * @brief Blocking delay using DMTimer7.
 *
 * Loads the counter with a precomputed overflow value,
 * enables the overflow interrupt and busy-waits until
 * the ISR signals completion through @c timer_overflow].
 *
 * @param us Delay duration in seconds.
 */
void DMTimer_Delay(uint32_t us) {
    uint32_t countVal = TIMER_OVERFLOW - (us * 24000); // 24Mhz

    DMTimer_WaitForWrite(DMTIMER_WRITE_POST_TCRR, SOC_DMTIMER_7_REGS);
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCRR) = countVal;

    timer_overflow = false;

    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_IRQENABLE_SET) |= 0x2;
    DMTimer_Enable(SOC_DMTIMER_7_REGS);

    while (timer_overflow == false) {}

    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_IRQENABLE_CLR) |= 0x2;
    DMTimer_Disable(SOC_DMTIMER_7_REGS);
}

void DMTimer_IntConfig(void) {
    HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR2) |= (1 << 31);
}