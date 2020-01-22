
#include "LCD2x16.h"
#include <stdio.h>
#include <stdarg.h>


void delay(long loops)       {
  while (loops-- > 0)   {}; 
}

void LCD_nib(char nib)  {
           // clear data lines
	  GPIO_WriteBit(peryferium,D4,nib & 0x01);
	  GPIO_WriteBit(peryferium,D5,nib & 0x02);
	  GPIO_WriteBit(peryferium,D6,nib & 0x04);
	  GPIO_WriteBit(peryferium,D7,nib & 0x08);
	  delay(de1);
	  GPIO_SetBits(peryferium, E);
	  delay(de2);
	  GPIO_ResetBits(peryferium,E);   // wiggle E
}

void LCD_char (char ch) {
  GPIO_SetBits(peryferium,RS);                 // use data register
  LCD_nib((ch & 0xf0) >> 4);            delay(de3);     // send MS nibble
  LCD_nib( ch & 0x0f);                  delay(de3);     // send LS nibble
}

void LCD_cmd (char ch) {
  GPIO_ResetBits(peryferium,RS);                // use command register
  LCD_nib((ch & 0xf0) >> 4);            delay(de3);     // send MS nibble
  LCD_nib( ch & 0x0f);                  delay(de3);     // send LS nibble
}

void LCD_string(char* str, char pos) {
	int i=0;
  LCD_cmd ((pos & 0x7f) + 0x80);        // set position on screen
  while (*str != '\0') {                // send all characters in a string
    LCD_char (*str++);
    i++;
  };
  for(;i<40;i++){LCD_char (' ');}
}

void LCD_clear (void)   {
  LCD_cmd (0x01);                       // clear screen command
  delay(0x10000);                       // 1.6ms to clear the display
}

void Bin2String (int arg1, char *ch)     {
int i, j, s = 0;
  if (arg1 < 0) {s = 1; arg1 = -arg1;};
  for (i=0; i<27; i++)  {                       // 36us to convert bin -> BCD
    for (j=6; j>=0; j--) ch[j] <<= 1;
    for (j=6; j>0; j--) {if (ch[j] & 0x10) { ch[j-1]++; ch[j] &= 0x0f;};};
    arg1 <<= 1;
    if (arg1 & 0x08000000) ch[6]++; 
    if (i < 26)
      for (j=6; j>=0; j--) {
        if (ch[j] >= 0x05)   ch[j] += 3;
      };
  };
  for (j=6; j>=0; j--) ch[j+1] = ch[j] + '0';     ch[8] = '\0';
  ch[0] = ' '; if (s == 1) ch[0] = '-';
}

void LCD_uInt32 (int arg1, char pos, char LZB)   {
char ch[10] = {0,0,0,0,0,0,0,0,0,0};
int j;
  Bin2String(arg1, ch);
  for (j=1; j<=8; j++) ch[j-1] = ch[j];
  j = 0; if (LZB != 0) while (ch[j] == '0' && j < 6) ch[j++] = ' ';
  LCD_string (ch, pos);                         // 750us to print
}

void LCD_sInt32 (int arg1, char pos, char LZB)   {
char ch[10] = {0,0,0,0,0,0,0,0,0,0};
int j;
  Bin2String(arg1, ch);
  j = 1; if (LZB != 0) while (ch[j] == '0' && j < 6) ch[j++] = ' ';
  LCD_string (ch, pos);                         // 750us to print
}

void LCD_uInt16 (int arg1, char pos, char LZB)   {
char ch[10] = {0,0,0,0,0,0,0,0,0,0};
int j;
  Bin2String(arg1, ch);
  for (j=3; j<=8; j++) ch[j-3] = ch[j];
  j =0; if (LZB != 0) while (ch[j] == '0' && j < 4) ch[j++] = ' ';
  LCD_string (ch, pos);                         // 550us to print
}

void LCD_sInt16 (int arg1, char pos, char LZB)   {
char ch[10] = {0,0,0,0,0,0,0,0,0,0};
int j;
  Bin2String(arg1, ch);
  for (j=3; j<=8; j++) ch[j-2] = ch[j];
  j = 1; if (LZB != 0) while (ch[j] == '0' && j <= 4) ch[j++] = ' ';
  LCD_string (ch, pos);                         // 550us to print
}

void LCD_sInt3DG (int arg1, char pos, char LZB)   {
int j;
char ch[10] = {0,0,0,0,0,0,0,0,0,0};
  Bin2String(arg1, ch);
  for (j=5; j<=8; j++) ch[j-4] = ch[j];
  j = 1; if (LZB != 0) while (ch[j] == '0' && j <= 2) ch[j++] = ' ';
  LCD_string (ch, pos);                         // 
}

void LCD_printl (char line,char * format, ...){
  char buffer[40];
  va_list args;
  va_start (args, format);
  vsnprintf (buffer, 40 ,format, args);
  if(line==1)
	  LCD_string(buffer,0x00);
  else
	  LCD_string(buffer,0x40);
   va_end (args);

}

void LCD_init (void) {
	RCC_AHB1PeriphClockCmd(zegar_peryferium, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;

		GPIO_InitStructure.GPIO_Pin = D4 | D5 | D6 | D7 | E | RS ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(peryferium, &GPIO_InitStructure);
		/* GPIOD Periph clock enable */
  LCD_nib(0x03);        delay(0x28000);
  LCD_nib(0x03);        delay(0x2000);
  LCD_nib(0x03);        delay(0x1000);
  LCD_nib(0x02);        delay(0x1000);
  LCD_cmd(0x28);
  LCD_cmd(0x0c);
  LCD_cmd(0x01);
  LCD_cmd(0x06);
  delay(0x10000);       
}
