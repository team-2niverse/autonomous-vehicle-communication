#ifndef BSW_GTM_ATOM_PWM_GTM_ATOM_PWM_H_
#define BSW_GTM_ATOM_PWM_GTM_ATOM_PWM_H_

#include "Ifx_Types.h"

#define ISR_PRIORITY_ATOM   20                                  /* Interrupt priority number                        */
#define PWM_A               IfxGtm_ATOM0_1_TOUT1_P02_1_OUT
#define PWM_B               IfxGtm_ATOM1_3_TOUT105_P10_3_OUT

#define CLK_FREQ            1000000.0f                          /* CMU clock frequency, in Hertz                    */
#define PWM_PERIOD          1000                                /* PWM period for the ATOM, in ticks                */

// Function Prototypes
void Gtm_Atom_Pwm_Init(void);
void Gtm_Atom_Pwm_Set_Duty_Cycle_A(uint32 dutyCycle);
void Gtm_Atom_Pwm_Set_Duty_Cycle_B(uint32 dutyCycle);

#endif /* BSW_GTM_ATOM_PWM_GTM_ATOM_PWM_H_ */
