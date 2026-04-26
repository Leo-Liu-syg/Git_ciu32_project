#include "adc.h"

/*------------------------------------------- 变量定义 ------------------------------------------*/
// ADC采集完成标志
__IO uint8_t g_adc_complete = 0;
// DMA采集结果缓冲区
__IO uint16_t g_dma_result[3];

/*------------------------------------------- 函数实现 ------------------------------------------*/

/**
 * @brief  ADC初始化配置
 * @retval 无
 */
void adc_init(void)
{
    std_adc_init_t adc_config = {0};

    /* 使能ADC时钟 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_ADC);

    /* ADC基础参数配置 */
    adc_config.clock_src = ADC_SYNC_CLK_PCLK_DIV1;       // 同步时钟PCLK不分频
    adc_config.conver_mode = ADC_CONTINUOUS_CONVER_MODE; // 连续转换模式
    adc_config.overrun_mode = ADC_OVRN_MODE_OVERWRITTEN; // 溢出模式：数据覆盖
    adc_config.trigger_edge = ADC_TRIG_HW_EDGE_RISING;   // 硬件触发边沿：上升沿
    adc_config.trigger_source = ADC_EXTRIG_TIM3_TRGO;    // 触发源：TIM3_TRGO
    adc_config.sampt1 = ADC_SAMPTIME_119CYCLES;          // 采样时间配置为119个时钟周期
    adc_config.sampt2 = ADC_SAMPTIME_119CYCLES;          // 采样时间配置为119个时钟周期
    adc_config.scan_dir = ADC_SCAN_DIR_FORWARD;          // 扫描方向：正向扫描
    std_adc_init(&adc_config);

    /* 选择ADC通道1 / 通道2 /通道3 DMA按照通道顺序进行数据搬运*/

    std_adc_fix_sequence_channel_enable(ADC_CHANNEL_1); // 烟雾传感器通道

    std_adc_fix_sequence_channel_enable(ADC_CHANNEL_2); // Verf通道

    std_adc_fix_sequence_channel_enable(ADC_CHANNEL_3); // CO传感器通道

    /* 通道1 / 通道2 /通道3 采样时间选择组2 */
    std_adc_channel_sample_time_select(ADC_SAMPTIME_GROUP_2, ADC_CHANNEL_1);

    std_adc_channel_sample_time_select(ADC_SAMPTIME_GROUP_2, ADC_CHANNEL_2);

    std_adc_channel_sample_time_select(ADC_SAMPTIME_GROUP_2, ADC_CHANNEL_3);

    /* 使能ADC DMA传输 */
    std_adc_dma_enable();

    /* 使能等待模式，防止数据未读取就被覆盖 */
    std_adc_wait_mode_enable();

    /* ADC参考电压源选择 */
    std_adc_set_reference_source(ADC_REFERENCE_VREFBUF);

    /* 使能ADC外设 */
    std_adc_enable();

    /* 等待ADC稳定
       当ADC时钟高于6MHz，等待2.5us
       当ADC时钟低于6MHz，等待17个ADC时钟
    */
    std_delayus(ADC_EN_DELAY);

    /* 使能ADC校准 */
    std_adc_calibration_enable();

    /* 等待校准完成 */
    while (!std_adc_get_flag(ADC_FLAG_EOCAL))
        ;
}

/**
 * @brief  DMA初始化
 * @retval 无
 */
void dma_init(void)
{
    std_dma_init_t dma_init_param = {0};

    /* 使能DMA时钟 */
    std_rcc_ahb_clk_enable(RCC_PERIPH_CLK_DMA);

    /* DMA初始化结构体配置 */
    dma_init_param.dma_channel = DMA_CHANNEL_0;
    dma_init_param.transfer_type = DMA_BLOCK_TRANSFER;
    dma_init_param.dma_req_id = DMA_REQUEST_ADC;
    dma_init_param.src_addr_inc = DMA_SRC_INC_DISABLE;
    dma_init_param.dst_addr_inc = DMA_DST_INC_ENABLE;
    dma_init_param.data_size = DMA_DATA_SIZE_HALFWORD;
    dma_init_param.mode = DMA_MODE_CIRCULAR;

    /* 初始化DMA */
    std_dma_init(&dma_init_param);

    /* 使能DMA传输完成中断 */
    std_dma_interrupt_enable(DMA_CHANNEL_0, DMA_INTERRUPT_TF);

    /* NVIC中断配置 */
    NVIC_SetPriority(DMA_Channel0_IRQn, NVIC_PRIO_1);
    NVIC_EnableIRQ(DMA_Channel0_IRQn);
}

/**
 * @brief  内部参考电压VREFBUF初始化
 * @retval 无
 */
void vrefbuf_init(void)
{
    /* 使能SYSCFG时钟 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_SYSCFG);
    /* 使能PMU时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);

    /* 使能内部带隙基准源BGR */
    std_pmu_bgr_enable();

    /* 等待BGR稳定 */
    std_delayus(BGR_RDY_DELAY);

    /* 关闭VREFBUF */
    std_vrefbuf_disable();
    /* 写入VREFBUF校准值 */
    std_rcc_write_vrefbuf_calibration(VREFBUF_CAL_VALUE_3000);
    /* 配置VREFBUF输出电压3.0V */
    std_vrefbuf_output_config(VREFBUF_VOLTAGE_3000);
    /* 使能VREFBUF */
    std_vrefbuf_enable();

    /* 等待VREFBUF稳定 */
    std_delayus(VREFBUF_RDY_DELAY);
}

/**
 * @brief  DMA通道0中断服务函数
 * @retval 无
 */
void DMA_Channel0_IRQHandler(void)
{
    if ((std_dma_get_interrupt_enable(DMA_CHANNEL_0, DMA_INTERRUPT_TF)) && (std_dma_get_flag(DMA_FLAG_TF0)))
    {
        /* 清除DMA通道0传输完成标志 */
        std_dma_clear_flag(DMA_FLAG_TF0);
        // 置位ADC采集完成标志
        g_adc_complete = 1;
    }
}

/**
 * @brief  配置ADC DMA源地址、目标地址、数据长度并启动传输
 * @retval 无
 */
void bsp_adc_dma_config(void)
{
    std_dma_config_t dma_config = {0};

    dma_config.dma_channel = DMA_CHANNEL_0;
    dma_config.src_addr = (uint32_t)&ADC->DR;
    dma_config.dst_addr = (uint32_t)&g_dma_result;
    dma_config.data_number = 3;
    std_dma_start_transmit(&dma_config);
}