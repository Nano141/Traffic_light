#include "PD_mode.h"

//----------------------------------------------------------------------------/
void PortB_Init(void)
	{ 
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; // activate clock for port B
  while ((SYSCTL_RCGCGPIO_R & 0x02) == 0){}
  GPIO_PORTB_LOCK_R |= 0x4c4f434b;
  GPIO_PORTB_CR_R |= mask1;
  GPIO_PORTB_DIR_R &= ~mask1;       // PF0 input
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
  __enable_irq(); //ENABLE INTERRUPTS GLOBALLY

}

void GPIOPortB_Handler (void)
{
  GPIO_PORTB_ICR_R = mask1;
  PD = true;
}
