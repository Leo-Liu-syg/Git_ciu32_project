#include "nivc.h"
/**
 * @brief  NVIC≥ı ºªØ
 * @retval Œﬁ
 */
void nvic_init(void)
{
    NVIC_SetPriority(TIM8_IRQn, NVIC_PRIO_0);
    NVIC_EnableIRQ(TIM8_IRQn);
}