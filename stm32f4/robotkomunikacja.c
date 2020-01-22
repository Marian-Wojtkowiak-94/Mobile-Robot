#include "robotkomunikacja.h"


#define CS_LOW GPIOA->BSRRH = GPIO_BSRR_BS_15;
#define CS_HIGH GPIOA->BSRRL = GPIO_BSRR_BS_15;


#include <stm32f4xx.h>
#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include <stm32f4xx_usart.h> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src



void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

/* This funcion initializes the USART1 peripheral
 *
 * Arguments: baudrate --> the baudrate at which the USART is
 * 						   supposed to operate
 */
void init_USART1(uint32_t baudrate){

	/* This is a concept that has to do with the libraries provided by ST
	 * to make development easier the have made up something similar to
	 * classes, called TypeDefs, which actually just define the common
	 * parameters that every peripheral needs to work correctly
	 *
	 * They make our life easier because we don't have to mess around with
	 * the low level stuff of setting bits in the correct registers
	 */
	GPIO_InitTypeDef GPIO_InitStruct; // this is for the GPIO pins used as TX and RX
	USART_InitTypeDef USART_InitStruct; // this is for the USART1 initilization
	NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)

	/* enable APB2 peripheral clock for USART1
	 * note that only USART1 and USART6 are connected to APB2
	 * the other USARTs are connected to APB1
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* enable the peripheral clock for the pins used by
	 * USART1, PB6 for TX and PB7 for RX
	 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* This sequence sets up the TX and RX pins
	 * so they work correctly with the USART1 peripheral
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Pins 6 (TX) and 7 (RX) are used
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			// this defines the output type as push pull mode (as opposed to open drain)
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
	GPIO_Init(GPIOB, &GPIO_InitStruct);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers

	/* The RX and TX pins are now connected to their AF
	 * so that the USART1 can take over control of the
	 * pins
	 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); //
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	/* Now the USART_InitStruct is used to define the
	 * properties of USART1
	 */
	USART_InitStruct.USART_BaudRate = baudrate;				// the baudrate is set to the value we passed into this init function
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
	USART_InitStruct.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
	USART_Init(USART1, &USART_InitStruct);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting


	/* Here the USART1 receive interrupt is enabled
	 * and the interrupt controller is configured
	 * to jump to the USART1_IRQHandler() function
	 * if the USART1 receive interrupt occurs
	 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		 // we want to configure the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART1 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	// finally this enables the complete USART1 peripheral
	USART_Cmd(USART1, ENABLE);
}

/* This function is used to transmit a string of characters via
 * the USART specified in USARTx.
 *
 * It takes two arguments: USARTx --> can be any of the USARTs e.g. USART1, USART2 etc.
 * 						   (volatile) char *s is the string you want to send
 *
 * Note: The string has to be passed to the function as a pointer because
 * 		 the compiler doesn't know the 'string' data type. In standard
 * 		 C a string is just an array of characters
 *
 * Note 2: At the moment it takes a volatile char because the received_string variable
 * 		   declared as volatile char --> otherwise the compiler will spit out warnings
 * */
void USART_puts(USART_TypeDef* USARTx, volatile char *s){

	while(*s){
		// wait until data register is empty
		while( !(USARTx->SR & 0x00000040) );
		USART_SendData(USARTx, *s);
		*s++;
	}
}

//int main(void) {
//
//
//init_USART1(9600); // initialize USART1 @ 9600 baud
//  USART_puts(USART1, "Init complete! Hello World!\r\n"); // just send a message to indicate that it works
//
//  while (1){
//    /*
//     * You can do whatever you want in here
//     */
//  }
//}

// this is the interrupt request handler (IRQ) for ALL USART1 interrupts
void USART1_IRQHandler(void){

	// check if the USART1 receive interrupt flag was set
	if( USART_GetITStatus(USART1, USART_IT_RXNE) ){

		static uint8_t cnt = 0; // this counter is used to determine the string length
		char t = USART1->DR; // the character from the USART1 data register is saved in t

		/* check if the received character is not the LF character (used to determine end of string)
		 * or the if the maximum string length has been been reached
		 */
		if( (t != '\n') && (cnt < MAX_STRLEN) ){
			received_string[cnt] = t;
			cnt++;
		}
		else{ // otherwise reset the character counter and print the received string
			if(t=='\n'){
				received_string[cnt]=0;
			}
			cnt = 0;
			//USART_puts(USART1, received_string);
		}
	}
}



void spiRasInit(){
	spiRasInitGPIO();
	spiRasInitSPI();
	spiRasInitGPIOState();
}


