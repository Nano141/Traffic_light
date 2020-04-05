#include "Tasks.c"
#include "include.h"
#include "HAL.h"

int main(void){   
//  PortE_Init();    	   // Call initialization of port E	
  PortF_Init();        // Call initialization of port PF3, PF2, PF1
  PortB_Init();    	   // Call initialization of port PB0	
	//PWM_Init();					 // Call initialization of PWM
  //interupt_init();
	
	//PD = false;
  
	tcross = (10000/portTICK_RATE_MS);
	
	/*__disable_irq();
	
	PLL_Init80MHz();
	// Must store the frequency in SystemCoreClock for FreeRTOS to use.
	SystemCoreClock = PLL_GetBusClockFreq();


	// These are the digital intputs for the onboard buttons.
	//GPIO_EnableDI(PORTF, PIN0 | PIN4, PULL_UP);
	
	// These are the digital intputs for the PD push button.
	//GPIO_EnableDI(PORTB, PIN0 , PULL_UP);
	
	// Enable interrupts for SW1 & SW2.
	//GPIO_EnableInterrupt(&PINDEF(PORTF, PIN0), 4, INT_TRIGGER_FALLING_EDGE, GPIOF_Interrupt);
	//GPIO_EnableInterrupt(&PINDEF(PORTF, PIN4), 4, INT_TRIGGER_FALLING_EDGE, GPIOF_Interrupt);
	
	// Enable interrupts for PD push button..
	//GPIO_EnableInterrupt(&PINDEF(PORTB, PIN0), 4, INT_TRIGGER_FALLING_EDGE, GPIOB_Handler);

	__enable_irq();*/
	
	xTaskCreate( vTask1, (const portCHAR *)"North", configMINIMAL_STACK_SIZE, (bool *)PD, 2, &xTask1Handle );
  xTaskCreate( vTask2, (const portCHAR *)"East", configMINIMAL_STACK_SIZE, (bool *)PD, 1, &xTask2Handle );
	xTaskCreate( vTask3, (const portCHAR *)"Train", configMINIMAL_STACK_SIZE, NULL , 0 , &xTask3Handle );
	xTaskCreate( vTask4, (const portCHAR *)"Dummy", configMINIMAL_STACK_SIZE, NULL , 2 , &xTask4Handle );
		/* Start the scheduler. */
	vTaskStartScheduler();
	
}
