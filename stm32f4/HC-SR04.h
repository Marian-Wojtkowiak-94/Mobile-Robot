#ifndef HC-SR04_H_   /* Include guard */
#define HC-SR04_H_


#define HC_SR04_triger 	GPIO_Pin_5
#define HC_SR04_eho		GPIO_Pin_4

#define HC_SR04_zegar_peryferium	  RCC_AHB1Periph_GPIOE
#define HC_SR04_peryferium  GPIOE

int HC_SR04_timer_incr;

#include "misc.h"
#include "math.h"

#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "LCD2x16.h"
#include "driver.h"
#include "servo.h"
#include "lain_feloer.h"

void HC_SR04_init();
void HC_SR04_init_pin();
int64_t HC_SR04_length_usec();
int64_t HC_SR04_length_mm();
int64_t HC_SR04_length_mm_avarge(int n);






#endif // HC-SR04_H_
