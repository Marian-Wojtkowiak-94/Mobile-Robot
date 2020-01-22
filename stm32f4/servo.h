#ifndef SERVO_H_   /* Include guard */
#define SERVO_H_



// drivery na timerze 3 i wyjsciu GPOIB czas powtarzania 20ms

#define period

#define swrvo_factor_multiplication_b0 500
#define swrvo_factor_multiplication_b1 500
#define swrvo_factor_multiplication_b4 850
#define swrvo_factor_multiplication_b5 500

#define swrvo_factor_add_b0 0
#define swrvo_factor_add_b1 0
#define swrvo_factor_add_b4 -9
#define swrvo_factor_add_b5 0


void servo_init();
float servo_check_kont(float kont);
void servo_set(int number,float kont);
void servo_set_b0(float kont);
void servo_set_b1(float kont);
void servo_set_b4(float kont); // przeciwnie do ruchuw wskazuweg zegara
void servo_set_b5(float kont);
void servo_init_NVIC(void);

#endif // SERVO_H_
