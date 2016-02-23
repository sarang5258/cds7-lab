#include <stdint.h>
#include "tm4c123gh6pm.h"
void PortD_Init(void);
void Delay(unsigned long time);
void PortF_Init(void);
unsigned long IN1,IN2;  // input from PF4,PF0
int main(void)
{
	PortD_Init();
	PortF_Init();
	
while(1)
{
IN1 = GPIO_PORTF_DATA_R&0x01;// read PF0 into IN1
IN2 = GPIO_PORTF_DATA_R&0x10;// read PF4 into IN2

if(IN1&&IN2) // both not pressed
GPIO_PORTD_DATA_R = 0x00;// LEDs are OFF
else if((!IN1)&&IN2)// PF0 IS PRESSED
GPIO_PORTD_DATA_R = 0x01;// LED ON PD0
else if(IN1&&(!IN2))//PF4 IS PRESSED
GPIO_PORTD_DATA_R = 0x08;// LED ON PD3
else if((!IN1) && (!IN2))// BOTH ARE PRESSED
GPIO_PORTD_DATA_R = 0x09;
else
GPIO_PORTD_DATA_R=0x00;// LED is off
}
}

void PortD_Init(void)
{
volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x08; // Port D clock
delay = SYSCTL_RCGC2_R; // wait 3-5 bus cycles
GPIO_PORTD_DIR_R |= 0x09; // PD3 PD0 output
GPIO_PORTD_AFSEL_R &= ~0x09; // not alternative
GPIO_PORTD_AMSEL_R &= ~0x09; // no analog
GPIO_PORTD_PCTL_R &= ~0xF00F; // bits for PD3 PD0
GPIO_PORTD_DEN_R |= 0x09; // enable PD3, PD0
}

void PortF_Init(void)
{
volatile unsigned long delay;
SYSCTL_RCGC2_R |= 0x20;// clock for Port F
delay = SYSCTL_RCGC2_R;// wait 3-5 bus cycles 	
GPIO_PORTF_LOCK_R = 0x4C4F434B;//unlock GPIOPortF
GPIO_PORTF_CR_R = 0x1F; // allow changes to PF4-0
// only PF0 to be unlocked, other bits can't be
GPIO_PORTF_AMSEL_R = 0x00;// disable analog
GPIO_PORTF_PCTL_R = 0x00;// bits for PF4-0
GPIO_PORTF_DIR_R = 0x0E;// 5) PF4,0 in, PF3,1 out
GPIO_PORTF_AFSEL_R = 0x00;//disable alt func
GPIO_PORTF_PUR_R = 0x11;// enable pullup on PF0,4 
GPIO_PORTF_DEN_R = 0x1F;// 7) enable digital I/O
}
