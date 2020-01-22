#include "interface.h"
#include "float.h"
#include <math.h>


#define interface_i_max 8
int interface_i;
int interface_id;
char* interface_tekst[interface_i_max]={"lain felower","jazda","help","odleglosc","odleglosc jazda","read  acelerometr","rasberypi","PTM"};

void interface_init_buton(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* GPIOD Periph clock enable */


}


void interface_init (){
	interface_init_buton();
	LCD_init();
	interface_i=0;
	interface_id=0;

}

int jazda_state(){
	char zgoda=1,przycisk;
	int prosto=0,prawo=0;
	while(zgoda){
		LCD_printl(1,"jazda  3,4");
		LCD_printl(2,"f=%4d r=%4d",prosto,prawo);
		przycisk=interface_read_butons_if();
		if(przycisk==12){
			zgoda=0;
			driver_set_rote(0,0);
			break;
		}
		if(przycisk==1){
			prosto--;
			if(prosto<-100){
				prosto=-100;
			}
		}
		if(przycisk==2){
			prosto++;
			if(prosto>100){
				prosto=100;
			}
		}
		if(przycisk==4){
			prawo--;
			if(prawo<-100){
				prawo=-100;
			}
		}
		if(przycisk==8){
			prawo++;
			if(prawo>100){
				prawo=100;
			}
		}

		driver_set_rote((float)prosto/100,(float)prawo/100);

	}
	return zgoda;

}

void interface_lop(){
	char zgoda=1;
	while (zgoda){
	switch (interface_id){
		case 0:{
			char przycisk=interface_read_butons_lup();
			if(przycisk==1){
				interface_i--;
				if(interface_i<0){
					interface_i=interface_i_max-1;
				}
			}
			if(przycisk==2){
				interface_i++;
				interface_i=interface_i%interface_i_max;
			}
			LCD_printl(1,"Menu");
			LCD_printl(2,"%s",interface_tekst[interface_i]);
			if(przycisk==8){
				interface_id=interface_i+1;
			}

			break;
		}
		case 1:{
			interface_id=interface_id*lain_feloer_state();
			break;
		}
		case 2:{
			interface_id=interface_id*jazda_state();
			break;
		}
		case 3:{
			LCD_printl(1,"help");
			LCD_printl(2,"return");
			char przycisk=interface_read_butons_lup();
			if(przycisk==4){
				interface_id=0;
			}
			if(przycisk==8){
				interface_id=0;
				zgoda=0;
			}
			break;
		}
		case 4:{
					interface_id=interface_id*odleglosc_state();
					break;
		}
		case 5:{
					interface_id=interface_id*odleglosc_jazda_state();
					break;
		}
		case 6:{
					interface_id=interface_id*acelerometr_state();
					break;
		}
		case 7:{
					interface_id=interface_id*RasState();
					break;
		}
		case 8:{
					interface_id=interface_id*PTM_state();
					break;
		}

	}

	if(RasButonEneble()==1){
		interface_id=7;
	}
	}
}

char interface_read_butons_if(){
	char wynik=0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
		wynik=wynik|(1<<0);
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)){
		wynik=wynik|(1<<1);
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)){
		wynik=wynik|(1<<2);
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)){
		wynik=wynik|(1<<3);
	}
	for(int i=0;i<10000;i++){}
	return wynik;
}

char interface_read_butons_lup(){
	char wynik=0;
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
		wynik=wynik|(1<<0);
	}
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)){
		wynik=wynik|(1<<1);
	}
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)){
		wynik=wynik|(1<<2);
	}
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)){
		wynik=wynik|(1<<3);
	}
	return wynik;
}

int odleglosc_state(){
	char zgoda=1,przycisk;
	int64_t length;
	while(zgoda){
		LCD_printl(1,"odleglosc  3,4");
		length=HC_SR04_length_mm();
		LCD_printl(2,"length=%5d ",length);
		przycisk=interface_read_butons_if();
		if(przycisk==12){
			zgoda=0;
			driver_set_rote(0,0);
			break;
		}


	}
	return zgoda;
}

