// 0.Documentation Section 
// 
// Runs on LM4F120 or TM4C123
// Use the SysTick timer to request interrupts .
// Jonathan Valvano
// November 3, 2013


#include "PLL.h"
#include "tm4c123gh6pm.h"// Constant declarations to access port registers using 
// symbolic names instead of addresses 
 
void DisableInterrupts(void); // Disable interrupts 
void EnableInterrupts(void); // Enable interrupts 
void WaitForInterrupt(void); // low power mode 
void PortA_Init(void); // start sound output 
void SysInit(void); 
void SysLoad(unsigned long period); 
  unsigned int i=0; 
  unsigned int i1=0; 
  //unsigned int i2=0; 
// MAIN: Mandatory for a C Program to be executable 
int main(void){
PLL_Init(); 
SysInit(); // 80 MHz
PortA_Init(); // initialize output and interrupts 
EnableInterrupts(); 
/*Initialize ports and timers*/ 
while(1){ 
//go to 90 deg 
i1=20; 
while(i1) 
{ 
//GPIO_PORTA_DATA_R |= (0x20); 
//SysLoad(40000); 
//GPIO_PORTA_DATA_R &= ~(0x20); 
//SysLoad(1600000); 


//i1=i1-1;; 
//} 
//back to 0deg 
//i1=20; 
//while(i1) 
//{ 

GPIO_PORTA_DATA_R |= (0x20); 
SysLoad(160000); 
GPIO_PORTA_DATA_R &= ~(0x20); 
SysLoad(1440000); 

i1=i1-1;
} 

//go to -90deg 
i1=20; 
while(i1) 
{ 
GPIO_PORTA_DATA_R |= (0x20); 
SysLoad(80000); 
GPIO_PORTA_DATA_R &= ~(0x20); 
SysLoad(1520000); 


i1=i1-1; 
} 


//back to 0deg 
//i1=20; 
//while(i1) 
//{ 
//GPIO_PORTA_DATA_R |= (0x20); 
//SysLoad(120000); 
//GPIO_PORTA_DATA_R &= ~(0x20); 
//SysLoad(1480000); 


//i1=i1-1;; 
//} 

} 

} 
// **************Port_Init********************* 
// Initialize SysTick periodic interrupts 

// Input: none 
// Output: none 
void PortA_Init(void){ unsigned long volatile delay;

 SYSCTL_RCGC2_R |= 0x00000001; // activate port A 
delay = SYSCTL_RCGC2_R; 
GPIO_PORTA_AMSEL_R &= ~0x20; // no analog 
GPIO_PORTA_PCTL_R &= ~0x00F00000; // regular function 
GPIO_PORTA_DIR_R |= 0x20; // make PA5 out 
GPIO_PORTA_DR8R_R |= 0x20; // can drive up to 8mA out 
GPIO_PORTA_AFSEL_R &= ~0x20; // disable alt funct on PA5 
GPIO_PORTA_DEN_R |= 0x20; // enable digital I/O on PA5 


} 

void SysLoad(unsigned long period){ 

NVIC_ST_RELOAD_R = period-1; // number of counts to wait 
NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears 
while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag 
} 

} 

void SysInit(void){ 

NVIC_ST_CTRL_R = 0; // disable SysTick during setup 
NVIC_ST_CTRL_R = 0x00000007; 


} 


void SysTick_Handler(void){
	
	GPIO_PORTA_DATA_R^=0x20;
/*Your code goes here*/
/*Your code goes here*/
	
			
	}



