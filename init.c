#include "include.h"

//----------------------------------------------------------------------------/
void PortB_Init(void)
	{ 
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; // activate clock for port B
  while ((SYSCTL_RCGCGPIO_R & 0x02) == 0){}
  GPIO_PORTB_LOCK_R |= 0x4c4f434b;
  GPIO_PORTB_CR_R |= mask1;
  GPIO_PORTB_DIR_R &= !mask1;       // PB0 input
	//============================================================
		GPIO_PORTB_DIR_R |= 0x1E;
		GPIO_PORTB_DEN_R |= 0x1E;
  //============================================================
	GPIO_PORTB_PUR_R|= mask1;          // pull-up for PB0
  GPIO_PORTB_DEN_R |= mask1;         // enable digital I/O on PB0 
  GPIO_PORTB_AMSEL_R = 0x00;        // disable analog function
  GPIO_PORTB_AFSEL_R = 0x00;        //  no alternate function
 
  //--------------Interrupt Init--------------------//
  
  //PINS ENABLE
  GPIO_PORTB_IS_R &= ~mask1;
  GPIO_PORTB_IBE_R &= ~mask1;
  GPIO_PORTB_IEV_R &= ~mask1;
  GPIO_PORTB_ICR_R |= mask1;
  GPIO_PORTB_IM_R |= mask1;

  NVIC_PRI0_R = ((NVIC_PRI0_R & 0xffff00ff)| 0x00004000); //setting priority to 2
  NVIC_EN0_R = 0X00000002; //NVIC ENABLE

}
/*-----------------------------------------------------------*/
void PortF_Init(void){ 
	
  SYSCTL_RCGCGPIO_R |= 0x00000020; // activate clock for port F
	GPIO_PORTF_LOCK_R |= 0x4c4f434b; // Unlock port F
  GPIO_PORTF_CR_R |= 0x011;
  GPIO_PORTF_DIR_R |= 0x0E;  
  GPIO_PORTF_DEN_R |= 0x1F;     // enable digital I/O on PF3-PF1 
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  //GPIO_PORTF_DEN_R = 0x0E;          // 7) enable digital pins PF3-PF1        
}
/*-----------------------------------------------------------*/

void PortE_Init(void){  
volatile unsigned long delay;
//use device clock 16MGHZ 
SYSCTL_RCC_R &= ~(SYSCTL_RCC_USEPWMDIV);
//enable module 0
SYSCTL_RCGC0_R |= (1<<20); 
//enable port e 
SYSCTL_RCGC2_R |= (1<<4); 
//delay to be stable 
delay = SYSCTL_RCGC2_R;  
GPIO_PORTE_DR4R_R |=(1<<4);
//enable port e pin 4 
GPIO_PORTE_DEN_R |=(1<<4);
//set alternative function 
GPIO_PORTE_AFSEL_R |= (1<<4); 
GPIO_PORTE_PCTL_R |= 0x40000;  
//disable first 
PWM0_2_CTL_R &=~(1<<0);  
//down counter 
PWM0_2_CTL_R &=~(1<<1); 
// design behaviour
PWM0_2_GENA_R = (0x83); 
//set load value 
PWM0_2_LOAD_R = 64000 - 1; 
//set duty cycle initial to zero 
PWM0_2_CMPA_R = 32000 - 1;     
//enable pwm0
PWM0_2_CTL_R |= 0x1;
//enable generator 
PWM0_ENABLE_R |= PWM_ENABLE_PWM4EN;

}
/*-----------------------------------------------------------*/

void PWM_Init(void){
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_USEPWMDIV);
	SYSCTL_RCGC0_R |= (1<<20); 	//Enable the PWM clock
	SYSCTL_RCGC2_R |= 0x10;			//Enable the clock to the GPIO PORTE			
	while((SYSCTL_RCGC2_R & 0x10) == 0){}
	PWM0_2_CTL_R &= ~0x001;	//disable PWM module
	PWM0_2_CTL_R &= ~0x002;	//set count down mode
	PWM0_2_GENA_R |= 0x83;  //drive PWM low on cmpA and high on zero*/
	PWM0_2_LOAD_R = (32000-1);  //set load value to 31999
	PWM0_2_CMPA_R = (16000-1);   //set comparator A value to 15999
	PWM0_2_CTL_R |= 0x01;		 //enable pwm
	PWM0_ENABLE_R |= (0x00000030); //enable pwm values for PE4,PE5
}

/*-----------------------------------------------------------*/
void GPIO_PortE_Handler(void){
  GPIO_PORTE_ICR_R = 0x01;		//clear interrupt flag for PE0
  vTaskPrioritySet(xTask3Handle, 5); //set priority of task5 (train mode) to 5
}

/*-----------------------------------------------------------*/
void GPIOB_Handler (uint32_t pinMap)
{
	GPIO_DisarmInterrupt(&PINDEF(PORTB, (PinName_t)(PIN0)));
	GPIO_PORTF_DATA_R |= 0x02;
	GPIO_PORTF_DATA_R |= 0x04;
	vTaskDelay(100);
	GPIO_PORTF_DATA_R &= ~(0x02); //turn off LED
	GPIO_PORTF_DATA_R &= ~(0x04);
	GPIO_RearmInterrupt(&PINDEF(PORTF, (PinName_t)(PIN0 | PIN4)));

}
