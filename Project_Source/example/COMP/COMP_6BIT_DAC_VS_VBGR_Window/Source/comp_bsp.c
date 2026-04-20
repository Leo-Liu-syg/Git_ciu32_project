/************************************************************************************************/
/**
* @file               comp_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              COMP BSP驱动函数。
*                     
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "comp_bsp.h"
#include "common.h"
/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  检查输入电压
* @retval 无
*/
uint32_t bsp_input_voltage_level_check(void)
{
    uint32_t state = 0U;
    
    /* COMP1和COMP2均输出高：输入高于阈值上限 */
    if ((std_comp_get_output_result(COMP1))
        && (std_comp_get_output_result(COMP2)))
    {
        state = STATE_OVER_THRESHOLD;
    }
    /* COMP1输出高，COMP2输出低：输入在阈值上限和下限之间 */
    else if ((std_comp_get_output_result(COMP1))
        && (!std_comp_get_output_result(COMP2)))
    {
        state = STATE_WITHIN_THRESHOLD;
    }
    /*  COMP1输出低，COMP2输出低：输入低于阈值下限 */
    else if ((!std_comp_get_output_result(COMP1))
        && (!std_comp_get_output_result(COMP2)))
    {
        state = STATE_UNDER_THRESHOLD;
    }
    return state;
}
