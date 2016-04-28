//Design a 4 input digital piano which will generate four different tones for four push button switches. 
//Waveform: 
//Key 1 corresponds to note B of 247Hz. 
//Waveform showing approx. 249 Hz on pressing Key1. 

//Key 2 corresponds to 
//note note A having 
//220.000Hz 

//Waveform for note A showing approx. 221.45 Hz 



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
void PortD_Init(void); 
unsigned int i=0; 


unsigned int i1=0; 
unsigned int i2=0; 
unsigned int in22=0; 
unsigned int val1,val2,val3,val4; 


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
PortD_Init(); 
// initialize output and interrupts 

EnableInterrupts(); 
/*Initialize ports and timers*/ 
i1=0; 

while(1) 


{ 

val1=GPIO_PORTD_DATA_R&0x0F; 

if(val1==0x01) 
{ 
i1=0; 
while(i1<64) 
{ 
GPIO_PORTA_DATA_R = (SineWave[i1]&0x3F)<<2; 
SysLoad(4879); //middle C 261hz 
i1=i1+1;; 
} 
} 

else if(val1==0x02) 
{ 
i1=0; 
while(i1<64) 
{ 
GPIO_PORTA_DATA_R =(SineWave[i1]&0x3F)<<2; 
SysLoad(5081); //note B 247hz 
i1=i1+1;; 


} 
} 


else if(val1==0x04) 
{ 
i1=0; 
while(i1<64) 
{ 
GPIO_PORTA_DATA_R = (SineWave[i1]&0x3F)<<2; 
SysLoad(5681); //note A 220.000hz 
i1=i1+1;; 
} 
} 


else if(val1==0x08) 
{ 
i1=0; 
while(i1<64) 
{ 
GPIO_PORTA_DATA_R = (SineWave[i1]&0x3F)<<2; 
SysLoad(6377); //note G 195.998hz 
i1=i1+1;; 
} 
} 


else 
//GPIO_PORTA_DATA_R = 0x00; 
{ 
i1=0; 
while(i1<64) 
{ 
GPIO_PORTA_DATA_R = SineWave[i1]&0x3F; 
SysLoad(200); //note G 
i1=i1+1;; 
} 
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

void PortD_Init(void){ unsigned long volatile delay; 
SYSCTL_RCGC2_R |= 0x08; // activate port C 
delay = SYSCTL_RCGC2_R; 
GPIO_PORTD_AMSEL_R &= ~0xFF;

 // GPIO_PORTC_PCTL_R &= ~0x00F00000; 
GPIO_PORTD_DIR_R &= ~0xFF; // make PC input 
GPIO_PORTD_AFSEL_R &= ~0xFF; // disable alt funct on PC 
GPIO_PORTD_DEN_R |= 0xFF; // enable digital I/O on PC 

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



