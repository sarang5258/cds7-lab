 //Generate a sine wave of a tone of your choice and listen to the tone using headphone/speaker. 
//Waveform showing approx. 193Hz for note G of 195.998Hz. 


Code: 

#include "PLL.h" 
#include "tm4c123gh6pm.h" 
// Constant declarations to access port registers using 
// symbolic names instead of addresses 


void SysTick_Wait10ms(unsigned long delay); 
void DisableInterrupts(void); // Disable interrupts 
void EnableInterrupts(void); // Enable interrupts 
void WaitForInterrupt(void); // low power mode 
void PortA_Init(void); // start sound output 
void SysFun(void); 
void SysLoad(unsigned long period); 

unsigned int i=0; 

unsigned int i1=0; 

const unsigned char SineWave[64]={32,35,38,41,44,47,49,52,54,56,58, 
59,61,62,62,63,63,63,62,62,61,59, 
58,56,54,52,49,47,44,41,38,35, 
32,29,26,23,20,17,15,12,10,8, 
6,5,3,2,2,1,1,1,2,2,3, 
5,6,8,10,12,15,17,20,23,26,29}; 

// 3. Subroutines Section 
// MAIN: Mandatory for a C Program to be executable 
int main(void){ 

PLL_Init(); 

SysFun(); 
// 80 MHz 
PortA_Init(); 

// initialize output and interrupts 
EnableInterrupts(); 
/*Initialize ports and timers*/ 

while(1) 
{ 
i1=0; 
while(i1<64) 
{ 
GPIO_PORTA_DATA_R = (SineWave[i1]&0x3F)<<2; 
SysLoad(6377); //note G 
i1=i1+1;; 
} 
} 

} 

// **************Port_Init********************* 
// Initialize SysTick periodic interrupts 

// Input: none 
// Output: none 
void PortA_Init(void){ unsigned long volatile delay; 

SYSCTL_RCGC2_R |= 0x00000001; // activate port A 
delay = SYSCTL_RCGC2_R; 
GPIO_PORTA_AMSEL_R &= ~0xFF; // no analog 
GPIO_PORTA_PCTL_R &= ~0x00F00000; // regular function 
GPIO_PORTA_DIR_R |= 0xFF; // make PA5 out 
GPIO_PORTA_DR8R_R |= 0xFF; // can drive up to 8mA out 
GPIO_PORTA_AFSEL_R &= ~0xFF; // disable alt funct on PA5 
GPIO_PORTA_DEN_R |= 0xFF; // enable digital I/O on PA5 

} 

void SysLoad(unsigned long period){ 

NVIC_ST_RELOAD_R = period-1; // number of counts to wait 
NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears 
while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag 
} 

} 

void SysFun(void){ 

NVIC_ST_CTRL_R = 0; // disable SysTick during setup 
NVIC_ST_CTRL_R = 0x00000005;} 
