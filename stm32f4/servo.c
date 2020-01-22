
#include "servo.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "misc.h"

void servo_init_timer(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	  /* Time base configuration - SystemCoreClock = 168000000 for 168 MHz board */
	  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (((SystemCoreClock / 1000000) / 2) - 1); // Shooting for 1 MHz, (1us)
	  TIM_TimeBaseStructure.TIM_Period = 20000 - 1; // 1 MHz / 20000 = 50 Hz (20ms)
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

//	TIM_TimeBaseStructure.TIM_Period = period;
//	TIM_TimeBaseStructure.TIM_Prescaler = 41;
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_Cmd(TIM3, ENABLE);
}

void servo_init_pin() {
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* Alternating functions for pins */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);
    /* Set pins */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);




}

void servo_init_pwm(){
		TIM_OCInitTypeDef TIM_OCInitStructure;
	    /* PWM1 Mode configuration: */
	    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	    TIM_OCInitStructure.TIM_Pulse = 1500;
	    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

}

void servo_init(){
	servo_init_timer();
	servo_init_pin();
	servo_init_pwm();
	servo_set_b0(0);
	servo_set_b1(0);
	servo_set_b4(0);
	servo_set_b5(0);

}

float servo_check_kont(float kont){
	if(kont>90){
			kont=90;
		}
		if(kont<-90){
			kont=-90;
		}
		return kont;
}

void servo_set(int number,float kont){
	if(number==0){
		servo_set_b0(kont);
	}
	if(number==1){
		servo_set_b1(kont);
	}
	if(number==4){
		servo_set_b4(kont);
	}
	if(number==5){
		servo_set_b5(kont);
	}
}

void servo_set_b0(float kont){
	kont=servo_check_kont(kont);
	TIM3->CCR3=1500+(kont+swrvo_factor_add_b0)/90*swrvo_factor_multiplication_b0;
}

void servo_set_b1(float kont){
	kont=servo_check_kont(kont);
	TIM3->CCR4=1500+(kont+swrvo_factor_add_b1)/90*swrvo_factor_multiplication_b1;
}

void servo_set_b4(float kont){
	kont=servo_check_kont(kont);
	TIM3->CCR1=1500+(kont+swrvo_factor_add_b4)/90*swrvo_factor_multiplication_b4;
}

void servo_set_b5(float kont){
	kont=servo_check_kont(kont);
	TIM3->CCR2=1500+(kont+swrvo_factor_add_b5)/90*swrvo_factor_multiplication_b5;
}

void servo_init_NVIC(void){
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // wyczyszczenie przerwania od timera 3 (wyst¹pi³o przy konfiguracji timera)
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  // zezwolenie na przerwania od przepe³nienia dla timera 3
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

}
