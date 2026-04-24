
#include "adc.h"
/*-------------------------------------------variables------------------------------------------*/
__IO uint8_t g_adc_complete = 0;
__IO uint16_t g_dma_result[2];
/*-------------------------------------------functions------------------------------------------*/
/**
 * @brief  ADC初始化配置函数
 * @retval 无
 */
void adc_init(void)
{
    std_adc_init_t adc_config = {0};

    /* 使能ADC时钟 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_ADC);

    /* ADC初始化配置 */
    adc_config.clock_src = ADC_SYNC_CLK_PCLK_DIV1;       /* 同步时钟PCLK，不分频 */
    adc_config.conver_mode = ADC_CONTINUOUS_CONVER_MODE; /* 连续转换模式 */
    adc_config.overrun_mode = ADC_OVRN_MODE_OVERWRITTEN; /* 数据溢出模式: 数据覆盖 */
    adc_config.trigger_edge = ADC_TRIG_HW_EDGE_RISING;   /* 上升沿触发 */
    adc_config.trigger_source = ADC_EXTRIG_TIM3_TRGO;    /* Timer触发 */
    adc_config.sampt1 = ADC_SAMPTIME_119CYCLES;          /* 采样时间设置为119CLK，组SAMPT1 */
    adc_config.sampt2 = ADC_SAMPTIME_119CYCLES;          /* 采样时间设置为119CLK，组SAMPT2 */
    adc_config.scan_dir = ADC_SCAN_DIR_FORWARD;          /* 正向扫描 */
    std_adc_init(&adc_config);

    /* 选择通道1/3 */
    std_adc_fix_sequence_channel_enable(ADC_CHANNEL_1); // smoke
    std_adc_fix_sequence_channel_enable(ADC_CHANNEL_3); // CO

    /* 配置通道1/3采样时间选择组2 */
    std_adc_channel_sample_time_select(ADC_SAMPTIME_GROUP_2, ADC_CHANNEL_1);
    std_adc_channel_sample_time_select(ADC_SAMPTIME_GROUP_2, ADC_CHANNEL_3);

    /* 使能ADC的DMA传输 */
    std_adc_dma_enable();

    /* 配置wait模式，避免数据未及时读取，转换溢出 */
    std_adc_wait_mode_enable();

    /* ADC参考电压源选择 */
    std_adc_set_reference_source(ADC_REFERENCE_VREFBUF);

    /* 使能ADC */
    std_adc_enable();

    /* 等待ADC使能状态稳定
       当ADC_CK时钟大于等于6MHz，需等待2.5us稳定时间
       当ADC_CK时钟小于6MHz，需等待17个ADC时钟周期稳定时间
    */
    std_delayus(ADC_EN_DELAY);

    /* 使能校准 */
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

    /* DMA外设时钟使能 */
    std_rcc_ahb_clk_enable(RCC_PERIPH_CLK_DMA);

    /* dma_init 结构体初始化 */
    dma_init_param.dma_channel = DMA_CHANNEL_0;
    dma_init_param.transfer_type = DMA_BLOCK_TRANSFER;
    dma_init_param.dma_req_id = DMA_REQUEST_ADC;
    dma_init_param.src_addr_inc = DMA_SRC_INC_DISABLE;
    dma_init_param.dst_addr_inc = DMA_DST_INC_ENABLE;
    dma_init_param.data_size = DMA_DATA_SIZE_HALFWORD;
    dma_init_param.mode = DMA_MODE_CIRCULAR;

    /* DMA初始化 */
    std_dma_init(&dma_init_param);

    /* 使能传输完成中断 */
    std_dma_interrupt_enable(DMA_CHANNEL_0, DMA_INTERRUPT_TF);

    /* NVIC初始化 */
    NVIC_SetPriority(DMA_Channel0_IRQn, NVIC_PRIO_1);
    NVIC_EnableIRQ(DMA_Channel0_IRQn);
}

void vrefbuf_init(void)
{
    /* 使能VREFBUF外设时钟 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_SYSCFG);
    /* 使能PMU时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);

    /* 使能VBGR */
    std_pmu_bgr_enable();

    /* 等待BGR稳定 */
    std_delayus(BGR_RDY_DELAY);

    /* 禁止vrefbuf */
    std_vrefbuf_disable();
    /* 写入vrefbuf校准值 */
    std_rcc_write_vrefbuf_calibration(VREFBUF_CAL_VALUE_3000);
    /* 配置VREFBUF输出电压3.0V */
    std_vrefbuf_output_config(VREFBUF_VOLTAGE_3000);
    /* 使能vrefbuf */
    std_vrefbuf_enable();

    /* 等待VREFBUF稳定，用户可根据对VREFBUF精度要求，调整该时间参数，单位:us */
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
        /* 清除DMA通道转换完成标志 */
        std_dma_clear_flag(DMA_FLAG_TF0);
        g_adc_complete = 1;
    }
}

/**
 * @brief  配置DMA源地址、目的地址和传输数据大小，并使能DMA
 * @retval 无
 */
void bsp_adc_dma_config(void)
{
    std_dma_config_t dma_config = {0};

    dma_config.dma_channel = DMA_CHANNEL_0;
    dma_config.src_addr = (uint32_t)&ADC->DR;
    dma_config.dst_addr = (uint32_t)&g_dma_result;
    dma_config.data_number = 2;
    std_dma_start_transmit(&dma_config);
}
