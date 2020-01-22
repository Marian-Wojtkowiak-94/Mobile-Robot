#ifndef LCD2x16_H_   /* Include guard */
#define LCD2x16_H_
% downloaded from  http://www.fmf.uni-lj.si/~ponikvar/STM32F407%20project/Alphanumeric%20LCD.pdf and changed
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"



#define D4 	GPIO_Pin_0
#define D5 	GPIO_Pin_1
#define D6 	GPIO_Pin_2
#define D7 	GPIO_Pin_3
#define E           GPIO_Pin_5
#define RS          GPIO_Pin_4

#define zegar_peryferium	  RCC_AHB1Periph_GPIOC
#define peryferium  GPIOC

#define MAX_STRING_SIZE 40

#define         de1     100
#define         de2     30              // == 1us, should work downto 300ns!
#define         de3     0x550           // does not work for 480

void delay(long loops);

void LCD_nib(char nib);

void LCD_char (char ch);

void LCD_cmd (char ch);

void LCD_string(char* str, char pos);

void LCD_clear (void);
void Bin2String (int arg1, char *ch);
void LCD_uInt32 (int arg1, char pos, char LZB);
void LCD_sInt32 (int arg1, char pos, char LZB);
void LCD_uInt16 (int arg1, char pos, char LZB);
void LCD_sInt16 (int arg1, char pos, char LZB);
void LCD_sInt3DG (int arg1, char pos, char LZB);
void LCD_printl (char line,char * format, ...);
void LCD_init (void);

#endif // LCD2x16_H_
