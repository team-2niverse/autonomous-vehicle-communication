#ifndef BSW_TOF_TOF_H_
#define BSW_TOF_TOF_H_

#include "Ifx_Types.h"

unsigned int Tof_getDist(void);
void Tof_setDist(uint8 *rxData);
uint64 Tof_getTofTime(void);
void Tof_pollCanSend(void);

#endif /* BSW_TOF_TOF_H_ */
