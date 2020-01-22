#ifndef interface_H   /* Include guard */
#define interface_H_


#include "misc.h"
#include "math.h"

#include "LCD2x16.h"
#include "driver.h"
#include "servo.h"
#include "lain_feloer.h"
#include "HC-SR04.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"
//#include "spiRas.h"
#include "robotkomunikacja.h"

unsigned char interface_deley_i;

void interface_deley(int us);
void interface_init_buton(); // inicjalizuje przyciski
void interface_init ();
void interface_lop();
char interface_read_butons_if(); // sprawdza czy dany przycisk / przyciski s¹ wcisniete bez czekania na ich zwolnienie
char interface_read_butons_lup();// sprawdza czy dany przycisk jest wcisniety z czekania na zwolnienie go
int jazda_state();
int odleglosc_state();
int acelerometr_state();
int PTM_state();
float Robot_kont_bez();
float Robot_kont(TM_LIS302DL_LIS3DSH_t osie);




#endif // interface_H_
