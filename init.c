#include "include.h"

//----------------------------------------------------------------------------/
void PortB_Init(void)
	{ 
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; // activate clock for port B
  while ((SYSCTL_RCGCGPIO_R & 0x02) == 0){}
  GPIO_PORTB_LOCK_R |= 0x4c4f434b;
  GPIO_PORTB_CR_R |= mask1;
  GPIO_PORTB_DIR_R &= !mask1;       // PF0 input
  GPIO_PORTB_PUR_R|= mask1;          // pull-up for PB0
  GPIO_PORTB_DEN_R |= mask1;         // enable digital I/O on PB0 
  GPIO_PORTB_AMSEL_R = 0x00;        // disable analog function
  GPIO_PORTB_AFSEL_R = 0x00;        //  no alternate function
  //--------------Interrupt Init--------------------//
  
  //PINS ENABLE
  GPIO_PORTB_IS_R &= !mask1;
  GPIO_PORTB_IBE_R &= !mask1;
  GPIO_PORTB_IEV_R &= !mask1;
  GPIO_PORTB_ICR_R |= mask1;
  GPIO_PORTB_IM_R |= mask1;

  NVIC_PRI0_R = ((NVIC_PRI0_R & 0xffff00ff)| 0x00004000); //setting priority to 2
  NVIC_EN0_R = 0X00000002; //NVIC ENABLE

}
/*-----------------------------------------------------------*/
void PortF_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x00000020; // activate clock for port F
  GPIO_PORTF_DIR_R |= 0x0E;  
  GPIO_PORTF_DEN_R |= 0x0E;     // enable digital I/O on PF3-PF1 
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_DEN_R = 0x0E;          // 7) enable digital pins PF3-PF1        
}
/*-----------------------------------------------------------*/

void PortE_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x00000010; // activate clock for port E
  GPIO_PORTE_DIR_R &= ~0x03;  		// make PE0,PE1 direction input
  GPIO_PORTE_DIR_R |= 0x30;  		// make PE4,PE5 direction output
  GPIO_PORTE_PUR_R |= 0x03;  		// enable pull up resistor for PE0,PE1
  GPIO_PORTE_DEN_R |= 0x33;     // enable digital I/O on PE0,PE1,PE4,PE5
  GPIO_PORTE_AFSEL_R |= 0x30;	//enable alternative fucntion for PE4,PE5
  GPIO_PORTE_DR4R_R |= 0x30;
  GPIO_PORTE_PCTL_R |= 0x440000;	//use M0PWM4, M0PWM5 for PE4,PE5 respectively
  GPIO_PORTE_IS_R &= ~0x03;       //interrupt statues = edge for PE0,PE1
  GPIO_PORTE_IBE_R &= ~0x03;		//single edge for PE0,PE1
  GPIO_PORTE_IEV_R &= ~0x03;		//falling edge for PE0,PE1
  GPIO_PORTE_ICR_R = 0x01;		//clear interrupt flag for PE0
  GPIO_PORTE_IM_R |= 0x03;		//unmask interrupts for PE0,PE1
  NVIC_PRI1_R &= ~0xE0;         //make train interrupt priority = 0 (highest)
  NVIC_EN0_R |= 0x10;			//enable interrupt register for PORTE
  __enable_irq();				//enable interrupts globally
  //GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  //GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  //GPIO_PORTE_AFSEL_R = 0x00;	        // 6) no alternate function
}
/*-----------------------------------------------------------*/

void PWM_Init(void){
	SYSCTL_RCGC0_R |= 0x100000; 	//Enable the PWM clock
	SYSCTL_RCGC2_R |= 0x10;			//Enable the clock to the GPIO PORTE			
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_USEPWMDIV);
	while((SYSCTL_RCGC2_R & 0x10) == 0){}
	PWM0_2_CTL_R &= ~0x001;	//disable PWM module
	PWM0_2_CTL_R &= ~0x002;	//set count down mode
	PWM0_0_GENA_R |= 0x83;  //drive PWM low on cmpA and high on zero
}

/*-----------------------------------------------------------*/
void GPIO_PortE_Handler(void){
  GPIO_PORTE_ICR_R = 0x01;		//clear interrupt flag for PE0
  vTaskPrioritySet(xTask3Handle, 5); //set priority of task5 (train mode) to 5
}

/*-----------------------------------------------------------*/
void GPIOB_Handler (void)
{
  GPIO_PORTB_ICR_R |= mask1;
  PD = true;
	GPIO_PORTF_DATA_R = !0x02;
	vTaskDelay(tcross);
}
