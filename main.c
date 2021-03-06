#include "Tasks.c"
#include "include.h"

int main(void){   
  PortE_Init();    	   // Call initialization of port PE4	
  PortF_Init();        // Call initialization of port PF3, PF2, PF1, PF0
  PortB_Init();    	   // Call initialization of port PB0, PB1, PB2 ,PB3, PB4
  
inter = 0;
tcross = (10000/portTICK_RATE_MS);
	
	
	xTaskCreate( vTask1, (const portCHAR *)"North", configMINIMAL_STACK_SIZE, NULL, 5, &xTask1Handle );
  xTaskCreate( vTask2, (const portCHAR *)"East", configMINIMAL_STACK_SIZE, NULL, 3, &xTask2Handle );
	xTaskCreate( vTask3, (const portCHAR *)"Train", configMINIMAL_STACK_SIZE, NULL , 0 , &xTask3Handle );
	xTaskCreate( vTask4, (const portCHAR *)"Dummy", configMINIMAL_STACK_SIZE, NULL , 4 , &xTask4Handle );
		/* Start the scheduler. */
	vTaskStartScheduler();
	
}
