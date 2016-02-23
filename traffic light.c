Design a traffic light controller for the given situation 


#include "PLL.h" 
#include "tm4c123gh6pm.h" 
// Constant declarations to access port registers using 
// symbolic names instead of addresses 


void SysTick_Wait10ms(unsigned long delay); 

void DisableInterrupts(void); // Disable interrupts 

void EnableInterrupts(void); // Enable interrupts 

void WaitForInterrupt(void); // low power mode 

void Port_Init(void); // start sound output 

void SysFun(void); 

void SysLoad(unsigned long period); 

unsigned int i=0; 

unsigned int i1=0; 

unsigned int i2=0; 

// 3. Subroutines Section 

// MAIN: Mandatory for a C Program to be executable 

int main(void){ 
PLL_Init(); 
SysFun(); 
// 80 MHz 
Port_Init(); 
// initialize output and interrupts 
EnableInterrupts(); 
/*Initialize ports and timers*/ 
/*PA 2,3,4 CONNECTED TO WEST RED YELLOW AND GREEN RES. AND PA 5,6,7 TO SOUTH 
SIMILARLY. 
PF1 AND PF3 ARE ON BOARD RED AND GREEN*/ 
while(1){ 
GPIO_PORTA_DATA_R = (0x84); //west is stopped 
GPIO_PORTF_DATA_R = (0x10); //people can walk 
SysTick_Wait10ms(20); 
GPIO_PORTA_DATA_R = (0x48); //both yellow 
GPIO_PORTF_DATA_R = (0x02); //people should stop 
SysTick_Wait10ms(100); 
GPIO_PORTA_DATA_R = (0x30); //west is going and south stop 
SysTick_Wait10ms(300);
 } 
} 

} 
// **************Port_Init********************* 
// Initialize SysTick periodic interrupts 
// Input: none 
// Output: none 
void Port_Init(void){ unsigned long volatile delay;
 SYSCTL_RCGC2_R |= 0x00000001; // activate port A 
delay = SYSCTL_RCGC2_R; 
GPIO_PORTA_AMSEL_R &= ~0x20; // no analog 
GPIO_PORTA_PCTL_R &= ~0x00F00000; // regular function 
GPIO_PORTA_DIR_R |= 0x20; // make PA5 out 
GPIO_PORTA_DR8R_R |= 0x20; // can drive up to 8mA out 
GPIO_PORTA_AFSEL_R &= ~0x20; // disable alt funct on PA5 
GPIO_PORTA_DEN_R |= 0x20; // enable digital I/O on PA5 
} void SysLoad(unsigned long period){ 
NVIC_ST_RELOAD_R = period-1; // number of counts to wait 
NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears 
while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag 
} 
}
void SysFun(void){ 
NVIC_ST_CTRL_R = 0; // disable SysTick during setup 
NVIC_ST_CTRL_R = 0x00000005; 
} 
void SysTick_Wait10ms(unsigned long delay){ 
unsigned long i; 
for(i=0; i<=delay; i++){
 SysLoad(2); // wait 10ms 
} 
}

