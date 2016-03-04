//Interface pins 1A,2A to Port A, and control the direction of the motor along with speed control.

#include "PLL.h"
#include "tm4c123gh6pm.h"
// Constant declarations to access port registers using 
// symbolic names instead of addresses
unsigned long ADC0_InSeq3(void);
void ADC0_InitSWTriggerSeq3_Ch1(void);
void SysTick_Wait10ms(unsigned long delay);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode
void PortA_Init(void);   // start sound output
void PortF_Init(void);    //using for direction
void SysFun(void);
void SysLoad(unsigned long period);

unsigned int ADCvalue;
unsigned long a;
unsigned int in1;
unsigned long t=20000;
unsigned long t1;

	
// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
  PLL_Init();  // 80 MHz 
	PortA_Init();	
	PortF_Init();
	ADC0_InitSWTriggerSeq3_Ch1();
	EnableInterrupts();
	SysFun();
	
	/*Initialize ports , ADC and timers*/
  while(1){   
		/*Your code goes here*/
		a= ADC0_InSeq3();
		t1=a*(10000/4096)+10000;
		if ((GPIO_PORTF_DATA_R&0x01)==0) //move right make PA1 high and PA2 low 
{ 
GPIO_PORTA_DATA_R |=0x01; //PA1 high
GPIO_PORTA_DATA_R &=~(0x02);//PA2 low 
} 

else //move left make PA2 high and PA1 low
{ 
GPIO_PORTA_DATA_R |=0x02; //PA2 high
GPIO_PORTA_DATA_R &=~(0x01);//PA1 low 
} 
		in1=50;
		while(in1)
			{ 
GPIO_PORTA_DATA_R |= (0x20); 
SysLoad(t1); 
GPIO_PORTA_DATA_R &= ~(0x20); 
SysLoad(t-t1); 
in1=in1-1; 

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
  GPIO_PORTA_AMSEL_R &= ~0x23;      // no analog 
  GPIO_PORTA_PCTL_R &= ~0x00F00000; // regular function
  GPIO_PORTA_DIR_R |= 0x23;     // make PA5 PA0 PA1 out
  GPIO_PORTA_DR8R_R |= 0x23;    // can drive up to 8mA out
  GPIO_PORTA_AFSEL_R &= ~0x23;  // disable alt funct on PA5 PA1 PA0
  GPIO_PORTA_DEN_R |= 0x23;     // enable digital I/O on PA5 PA0 PA1
}
void PortF_Init(void){ volatile unsigned long delay; 
  SYSCTL_RCGC2_R |= 0x00000020; // 1) F clock 
  delay = SYSCTL_RCGC2_R; // delay 
  GPIO_PORTF_LOCK_R = 0x4C4F434B; // 2) unlock PortF PF0 
  GPIO_PORTF_CR_R = 0x1F; // allow changes to PF4-0 
  GPIO_PORTF_AMSEL_R = 0x00; // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000; // 4) GPIO clear bit PCTL
  GPIO_PORTF_DIR_R = 0x0E; // 5) PF4,PF0 input, PF3,PF2,PF1 output
  GPIO_PORTF_AFSEL_R = 0x00; // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11; // enable pullup resistors on PF4,PF0
  GPIO_PORTF_DEN_R = 0x1F; // 7) enable digital pins PF4-PF0 
} 

void SysLoad(unsigned long period){

	
	NVIC_ST_RELOAD_R = period-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
	
}

void SysFun(void){
	
	NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;
          
} 

//ADC initialisation on PE2

void ADC0_InitSWTriggerSeq3_Ch1(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;   // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;         //    allow time for clock to stabilize
  GPIO_PORTE_DIR_R &= ~0x04;      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0x04;     // 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // 5) enable analog function on PE2
  SYSCTL_RCGC0_R |= 0x00010000;   // 6) activate ADC0 
  delay = SYSCTL_RCGC2_R;         
  SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1; // 11) channel Ain1 (PE2)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_ACTSS_R |= 0x0008;         // 13) enable sample sequencer 3
}






//------------ADC0_InSeq3------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned long ADC0_InSeq3(void){  
	unsigned long result;
	
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}



