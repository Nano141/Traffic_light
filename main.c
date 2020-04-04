#include "Tasks.c"
#include "include.h"
#include "PD_mode.c"
#include "PD_mode.h"

int main(void){    
  PortF_Init();        // Call initialization of port PF3, PF2, PF1
  PortB_Init();    	   // Call initialization of port PB0	
		PD = false;
  xTaskCreate( vTask1, (const portCHAR *)"North", configMINIMAL_STACK_SIZE, (bool *)PD, 2, &xTask1Handle );
  xTaskCreate( vTask2, (const portCHAR *)"East", configMINIMAL_STACK_SIZE, (bool *)PD, 1, &xTask2Handle );
	xTaskCreate( vTask3, (const portCHAR *)"Dummy", configMINIMAL_STACK_SIZE, NULL , 2 , &xTask3Handle );
	tcross = (10000/portTICK_RATE_MS);
		/* Start the scheduler. */
	vTaskStartScheduler();
	
}
