#ifndef __ADC_H_
#define __ADC_H_


#define BGR_RDY_DELAY            (30UL)
#define VREFBUF_RDY_DELAY        (1000UL)

#include "main.h"

extern __IO uint8_t g_adc_complete;
extern __IO uint16_t g_dma_result[3];
    
/*-------------------------------------------functions------------------------------------------*/
void adc_init(void);
void dma_init(void);
void vrefbuf_init(void);
void DMA_Channel0_IRQHandler(void);
void bsp_adc_dma_config(void);

#endif /* ADC_H */

