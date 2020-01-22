#ifndef robotkomunikacja_H_   /* Include guard */
#define robotkomunikacja_H_

//#include  "stdbool.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "LCD2x16.h"
#include "interface.h"
#include <stm32f4xx.h>
#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include <stm32f4xx_usart.h> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src

#include "lain_feloer.h"
#include "driver.h"
#include "HC-SR04.h"
#include "interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_STRLEN 100 // this is the maximum string length of our string in characters
char received_string[MAX_STRLEN+1]; // this will hold the recieved string

void Delay(__IO uint32_t nCount) ;
void init_USART1(uint32_t baudrate);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);// this is the interrupt request handler (IRQ) for ALL USART1 interrupts
void USART1_IRQHandler(void);

void spiRasInit();
void spiRasInitSPI();
void RasInitGPIO();
void RasInitGPIOState();
int RasButonEneble();
void RasOdbiur();
void RasNadawaj();
int RasState();
void RasComunicat(SPI_TypeDef* SPIx,char *send,char *receive,int N);



#endif // spiRas_H_ lain_feloer