void spiRasInitSPI(){
	SPI_InitTypeDef SPI_InitStruct;
	// enable peripheral clock
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
//    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex
//    SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;     // transmit in slave mode, NSS pin has
//    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
//    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;        // clock is low when idle
//    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
//    SPI_InitStruct.SPI_NSS = SPI_NSS_Hard; // set the NSS HARD
//    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // SPI frequency is
//    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
//	  SPI_Init(SPI3, &SPI_InitStruct);


//	  SPI_InitTypeDef  SPI_InitStructure;
//	    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//	    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
//	    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//	    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//	    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//	    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//	    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
//	    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//	    SPI_InitStructure.SPI_CRCPolynomial = 7;
//
//
//	    SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_RXNE, ENABLE);
//	    /* Enable the Tx empty interrupt */
//	    SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, ENABLE);
//
//	    /* Enable the SPI peripheral */
//
//	    SPI_Cmd(SPI3, ENABLE);
//	  //  SPI3_ReadWriteByte(0xff);

//    SPI_InitTypeDef SPI_InitStructure;
//    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
//    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//    SPI_InitStructure.SPI_CRCPolynomial = 7;
//    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//    SPI_Init(SPI3, &SPI_InitStructure);


	SPI_InitTypeDef spi;

	//ENABLE THE CLOCK TO SPI1 PERIPHERAL
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	//SET SPI SPEED TO LOW SPEED MODE (~282KHZ)
//	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
//	spi.SPI_CPHA = SPI_CPHA_2Edge;
//	spi.SPI_CPOL = SPI_CPOL_High;
//	spi.SPI_DataSize = SPI_DataSize_8b;
//	spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//	spi.SPI_FirstBit = SPI_FirstBit_MSB;
//	spi.SPI_Mode = SPI_Mode_Slave;
//	spi.SPI_NSS = SPI_NSS_Soft;
//	spi.SPI_CRCPolynomial=7;


	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	SPI_Init(SPI3, &SPI_InitStructure);
	//SPI_Init(SPI3, &spi);
	//SPI_Cmd(SPI3, ENABLE);


    SPI_Cmd(SPI3, ENABLE); // enable SPI3

}


void RasInitGPIO(){
	GPIO_InitTypeDef GPIO_InitStruct;
    // enable clock for used IO pins
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /* configure pins used by SPI3
     * PC10 = SCK
     * PC11 = MISO
     * PC12 = MOSI
     */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // connect SPI3 pins to SPI alternate function
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; // cs
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource15,GPIO_AF_SPI3);


}

void RasInitGPIOState(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // cs
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);



}

int RasButonEneble(){
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0){
		return 1;
	}
	return 0;
}

//SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256

void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    SPI3->CR1&=0XFFC7;
    SPI3->CR1|=SPI_BaudRatePrescaler;
    SPI_Cmd(SPI3,ENABLE);
}

u8 SPI3_ReadWriteByte(u8 TxData)
{

	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI3);
  //while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET){}

    //return SPI_I2S_ReceiveData(SPI3);
}

void RasOdbiur(){
	char* a[25];
	//int forth, right;
	int i = 0;
	char kopia[MAX_STRLEN+1];
	strcpy(kopia, received_string);

	a[i] = strtok(kopia," ");

	while(a[i]!=NULL)
	{
	   a[++i] = strtok(NULL," ");
	}
	//strcmp(lhs, rhs)

	//parsowania danych z rasberry
	for( i=0;i<25;i++){
		if(a[i]==NULL)
			break;
		if(strcmp(a[i],"k")==0)
			break;
		if((strcmp(a[i],"jazda")==0)){
			int forth= atoi(a[i+1]);
			int right = atoi(a[i+2]);
			if((abs(forth)<=100)&&(abs(right)<=100)){
			driver_set_rote((double)forth/100,(double)right/100);
			i=i+2;
			}
		}
		if((strcmp(a[i],"servo")==0)){
			int id= atoi(a[i+1]);
			float kont = atof(a[i+2]);
			servo_set(id,kont);
			i=i+2;
		}
	}
}

void RasNadawaj(){
	float kont;
	uint64_t odleglosci;
	TM_LIS302DL_LIS3DSH_t osie;
	char send[100];
	odleglosci=HC_SR04_length_mm();
	TM_LIS302DL_LIS3DSH_ReadAxes(&osie);
	kont=Robot_kont(osie);
	// odleglosci	lain felower 1	lain felower 2	x y z

	sprintf(send,"%04d\t%04d\t%04d\t%04d\t%04d\t%04d\t%04d\t%04d\n",odleglosci,lain_feloer_ADCConvertedValues[0],lain_feloer_ADCConvertedValues[1],osie.X,osie.Y,osie.Z,(int)kont);
	USART_puts(USART1, send);
}

int RasState(){
	char zgoda=1;

	//char receive[38];


	//LCD_printl(1,"wysylanie");
	LCD_printl(2,"");



	//USART_puts(USART1, "Init complete! Hello World!\r\n"); // just send a message to indicate that it works
	while(zgoda)
	{

		LCD_printl(1,"Rasbery");
		//SPI3_ReadWriteByte('a');
//		spiRasComunicat(SPI3,send,receive,38);

		//LCD_printl(2,"%s",receive);
		LCD_printl(2,"%s",received_string);
		//a[0]= strtok(received_string,' ');

		//USART_puts(UART1,"dostal");
		RasOdbiur();
		RasNadawaj();
		//LCD_printl(2,"%04d %04d", lain_feloer_ADCConvertedValues[0], lain_feloer_ADCConvertedValues[1]);
		delay(1000000);
		char przycisk=interface_read_butons_if();
		if(przycisk==12&&RasButonEneble()==0){
			zgoda=0;
			driver_set_rote(0,0);
		}


	}
	LCD_printl(1,"koniec wysylania");
	return zgoda;


}

void spiRasComunicat(SPI_TypeDef* SPIx,char *send,char *receive,int N){

	for (int i=0;i<N;i++){

		SPI_I2S_SendData(SPI3, send[i]);
		while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
		receive[i] = SPI_I2S_ReceiveData(SPI3);
	}

}

void SPI3_IRQHandler(void)
{
    unsigned char rx;
    static unsigned short int count = 0, i = 0 ;


    if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_TXE) == SET)
    {

        if(count < 10)
        {
                SPI3->DR  = count;
                count++ ;
        }
        else
        {
                SPI_Cmd(SPI3, DISABLE);
        }

    }
    if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET)
    {
        //master_rev_buff[i] = SPI3->DR ;
        i++ ;

    }
}

