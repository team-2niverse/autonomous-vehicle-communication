#include "Stm.h"
#include "IfxCpu.h"

uint64 Stm_Get_Tick(void) {
    uint64 result = MODULE_STM0.TIM0.U | ((uint64)MODULE_STM0.CAP.U << 32);
    return result;
}

uint64 Stm_Get_Time_Us(void) {
    uint64 result = MODULE_STM0.TIM0.U | ((uint64)MODULE_STM0.CAP.U << 32);
    return result / CPU_CLOCK_MHZ;
}

uint64 Stm_Get_Time_Ms(void) {
    uint64 result = MODULE_STM0.TIM0.U | ((uint64)MODULE_STM0.CAP.U << 32);
    return result / (CPU_CLOCK_MHZ*1000);
}
