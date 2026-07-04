//
// Created by yes on 6/29/26.
//

#include <bdk.h>
#include "timeout.h"
#include "error.h"

saveState_t savedState = {0};

extern u32 saveState(saveState_t *saveState);
extern u32 restoreState(saveState_t *saveState);
extern void irq_enable_cpu_irq_exceptions();
extern void overrideIRQ();

int _irqCHandler(u32 irq, void* data) {
    restoreState(&savedState);
    return IRQ_HANDLED;
}

int executeWithTimeout(u32 timeoutUS, workPointer worker, void* args) {
    int res;

    if (!saveState(&savedState)) {
        overrideIRQ();
        TMR(TIMER_TMR8_TMR_PTV) = TIMER_EN | timeoutUS;
        irq_request(IRQ_TMR8, &_irqCHandler, &savedState, IRQ_FLAG_ONE_OFF);
        res = worker(args);

    } else {
        savedState = (saveState_t){0};
        drawError(newError(TE_ERROR_IO)); // TODO: make a dedicated error for this.
        res = -1;
    }

    TMR(TIMER_TMR8_TMR_PCR) = TIMER_INTR_CLR;
    irq_free(IRQ_TMR8);

    return res;
}
