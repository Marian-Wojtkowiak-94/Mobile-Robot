
#include "driver.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void driver_init_timer(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period = period-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_Cmd(TIM4, ENABLE);
}

void driver_init_pin() {
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* Alternating functions for pins */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
    /* Set pins */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);




}

void driver_init_pwm(){
		TIM_OCInitTypeDef TIM_OCInitStructure;
	    /* PWM1 Mode configuration: */
	    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	    TIM_OCInitStructure.TIM_Pulse = 0;
	    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

}

void driver_init(){
	driver_init_timer();
	driver_init_pin();
	driver_init_pwm();
}

float driver_check_duty(float duty){
		if(duty>1){
			duty=1;
		}
		if(duty<-1){
			duty=-1;
		}
		return duty;
}

void driver_set_left(float duty){
	duty=driver_check_duty(duty);
	if(duty<0){
		TIM4->CCR2=-duty*period;
		TIM4->CCR4=0;
	}
	else{
		TIM4->CCR4=duty*period;
		TIM4->CCR2=0;
	}
}

void driver_set_right(float duty){
	duty=driver_check_duty(duty);
	if(duty<0){
		TIM4->CCR1=-duty*period;
		TIM4->CCR3=0;
	}
	else{
		TIM4->CCR3=duty*period;
		TIM4->CCR1=0;
	}
}

void driver_set_rote(float stright,float right){
	if(stright<0){
		right=-right;
	}
	driver_set_left(stright+right);
	driver_set_right(stright-right);
}


