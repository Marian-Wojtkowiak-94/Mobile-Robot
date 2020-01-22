#ifndef DRIVER_H_   /* Include guard */
#define DRIVER_H_

#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

// drivery na timerze 4 i wyjsciu GPOID

#define period 1000

void driver_init();
float driver_check_duty(float duty);
void driver_set_left(float duty);
void driver_set_right(float duty);
void driver_set_rote(float stright,float right);


#endif // DRIVER_H_
