#include "stm32f10x.h"                  // Device header



void Tim3_Init(uint32_t x, uint32_t y) //用于信号调制时传输0，1的信号
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = x - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = y - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    NVIC_EnableIRQ(TIM3_IRQn);
    TIM_Cmd(TIM3, ENABLE);
}
