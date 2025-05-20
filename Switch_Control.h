#ifndef SWITCH_CONTROL_H
#define SWITCH_CONTROL_H

#include <MKL46Z4.h>

// Function declarations
void operate_switch_polling();
void operate_switch_interrupts();
void PORTC_PORTD_IRQHandler(void);

#endif /* SWITCH_CONTROL_H */
