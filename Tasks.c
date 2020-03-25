#include "include.h"

static void vTask1( void *pvParameters )
{
	for( ;; )
	{
		vTaskSuspend( xTask2Handle );
		vTaskPrioritySet(xTask2Handle, 3);
    GPIO_PORTF_DATA_R = 0x04;       // LED is Blue
		vTaskDelay( pdMS_TO_TICKS( 5000UL ));
		GPIO_PORTF_DATA_R = !0x04;
		vTaskResume(xTask2Handle);		

	}
}
/*-----------------------------------------------------------*/

static void vTask2( void *pvParameters )
{
	for( ;; )
	{
    vTaskSuspend( xTask1Handle );
		vTaskPrioritySet(NULL, 1);
    GPIO_PORTF_DATA_R = 0x02;       // LED is Red
		vTaskDelay( pdMS_TO_TICKS( 2500UL ));
		GPIO_PORTF_DATA_R = !0x02;
		vTaskResume(xTask1Handle);		

	}
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