int  acelerometr_state(){
	char zgoda=1,przycisk;
	int64_t length;
	while(zgoda){
//		LCD_printl(1,"odleglosc  3,4");
//		length=HC_SR04_length_mm();
//		LCD_printl(2,"length=%5d ",length);


    	TM_LIS302DL_LIS3DSH_t osie;
    	TM_LIS302DL_LIS3DSH_ReadAxes(&osie);
    	LCD_printl(1,"x=%4d 3,4",osie.X);
    	LCD_printl(2,"y=%4d z=%4d ",osie.Y,osie.Z);
		przycisk=interface_read_butons_if();

		if(przycisk==12){
			zgoda=0;
			driver_set_rote(0,0);
			break;
		}


	}
	return zgoda;
}


int odleglosc_jazda_state(){
	char zgoda=1,przycisk,zgoda_jazda=0;
	int64_t length=-1,prawo,lewo;
	while(zgoda){
		LCD_printl(1,"odle_jazda  3,4");
			length=-1;
			while (length==-1){
				length=HC_SR04_length_mm();
			}
		if ((length<100)&&(length!=-1)){

			driver_set_rote(0,0);
			interface_deley(1000000);
			servo_set_b4(90);
			interface_deley(2000000);
			lewo=-1;
			while(lewo==-1){
				lewo=HC_SR04_length_mm();
			}
			servo_set_b4(-90);
			interface_deley(2000000);
			prawo=-1;
			while(prawo==-1){
				prawo=HC_SR04_length_mm();
			}
			servo_set_b4(0);
			interface_deley(2000000);
			if((prawo>100)||(lewo>100)){
				if(prawo>100){
					driver_set_rote(0,1);
					interface_deley(1000000);
					driver_set_rote(1,0);

				}
				else{
				if(lewo>100){
					driver_set_rote(0,-1);
					interface_deley(1000000);
					driver_set_rote(1,0);

				}
				}
			}
		}

			driver_set_rote(zgoda_jazda,0);

		LCD_printl(2,"length=%5d ",length);
		przycisk=interface_read_butons_if();
		if(przycisk==12){
			zgoda=0;
			driver_set_rote(0,0);
			break;
		}
		if(przycisk==2){
			zgoda_jazda=1;
		}


	}
	return zgoda;
}

void interface_deley(int us){
	unsigned char deley_start=interface_deley_i;
	int dley_cunt_start= TIM3->CNT;
	while((TIM3->CNT-dley_cunt_start+20000*(interface_deley_i-deley_start))<us){

	}
}


int PTM_state(){
	char zgoda=1,przycisk;
	int64_t length,length1;
	while(zgoda){
		LCD_printl(1,"PTM  3,4");
//		length=HC_SR04_length_mm();
//		LCD_printl(2,"length=%5d ",length);



    	float kont=Robot_kont_bez();

		length1=HC_SR04_length_mm();
		if(length1>0){
			length=length1;
		}
		if((kont<30)&&(length>100)){
			lain_feloer_set(lain_feloer_kierunek(lain_feloer_ADCConvertedValues[0], lain_feloer_ADCConvertedValues[1]));
		}
		else{
			driver_set_rote(0,0);
		}
//    	LCD_printl(1,"x=%4d 3,4",osie.X);
    	LCD_printl(2,"k=%2d l=%5d",(int)kont,length);
		przycisk=interface_read_butons_if();

		if(przycisk==12){
			zgoda=0;
			driver_set_rote(0,0);
			break;
		}


	}
	return zgoda;
}

float Robot_kont_bez(){
	TM_LIS302DL_LIS3DSH_t osie;
	TM_LIS302DL_LIS3DSH_ReadAxes(&osie);
	double poziom=sqrt(osie.X*osie.X+osie.Y*osie.Y);
	return (float) atan2(poziom,osie.Z)*180/3.14;
}
float Robot_kont(TM_LIS302DL_LIS3DSH_t osie){

	//TM_LIS302DL_LIS3DSH_ReadAxes(&osie);
	double poziom=sqrt(osie.X*osie.X+osie.Y*osie.Y);
	return (float) atan2(poziom,osie.Z)*180/3.14;
}
