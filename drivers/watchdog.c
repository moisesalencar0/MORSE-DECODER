/**
 * @file watchdog.c
 * @brief Watchdog driver implementations
 */

#include "watchdog.h"
#include "hw_types.h"
#include "hw_regs.h"

/**
 * @brief Internal watchdog toggle value used for triggering.
 */
static vuint32_t wtgr_val = 0;

/**
 * @brief Triggers (kicks) the watchdog to prevent reset.
 *
 * Writes alternating values to the watchdog trigger register
 * and waits for write completion.
 */
void Watchdog_Trigger() {
    wtgr_val ^= 0xFFFFFFFF;
    HWREG(WDT1_WTGR) = wtgr_val;
    while (HWREG(WDT1_WWPS) & WWPS_PEND_WTGR);
}

/**
 * @brief Disables the watchdog timer.
 *
 * Writes the required unlock sequence to stop the watchdog
 * and waits for write completion after each step.
 */
void Watchdog_Disable() {
    HWREG(WDT1_WSPR) = 0xAAAA;
    while (HWREG(WDT1_WWPS) & WWPS_PEND_WSPR);

    HWREG(WDT1_WSPR) = 0x5555;
    while (HWREG(WDT1_WWPS) & WWPS_PEND_WSPR);
}