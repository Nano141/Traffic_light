#include "include.h"

//----------------------------------------------------------------------------/
void PortB_Init(void)
	{ 
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; // activate clock for port B
  while ((SYSCTL_RCGCGPIO_R & 0x02) == 0){}
  GPIO_PORTB_LOCK_R |= 0x4c4f434b;
  GPIO_PORTB_CR_R |= 0x01;
  GPIO_PORTB_DIR_R &= ~0x61;       // PB0,PB5,PB6 input
	//============================================================
		GPIO_PORTB_DIR_R |= 0x1E;
		GPIO_PORTB_DEN_R |= 0x1E;
  //============================================================
	GPIO_PORTB_PUR_R|= 0x61;          // pull-up for PB0,PB5,PB6
  GPIO_PORTB_DEN_R |= 0x61;         // enable digital I/O on PB0,PB5,PB6
  GPIO_PORTB_AMSEL_R = 0x00;        // disable analog function
  GPIO_PORTB_AFSEL_R = 0x00;        //  no alternate function
 
}
/*-----------------------------------------------------------*/
void PortF_Init(void){ 
	
  SYSCTL_RCGCGPIO_R |= 0x00000020; // activate clock for port F
	GPIO_PORTF_LOCK_R |= 0x4c4f434b; // Unlock port F
  GPIO_PORTF_CR_R |= 0x011;
	GPIO_PORTF_DIR_R &= !0x10; 
  GPIO_PORTF_DIR_R |= 0x0E;  
  GPIO_PORTF_DEN_R |= 0x1F;     // enable digital I/O on PF3-PF1 
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
	GPIO_PORTF_PUR_R|= 0x10;      
}
/*-----------------------------------------------------------*/

void PortE_Init(void){  
volatile unsigned long delay;
SYSCTL_RCC_R &= ~(SYSCTL_RCC_USEPWMDIV); //use device clock 16MHZ 
SYSCTL_RCGC0_R |= (1<<20); //enable pwm module 0
SYSCTL_RCGC2_R |= (1<<4);	 //enable port E 
delay = SYSCTL_RCGC2_R;    //delay to stabilize 
GPIO_PORTE_DR4R_R |=(1<<4); 
GPIO_PORTE_DEN_R |=(1<<4); //enable port E pin 4
GPIO_PORTE_AFSEL_R |= (1<<4); //set alternative function 
GPIO_PORTE_PCTL_R |= 0x40000; //port control to choose pwm
PWM0_2_CTL_R &=~(1<<0);				//disable pwm  
PWM0_2_CTL_R &=~(1<<1); 			//down counter 
PWM0_2_GENA_R = (0x83); 				
}


/*-----------------------------------------------------------*/
