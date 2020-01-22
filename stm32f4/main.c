
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "math.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"

#include "LCD2x16.h"
#include "driver.h"
#include "servo.h"
#include "lain_feloer.h"
#include "interface.h"
#include "HC-SR04.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"
//#include "spiRas.h"
#include "robotkomunikacja.h"


void acelerometrInit(){
	auto IMU_Type = TM_LIS302DL_LIS3DSH_Detect();
	if(IMU_Type ==TM_LIS302DL_LIS3DSH_Device_LIS302DL){
		TM_LIS302DL_LIS3DSH_Init(TM_LIS302DL_Sensitivity_2_3G, TM_LIS302DL_Filter_2Hz);
	}
	if(IMU_Type ==TM_LIS302DL_LIS3DSH_Device_LIS3DSH){
		TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_16G, TM_LIS3DSH_Filter_800Hz);
	}
}

int s,l,k,kont;


void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		// miejsce na kod wywo�ywany w momencie wyst�pienia przerwania
		l=(l+1)%50;
		interface_deley_i++;
		HC_SR04_timer_incr++;
		if(l==0){
			s++;
			kont=kont+k;
			if(abs(kont)>90){
				k=-k;
				kont=kont+2*k;
			}
			//servo_set_b4((float)kont);
		}
		// wyzerowanie flagi wyzwolonego przerwania
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}


int main(void)
{
	s=0;
	l=0;
	k=10;
	kont=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemInit();
	init_USART1(9600); // initialize USART1 @ 9600 baud
	//init_USART1(115200);
	RasInitGPIOState();
	//init_disc_led();
	HC_SR04_init();
	driver_init();
	servo_init();
	servo_init_NVIC();
	lain_feloer_init();
	//spiRasInit();
	LCD_init();
	interface_init();
	acelerometrInit();


    while(1)
    {
    	interface_lop();
    }
}
