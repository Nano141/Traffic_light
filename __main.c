// This is your second program to run on the LaunchPad
// You will debug this program as your Lab 4
// If both switches SW1 and SW2 are pressed, the LED should be blue
// If just SW1 switch is pressed,            the LED should be red
// If just SW2 switch is pressed,            the LED should be green
// If neither SW1 or SW2 is pressed,         the LED should be off

// 0.Documentation Section 
// main.c
// Runs on LM4F120 or TM4C123
// Lab4_IO, Inputs from PF4,PF0, output to PF3,PF2,PF1 (LED)
// Authors: Daniel Valvano, Jonathan Valvano and Ramesh Yerraballi
// Date: January 15, 2016

// LaunchPad built-in hardware
// SW1 left switch is negative logic PF4 on the Launchpad
// SW2 right switch is negative logic PF0 on the Launchpad
// red LED connected to PF1 on the Launchpad
// blue LED connected to PF2 on the Launchpad
// green LED connected to PF3 on the Launchpad

// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
// TExaS.o is the object code for the real-board grader
//#include "TExaS.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "StackMacros.h"

// 2. Declarations Section
//   Global Variables
unsigned long SW1,SW2;  // input from PF4,PF0
unsigned long Out;      // outputs to PF3,PF2,PF1 (multicolor LED)

//   Function Prototypes
void PortF_Init(void);
void Delay(void);
//void EnableInterrupts(void);
/* Priority definitions for the LED tasks.  Other tasks just use the idle
priority. */
#define mainLED_FLASH_PRIORITY			( tskIDLE_PRIORITY + ( unsigned portBASE_TYPE ) 1 )

static void vTask1( void *pvParameters );
static void vTask2( void *pvParameters );
//static void vTask3( void *pvParameters );

// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){    
//  TExaS_Init(SW_PIN_PF40,LED_PIN_PF321); 
  // TExaS_Init initializes the real board grader for lab 4
  PortF_Init();        // Call initialization of port PF4, PF3, PF2, PF1, PF0    
//  EnableInterrupts();  // The grader uses interrupts
	/* Start the standard LED flash tasks as defined in demo/common/minimal. */
	/*	vStartLEDFlashTasks( mainLED_FLASH_PRIORITY );*/
	xTaskCreate( vTask1, (const portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, mainLED_FLASH_PRIORITY+1, NULL );
//	xTaskCreate( vTask2, (const portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, mainLED_FLASH_PRIORITY+1, NULL );
//	xTaskCreate( vTask3, (const portCHAR *)"Task3", configMINIMAL_STACK_SIZE, NULL, mainLED_FLASH_PRIORITY, NULL );

	/* Start the scheduler. */
	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

static void vTask1( void *pvParameters )
{
volatile unsigned long ul;
volatile unsigned long u2;
	/* Continuously perform a calculation.  If the calculation result is ever
	incorrect turn the LED on. */
	for( ;; )
	{
	xTaskCreate( vTask2, (const portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, mainLED_FLASH_PRIORITY+1, NULL );

    GPIO_PORTF_DATA_R = 0x04;       // LED is blue
    for( ul = 0; ul < 0xFFFE0; ul++ )
		{
			u2=2;
		}
//		vTaskDelay(330);		
		vTaskDelay(1000/portTICK_RATE_MS);
	}
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

static void vTask2( void *pvParameters )
{

	/* Continuously perform a calculation.  If the calculation result is ever
	incorrect turn the LED on. */
	for( ;; )
	{
    GPIO_PORTF_DATA_R = 0x02;     // LED is red
//		vTaskDelay(660);
		vTaskDelay(2000/portTICK_RATE_MS);

	}
}
/*-----------------------------------------------------------*/

static void vTask3( void *pvParameters )
{

	/* Continuously perform a calculation.  If the calculation result is ever
	incorrect turn the LED on. */
	for( ;; )
	{
    GPIO_PORTF_DATA_R = 0x08;   // LED is green
//		vTaskDelay(990);
	  vTaskDelay(3000/portTICK_RATE_MS);

	}
}
//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}
// Subroutine to initialize port F pins for input and output
// PF4 and PF0 are input SW1 and SW2 respectively
// PF3,PF2,PF1 are outputs to the LED
// Inputs: None
// Outputs: None
// Notes: These five pins are connected to hardware on the LaunchPad
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}
// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06


