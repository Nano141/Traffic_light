#include "include.h"
#include "PD_mode.h"
	
static void vTask1( void *pvParameters)
{
	bool *pd;
	pd = (bool *) pvParameters;
	xLastWakeTime1 = xTaskGetTickCount();
	 const TickType_t nsDelayTime = (5000/portTICK_RATE_MS);
	for( ;; )
	{
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
		GPIO_PORTF_DATA_R = !0x04;
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
        GPIO_PORTF_DATA_R = 0x02;       // LED is Red
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
		GPIO_PORTF_DATA_R = !0x02;
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

void PortF_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x00000020; // activate clock for port F
  GPIO_PORTF_DIR_R |= 0x0E;  
  GPIO_PORTF_DEN_R |= 0x0E;     // enable digital I/O on PF3-PF1 
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_DEN_R = 0x0E;          // 7) enable digital pins PF3-PF1        
}
/*-----------------------------------------------------------*/

void PortE_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x00000010; // activate clock for port E
  GPIO_PORTE_DIR_R &= ~0x03;  		// make PE0,PE1 direction input
  GPIO_PORTE_DIR_R |= 0x30;  		// make PE4,PE5 direction output
  GPIO_PORTE_PUR_R |= 0x03;  		// enable pull up resistor for PE0,PE1
  GPIO_PORTE_DEN_R |= 0x33;     // enable digital I/O on PE0,PE1,PE4,PE5
  GPIO_PORTE_AFSEL_R |= 0x30;	//enable alternative fucntion for PE4,PE5
  GPIO_PORTE_DR4R_R |= 0x30;
  GPIO_PORTE_PCTL_R |= 0x440000;	//use M0PWM4, M0PWM5 for PE4,PE5 respectively
  GPIO_PORTE_IS_R &= ~0x03;       //interrupt statues = edge for PE0,PE1
  GPIO_PORTE_IBE_R &= ~0x03;		//single edge for PE0,PE1
  GPIO_PORTE_IEV_R &= ~0x03;		//falling edge for PE0,PE1
  GPIO_PORTE_ICR_R = 0x01;		//clear interrupt flag for PE0
  GPIO_PORTE_IM_R |= 0x03;		//unmask interrupts for PE0,PE1
  NVIC_PRI1_R &= ~0xE0;         //make train interrupt priority = 0 (highest)
  NVIC_EN0_R |= 0x10;			//enable interrupt register for PORTE
  EnableInterrupts();				//enable interrupts globally
  //GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  //GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  //GPIO_PORTE_AFSEL_R = 0x00;	        // 6) no alternate function
}
/*-----------------------------------------------------------*/

void PWM_init(void){
	SYSCTL_RCGC0_R |= 0x100000; 	//Enable the PWM clock
	SYSCTL_RCGC2_R |= 0x10;			//Enable the clock to the GPIO PORTE			
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_USEPWMDIV);
	while((SYSCTLRCGC2_R) & 0x10 == 0){}
	PWM0_2_CTL_R &= ~0x001;	//disable PWM module
	PWM0_2_CTL_R &= ~0x002;	//set count down mode
	PWM0_0_GENA_R |= 0x83;  //drive PWM low on cmpA and high on zero
}

/*-----------------------------------------------------------*/
void GPIO_PortE_Handler(void){
  GPIO_PORTE_ICR_R = 0x01;		//clear interrupt flag for PE0
  vTaskPrioritySet(xTask5Handle, 5); //set priority of task5 (train mode) to 5
}

