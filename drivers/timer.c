/**
 * @file timer.c
 * @brief Timer-based delay implementation using DMTimer7 (AM335x).
 */

#include "timer.h"

#include "soc_AM335x.h"
#include "hw_types.h"
#include "hw_regs.h"
//#include "intc.h"


/** @brief Maximum value of 32 bit. */
#define TIMER_OVERFLOW 0xFFFFFFFF

/** @brief Timer ticks per microsecond at 24MHz. */
#define TIMER_1US_COUNT 24

/**
 * @brief Configures DMTimer7 for delay generation.
 *
 * Enables posted mode, stops the timer and resets
 * counter and load registers to a clean state.
 */
void TIMER_SETUP(void) {
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TSICR) |= DMTIMER_TSICR_POSTED;
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCLR)  &= ~DMTIMER_TCLR_ST;

    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCRR)   = 0x00000000;
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TLDR)   = 0x00000000;
}

/* ============================================================================
 * DELAY RELATED FUNCTIONS
 * ========================================================================= */

static uint32_t DMTimerWritePostedStatusGet(uint32_t baseAdd)
{
    /* Return the status of TWPS register */
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
#define DMTimerWaitForWrite(reg, baseAdd)                           \
    do {                                                            \
        if (HWREG(baseAdd + DMTIMER_TSICR) & DMTIMER_TSICR_POSTED) \
            while ((reg & DMTimerWritePostedStatusGet(baseAdd)));   \
    } while (0)

/**
 * @brief Sets the timer counter register.
 *
 * @param baseAdd Timer base address.
 * @param counter Value to load into @c DMTIMER_TCRR.
 */
void DMTimerCounterSet(uint32_t baseAdd, uint32_t counter) {
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCRR, baseAdd);
    HWREG(baseAdd + DMTIMER_TCRR) = counter;
}

/**
 * @brief Returns the current timer counter value.
 *
 * @param baseAdd Timer base address.
 * @return Current value of @c DMTIMER_TCRR.
 */
uint32_t DMTimerCounterGet(uint32_t baseAdd) {
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCRR, baseAdd);
    return HWREG(baseAdd + DMTIMER_TCRR);
}

/**
 * @brief Starts the timer by setting the ST bit in TCLR.
 *
 * @param baseAdd Timer base address.
 */
void DMTimerEnable(uint32_t baseAdd) {
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCLR, baseAdd);
    HWREG(baseAdd + DMTIMER_TCLR) |= DMTIMER_TCLR_ST;
}

/**
 * @brief Stops the timer by clearing the ST bit in TCLR.
 *
 * @param baseAdd Timer base address.
 */
void DMTimerDisable(uint32_t baseAdd) {
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCLR, baseAdd);
    HWREG(baseAdd + DMTIMER_TCLR) &= ~DMTIMER_TCLR_ST;
}

/** @brief Global flag set by DMTimer7 ISR on overflow. */
volatile uint32_t flag_timer = false;

/**
 * @brief DMTimer7 overflow interrupt service routine.
 *
 * Clears the overflow flag in hardware and signals
 * the delay function through @c flag_timer.
 */
void DMTimerIsr(void) {
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_IRQSTATUS) |= 0x2;
    flag_timer = true;
}

/**
 * @brief Blocking delay using DMTimer7.
 *
 * Loads the counter with a precomputed overflow value,
 * enables the overflow interrupt and busy-waits until
 * the ISR signals completion through @c flag_timer.
 *
 * @param us Delay duration in seconds.
 */
void delay(uint32_t us) {
    uint32_t countVal = TIMER_OVERFLOW - (us * 1000000 * TIMER_1US_COUNT );

    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCRR, SOC_DMTIMER_7_REGS);
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCRR) = countVal;

    flag_timer = false;

    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_IRQENABLE_SET) |= 0x2;
    DMTimerEnable(SOC_DMTIMER_7_REGS);

    while (flag_timer == false) {}

    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_IRQENABLE_CLR) |= 0x2;
    DMTimerDisable(SOC_DMTIMER_7_REGS);
}

/** @brief Unmasks DMTimer7 interrupt line (95) in the INTC. */
void timerIntConfig(void) {
    HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR2) |= (1 << 31);
}


