#include "Tasks.c"
#include "include.h"

int main(void){    
  PortF_Init();        // Call initialization of port PF3, PF2, PF1    
	xTaskCreate( vTask1, (const portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 2, &xTask1Handle );
	xTaskCreate( vTask2, (const portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, 1, &xTask2Handle );
		/* Start the scheduler. */
	vTaskStartScheduler();
}