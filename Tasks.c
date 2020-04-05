#include "init.c"
#include "HAL.h"


static void vTask1( void *pvParameters)
{
	bool *pd;
	pd = (bool *) pvParameters;
	xLastWakeTime1 = xTaskGetTickCount();
	 const TickType_t nsDelayTime = (5000/portTICK_RATE_MS);
	for( ;; )
	{
		GPIO_PORTB_DATA_R |= 0x012;
        GPIO_PORTF_DATA_R = 0x04;       // LED is Blue
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
				GPIO_PORTB_DATA_R |= 0x0C;
        GPIO_PORTF_DATA_R |= 0x02;       // LED is Red
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
    	// vTaskSuspend( xTask1Handle );
    	// vTaskSuspend( xTask2Handle );
    	// vTaskSuspend( xTask3Handle );
    	// vTaskSuspend( xTask4Handle );
		GPIO_PORTF_DATA_R = 0x02;       // LED is Red (make all lights red)
		PWM0_0_LOAD_R = (32000-1);  //set load value to 31999
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
		vTaskPrioritySet(NULL, 0);
	}
}
/*-----------------------------------------------------------*/
static void vTask4( void *pvParameters ) // dummy
{
	for(;;){
		if (!(GPIO_PORTB_DATA_R &= mask1)){
			GPIO_PORTF_DATA_R |= 0x02;
	GPIO_PORTF_DATA_R |= 0x04;
	vTaskDelay(100);
	GPIO_PORTF_DATA_R &= ~(0x02); //turn off LED
	GPIO_PORTF_DATA_R &= ~(0x04);
	};
};
}
/////////////////////////////////////////////////////////////////////////////

void interupt_init(){
	
	GPIO_PORTF_IS_R &= ~0x1;					//edge
	GPIO_PORTF_IBE_R &= ~0x1;					//single edge
	GPIO_PORTF_IEV_R |= 0x1;					//rising edge
	GPIO_PORTF_ICR_R = 0x10;					//clear flag 4
	GPIO_PORTF_IM_R |= 0x10;					//unmask for port f
	NVIC_PRI7_R =(NVIC_PRI7_R& 0xff00ffff)|0x00100000; //priority 1
	NVIC_EN0_R =0x40000000;						//IRQ of port f
	
	
	
}


void GPIOF_Interrupt(uint32_t pinMap){
	const TickType_t SafetyDelay = (30000/portTICK_RATE_MS);
	GPIO_DisarmInterrupt(&PINDEF(PORTF, (PinName_t)(PIN0 | PIN4)));
	/*vTaskSuspend( xTask1Handle );
	vTaskSuspend( xTask2Handle );
	vTaskSuspend( xTask3Handle );
	vTaskSuspend( xTask4Handle );*/
	GPIO_PORTF_DATA_R |= 0x02;       // LED is Red
	GPIO_PORTF_DATA_R |= 0x04;
	vTaskDelay(1000);
	GPIO_PORTF_DATA_R &= ~(0x02); //turn off LED
	GPIO_PORTF_DATA_R &= ~(0x04);
	/*vTaskResume(xTask1Handle);
	vTaskResume(xTask2Handle);*/
	GPIO_RearmInterrupt(&PINDEF(PORTF, (PinName_t)(PIN0 | PIN4)));
	
	}

