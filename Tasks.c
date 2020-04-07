#include "init.c"


static void vTask1( void *pvParameters)
{
	bool *pd;
	pd = (bool *) pvParameters;
	xLastWakeTime1 = xTaskGetTickCount();
	 const TickType_t nsDelayTime = (5000/portTICK_RATE_MS);
	for( ;; )
	{
		inter = 0;
		printf("North-South junction is on");
		vTaskPrioritySet(xTask2Handle, 3);
		GPIO_PORTF_DATA_R &= ~0x06; // Turn off all Tiva LEDs
		GPIO_PORTB_DATA_R &= ~0x1E; // Turn off junction LEDs
		GPIO_PORTB_DATA_R |= 0x012; // Turn NS Green and EW Red
		GPIO_PORTF_DATA_R |= 0x04; // Turn Tiva LED Blue
		vTaskDelayUntil(&xLastWakeTime1,nsDelayTime);
if ((inter) == 1)
			{
			printf("Pedestrian mode is on");
			GPIO_PORTF_DATA_R &= ~0x0E; // Turn off all Tiva LEDs
			GPIO_PORTB_DATA_R &= ~0x1E; // Turn off junction LEDs
			GPIO_PORTB_DATA_R |= 0x14; // Turn NS Red and EW Red
			xLastWakeTime4 = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime4,tcross);
			printf("Pedestrian mode is off");
			}
		xLastWakeTime2 = xTaskGetTickCount();
		GPIO_PORTB_DATA_R &= ~0x01E; // Turn off junction LEDs
		GPIO_PORTF_DATA_R &= ~0x06; // Turn off all Tiva LEDs
			if ((inter)!= 2)
			{
		inter = 0;
		printf("North-South junction is off");
		vTaskPrioritySet(xTask2Handle, 5);
			}
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
		inter = 0;
		printf("East-West junction is on");
				vTaskPrioritySet(xTask1Handle, 3);
				GPIO_PORTF_DATA_R &= ~0x06; // Turn off all Tiva LEDs
				GPIO_PORTB_DATA_R &= ~0x1E; // Turn off junction LEDs
				GPIO_PORTB_DATA_R |= 0x0C; // Turn EW Green and NS Red
        GPIO_PORTF_DATA_R |= 0x02; // Turn Tiva LED Red
		vTaskDelayUntil(&xLastWakeTime2,ewDelayTime);
if ((inter) == 1)
			{
			printf("Pedestrian mode is on");
			GPIO_PORTF_DATA_R &= ~0x0E; // Turn off all Tiva LEDs
			GPIO_PORTB_DATA_R &= ~0x1E; // Turn off junction LEDs
			GPIO_PORTB_DATA_R |= 0x14; // Turn NS Red and EW Red 
			xLastWakeTime4 = xTaskGetTickCount();
			vTaskPrioritySet(xTask1Handle, 1);
			vTaskDelayUntil(&xLastWakeTime4,tcross);
			printf("Pedestrian mode is off");
			}
		xLastWakeTime1 = xTaskGetTickCount();
		GPIO_PORTB_DATA_R &= ~0x1E; // Turn off junction LEDs
		GPIO_PORTF_DATA_R &= ~0x06; // Turn off all Tiva LEDs
		printf("East-West junction is off");
		vTaskPrioritySet(xTask1Handle, 5);

	}
}
/*-----------------------------------------------------------*/

static void vTask3( void *pvParameters ) // train mode
{
	const TickType_t SafetyDelayTime = (30000/portTICK_RATE_MS);
	for(;;){
		printf("Train mode is on");
		state = (GPIO_PORTB_DATA_R & 0x1E);
		GPIO_PORTB_DATA_R &= ~0x1E;
		GPIO_PORTB_DATA_R |= 0x14;       // LED is Red (make all lights red)
		GPIO_PORTE_DEN_R |=(1<<4); //enable port E pin 4
		PWM0_2_LOAD_R = (320000-1);  //set load value
		PWM0_2_CMPA_R = (296000-1);   //set comparator A value
		PWM0_2_CTL_R |= 0x01;		 //enable pwm
		PWM0_ENABLE_R |= (PWM_ENABLE_PWM4EN); //enable pwm values for PE4
		vTaskPrioritySet(xTask4Handle, 6);
		xLastWakeTime3 = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime3,SafetyDelayTime);	//wait for safety time to pass (30s)
		while ((GPIO_PORTB_DATA_R & SENSOR2) == 0x040){		//wait till sensor 2 read a gignal this means that the train passed already
			if ((GPIO_PORTF_DATA_R & 0x10) == 0)
			{
				inter = 2 ;
				xTaskAbortDelay(xTask2Handle);
				xTaskAbortDelay(xTask3Handle);
				xTaskAbortDelay(xTask4Handle);
				vTaskPrioritySet(xTask1Handle, 5);
				vTaskPrioritySet(xTask2Handle, 3);
				xLastWakeTime1 = xTaskGetTickCount();
				goto reset;
			}
		}  
		PWM0_2_CTL_R &= ~0x01;		 //disable pwm
		PWM0_2_LOAD_R = (320000-1);  //set load value
		PWM0_2_CMPA_R = (288000-1);   //set comparator A value
		PWM0_2_CTL_R |= 0x01;		 //enable pwm
		PWM0_ENABLE_R |= (PWM_ENABLE_PWM4EN); //enable pwm values for PE4
		vTaskDelay(1000/portTICK_PERIOD_MS);
		reset:
		PWM0_2_CTL_R &= ~0x01;		 //disable pwm
		vTaskPrioritySet(xTask4Handle, 4);
		GPIO_PORTE_DEN_R &= ~(1<<4); //disable port E pin 4
		GPIO_PORTB_DATA_R &= ~0x14;    
		GPIO_PORTB_DATA_R |= state;
		printf("Train mode is off");
		vTaskPrioritySet(NULL, 0);
	}
}
/*-----------------------------------------------------------*/
static void vTask4( void *pvParameters ) // mother task
{
	for(;;){
		//polling of train mode
		if((GPIO_PORTB_DATA_R & SENSOR1) != 0x020){
			vTaskPrioritySet(xTask3Handle, 7);
		}
		//polling of pedestrian mode
		if ((GPIO_PORTB_DATA_R & 0x01) == 0)
			{
					inter = 1;
			}
			//polling of reset
		else if ((GPIO_PORTF_DATA_R & 0x10) == 0)
		{
			inter = 2 ;
			printf("Reset button is pressed");
			xTaskAbortDelay(xTask2Handle);
			xTaskAbortDelay(xTask3Handle);
			xTaskAbortDelay(xTask4Handle);
			vTaskPrioritySet(xTask4Handle, 6);
			vTaskPrioritySet(xTask1Handle, 5);
			vTaskPrioritySet(xTask2Handle, 3);
			vTaskPrioritySet(xTask3Handle, 0);
			xLastWakeTime1 = xTaskGetTickCount();
			vTaskPrioritySet(xTask4Handle, 4);
			
		}

	}
}
