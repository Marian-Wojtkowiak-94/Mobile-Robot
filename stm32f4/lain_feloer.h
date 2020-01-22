#ifndef LAIN_FELOER_H_   /* Include guard */
#define LAIN_FELOER_H_

#include "misc.h"
#include "math.h"

volatile uint16_t lain_feloer_ADCConvertedValues[2];
void lain_feloer_init_ADC_Pin();
void lain_feloer_init_ADC();
int lain_feloer_kierunek(int c1,int c2);
void lain_feloer_set(int kierunek);
int lain_feloer_state();
void lain_feloer_init();




#endif // LAIN_FELOER_H_ lain_feloer
