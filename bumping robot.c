//bumping motor code


#include <stdint.h>
#include "tm4c123gh6pm.h"
void PortD_Init(void);
void PortB_Init(void);
unsigned long delay, IN1,IN2;// input from PD1,PD0

int main(void)
{
	PortD_Init();
	PortB_Init();
	
	while(1)
	{
		IN1 = GPIO_PORTD_DATA_R&0x01;// read PD0 into IN1
		IN2 = GPIO_PORTD_DATA_R&0x02;// read PD1 into IN2

		if((!IN1)&&(!IN2)) // both not pressed
			GPIO_PORTB_DATA_R = 0x05;// motor rotate in same direction,forward
		else if((IN1)&&(!IN2))// PD0 IS PRESSED left bump
			GPIO_PORTB_DATA_R = 0x09;//  ON PD0,motor take right turn
		else if((!IN1)&&(IN2))//PD1 IS PRESSED right bump
			GPIO_PORTB_DATA_R = 0x06;//  ON PD3,motor take left turn
		else if((IN1) && (IN2))// BOTH ARE PRESSED
			GPIO_PORTB_DATA_R = 0x00;
		else
			GPIO_PORTD_DATA_R=0x05;// motor is off
	}
}

void PortB_Init(void)
{
	SYSCTL_RCGC2_R |= 0x02; // Port B clock
	delay = SYSCTL_RCGC2_R; // wait 3-5 bus cycles
	GPIO_PORTB_DIR_R |= 0x0F; // PB3 PB2 PB1 PB0 output
	GPIO_PORTB_AFSEL_R &= (~0x0F); // not alternative
	GPIO_PORTB_AMSEL_R &= (~0x0F); // no analog
	GPIO_PORTB_PCTL_R &= (~0xFFFF);//bits for PB3 PB0
	GPIO_PORTB_DEN_R |= 0x0F; // enable PB3 PB2 PB1 PB0
}

void PortD_Init(void)
{
	SYSCTL_RCGC2_R |= 0x08; // Port D clock
	delay = SYSCTL_RCGC2_R; // wait 3-5 bus cycles
	GPIO_PORTD_DIR_R |= 0x00; // PD3 PD2 PD1 PD0 output
	GPIO_PORTD_AFSEL_R &= (~0x0F); // not alternative
	GPIO_PORTD_AMSEL_R &= (~0x0F); // no analog
	GPIO_PORTD_PCTL_R &= (~0xFFFF);//bits for PD3 PD0
	GPIO_PORTD_DEN_R |= 0x0F; // enable PD3 PD2 PD1 PD0
}

