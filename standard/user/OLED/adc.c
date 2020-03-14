#include "adc.h"
#include "delay.h"
#include "stm32f4xx.h"

void OLED_Button_ADC_init(void)
{
	/*--------------------------------GPIO init--------------------------------*/
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PA5 通道5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
	/*--------------------------------ADC init--------------------------------*/
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);

    ADC_TempSensorVrefintCmd(ENABLE);

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInit(&ADC_CommonInitStructure);
	
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure); 

    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles);
    ADC_Cmd(ADC1, ENABLE);
}

uint16_t get_ADC(uint8_t ch)
{
    ADC_ClearFlag(ADC1,ADC_FLAG_STRT|ADC_FLAG_OVR|ADC_FLAG_EOC);
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_15Cycles);

    ADC_SoftwareStartConv(ADC1);

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
    {
        ;
    }
    return ADC_GetConversionValue(ADC1);
}

//double get_temprate(void)
//{
//    uint16_t adcx = 0;
//    double temperate;
//    temperature_ADC_Reset();
//    adcx = get_ADC(ADC_Channel_18);
//    temperate = (double)adcx * (3.3f / 4096.0f);
//    temperate = (temperate - 0.76f) / 0.0025f + 25.0f;
//    return temperate;
//}
