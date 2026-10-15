/*
	6.5:加入旋钮调节频率，波形，幅值考虑使用旋钮的按键功能，使用外部中断
		加一个数组用于按键模式切换，使用switch函数，添加步长修改
	6.6：旋钮反应不灵敏//未解决，部分频率波形无法生成，（因为频率数值太大？），正弦波最大应该得输出
		1mHz，按键状态调整有缺陷，考虑加入ask/fsk信号调制功能
		添加菜单功能（状态判定怎么实现？）；ask调制（使用定时器传输01数据？晶振频率能否做到？）；
		fsk调制：同样使用定时器，然后同样判断01，并且加入倍率选择功能，一个旋钮是否够用？
		PSK:过程同上，直接使用AD9833变相函数
		ask调制的时候直接将amp数值改为0？
		6.7：解调如何设计？
*/

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD9833.h"
#include "IC.h"
#include "MySPI.h"
#include "PWM.h"
#include "Encoder.h"
#include "KEY.h"
#include "Timer3.h"
uint16_t Key[4] = {1, 2, 3, 4}, Key_Index, Form,  amp=128;
uint32_t freq,Step_Size = 1,x=1,y=1;
int main(void)
{
    AD9833_Init();
	Tim3_Init(1000,1);
    Encoder_Init();
    Key_Init();
    OLED_Init();
	OLED_ShowString(1,1,"Form:");
	OLED_ShowString(2,1,"Step:");
	OLED_ShowString(3,1,"Freq:");
	OLED_ShowString(4,1,"Amp:");
    while (1)
    {

    }
}

void EXTI0_IRQHandler(void)//反转为减
{
    if (EXTI_GetITStatus(EXTI_Line0) == SET)
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
        {
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
            {
                switch(Key[Key_Index])
                {
                case 1:
                {
                    Form--;
                    if(Form < 0) Form = 0;
					if(Form==0) OLED_ShowString(1,6,"TRI");
					if(Form==1) OLED_ShowString(1,6,"SIN");
					if(Form==2) OLED_ShowString(1,6,"SQU");	
                }
                break;
                case 2:
                {
                    Step_Size /= 10;
                    if(Step_Size == 0) Step_Size = 1;
					OLED_ShowNum(2,6,Step_Size,7);
					
                }
                break;
                case 3:
                {
                    freq -= Step_Size;
                    if(freq == 0) freq = 1;
					OLED_ShowNum(3,6,freq,8);
					OLED_ShowString(3,14,"Hz");
                }
                break;
                case 4:
                {
                    amp--;
                    if(amp == 0) amp = 1;
					OLED_ShowNum(4,5,amp,3);
					
                }
                break;
                }
                AD9833_WaveSeting(freq, 0, Form, 0);
                //此方向定义为反转，计数变量自减
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void)//正转为增
{
    if (EXTI_GetITStatus(EXTI_Line1) == SET)
    {

        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
        {
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
            {
                switch(Key[Key_Index])
                {
                case 1:
                {
                    Form++;
                    if(Form > 3) Form = 3;
					if(Form==0) OLED_ShowString(1,6,"TRI");
					if(Form==1) OLED_ShowString(1,6,"SIN");
					if(Form==2) OLED_ShowString(1,6,"SQU");	
					
                }
                break;
                case 2:
                {
                    Step_Size *= 10;
                    if(Step_Size == 0) Step_Size = 1;
					OLED_ShowNum(2,6,Step_Size,7);
					
                }
                break;
                case 3:
                {
                    freq += Step_Size;
                    if(freq == 0) freq = 1;
					OLED_ShowNum(3,6,freq,8);
					OLED_ShowString(3,14,"Hz");
					
                }
                break;
                case 4:
                {
                    amp++;
                    if(amp > 255) amp = 255;
					OLED_ShowNum(4,5,amp,3);
					
					AD9833_AmpSet(amp);
                }
                break;
                }
				AD9833_WaveSeting(freq, 0, Form, 0);
                //此方向定义为正转，计数变量自增
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line10))
    {
        Key_Index++;
        Key_Index%= 4;
		
    }
    EXTI_ClearITPendingBit(EXTI_Line10);
}

void TIM3_IRQHandler(void)//同样的，定义一个01数组，过多久传输一个bit即可，理论上传输速度可达7.2M
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update))
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}
