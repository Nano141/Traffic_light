#include "init.c"

static void vTask1( void *pvParameters)
{
	bool *pd;
	pd = (bool *) pvParameters;
	xLastWakeTime1 = xTaskGetTickCount();
	 const TickType_t nsDelayTime = (5000/portTICK_RATE_MS);
	for( ;; )
	{
    GPIO_PORTB_DATA_R |= 0x012;  //NS Green EW Red
		GPIO_PORTF_DATA_R |= 0x04;
		vTaskDelayUntil(&xLastWakeTime1,nsDelayTime);
		if (pd)
		{
			GPIO_PORTF_DATA_R = !0x04;
			GPIO_PORTF_DATA_R = !0x02;
			xLastWakeTime1 = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime1,tcross);
			pd=false;
		}
		xLastWakeTime2 = xTaskGetTickCount();
		GPIO_PORTB_DATA_R &= ~0x012;
		GPIO_PORTF_DATA_R &= ~0x04;
		vTaskPrioritySet(xTask2Handle, 3);

	}
}
/*-----------------------------------------------------------*/

static void vTask2( void *pvParameters )
{
	bool *pd;
	pd = (bool *) pvParameters;
	xLastWakeTime2 = xTaskGetTickCount();
	 const TickType_t ewDelayTime = (2500/portTICK_RATE_MS);
	for( ;; )
	{
    GPIO_PORTB_DATA_R |= 0x0C;       // NS is Red EW Green
		GPIO_PORTF_DATA_R |= 0x02;
		vTaskDelayUntil(&xLastWakeTime2,ewDelayTime);
		if (pd)
		{
			//GPIO_PORTF_DATA_R = !0x04;
			GPIO_PORTF_DATA_R = 0x0;
		  xLastWakeTime2 = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime2,tcross);
			pd=false;
		}
		xLastWakeTime1 = xTaskGetTickCount();
		GPIO_PORTB_DATA_R &= ~0x0C;
		GPIO_PORTF_DATA_R &= ~0x02;
    vTaskPrioritySet(NULL, 1);		

	}
}
/*-----------------------------------------------------------*/

static void vTask3( void *pvParameters ) // train mode
{
	for(;;){

		GPIO_PORTF_DATA_R = 0x02;       // LED is Red (make all lights red)
/*		PWM0_0_LOAD_R = (32000-1);  //set load value to 31999
		PWM0_0_CMPA_R = (16000-1);   //set comparator A value to 15999
		PWM0_0_CTL_R |= 0x01;		 //enable pwm
		PWM0_ENABLE_R |= (0x00000030); //enable pwm values for PE4,PE5
		//siren code
		while ((GPIO_PORTE_DATA_R & SENSOR2) == 0x01){}
		PWM0_0_CTL_R &= ~0x01;		 //disable pwm
		PWM0_0_LOAD_R = (16000-1);  //set load value to 16999
		PWM0_0_CMPA_R = (8000-1);   //set comparator A value to 7999
		PWM0_0_CTL_R |= 0x01;		 //enable pwm
		PWM0_ENABLE_R |= (0x00000030); //enable pwm values for PE4,PE5
    	// vTaskResume( xTask1Handle );
    	// vTaskResume( xTask2Handle );
    	// vTaskResume( xTask3Handle );
    	// vTaskResume( xTask4Handle );
		vTaskPrioritySet(NULL, 0);*/
	}
}
/*-----------------------------------------------------------*/
static void vTask4( void *pvParameters ) // dummy
{
	for(;;){
		
	}
}
