#include "Tasks.c"
#include "include.h"
#include "PD_mode.c"
#include "PD_mode.h"

int main(void){    
  PortF_Init();        // Call initialization of port PF3, PF2, PF1
  PortB_Init();    	   // Call initialization of port PB0	
  xTaskCreate( vTask1, (const portCHAR *)"Task1", configMINIMAL_STACK_SIZE, (bool *)PD, 2, &xTask1Handle );
  xTaskCreate( vTask2, (const portCHAR *)"Task2", configMINIMAL_STACK_SIZE, (bool *)PD, 1, &xTask2Handle );
	PD = false;
	tcross = 10;
		/* Start the scheduler. */
	vTaskStartScheduler();
	
}
