#include "HC-SR04.h"


void HC_SR04_init(){
	HC_SR04_init_pin();
	servo_init_NVIC();
	servo_init_timer();

}
void HC_SR04_init_pin(){
	RCC_AHB1PeriphClockCmd(HC_SR04_zegar_peryferium, ENABLE);
		GPIO_InitTypeDef  GPIO_InitStructure;

			GPIO_InitStructure.GPIO_Pin = HC_SR04_triger  ;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
			GPIO_Init(HC_SR04_peryferium, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin = HC_SR04_eho  ;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
			GPIO_Init(HC_SR04_peryferium, &GPIO_InitStructure);
}
int64_t HC_SR04_length_usec(){
	int64_t length=0;
	int imp_start,imp_end;
	GPIO_WriteBit(HC_SR04_peryferium,HC_SR04_triger,1);

	int imp_trig_length=TIM3->CNT+10%(20000 - 1);
	HC_SR04_timer_incr=0;
	while(TIM3->CNT<imp_trig_length){
		if(HC_SR04_timer_incr>2)
			return -1;
	}
	GPIO_WriteBit(HC_SR04_peryferium,HC_SR04_triger,0);
	HC_SR04_timer_incr=0;
	do{
		imp_start=TIM3->CNT;
		if(HC_SR04_timer_incr>2)
			return -1;
	}while(GPIO_ReadInputDataBit(HC_SR04_peryferium,HC_SR04_eho)==0);
	do{
			imp_end=TIM3->CNT;
			if(HC_SR04_timer_incr>2)
				return 30000;
	}while(GPIO_ReadInputDataBit(HC_SR04_peryferium,HC_SR04_eho)==1);
	length=imp_end-imp_start+20000*HC_SR04_timer_incr;
	return length;

}

int64_t HC_SR04_length_mm(){
	uint64_t odl=HC_SR04_length_usec()*0.17;
	if(odl<10)
		return -1;
	return odl;
}

int64_t HC_SR04_length_mm_avarge(int n){
	int64_t sred=0,prub;
	int blnd=0;
	for(int i=0;i<n;i++){
		for (int j=0;j<n;j++){
			prub=HC_SR04_length_mm();
			if(prub!=-1){
				break;
			}
		}
		if(prub==-1){
			blnd++;
			prub=0;
		}

		sred=sred+prub;
	}
	if(blnd>(n/2))
		return -1;
	return sred/n;

}
