#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include <ctype.h>
#include <stdbool.h>

#define SENSOR1 (0x0001)
#define SENSOR2 (0x0002)
#define mask1 (0x01)

static TickType_t tcross ;
static bool PD ;
static xTaskHandle xTask1Handle;
static xTaskHandle xTask2Handle;
static xTaskHandle xTask3Handle;
static xTaskHandle xTask4Handle;
static xTaskHandle PDTaskHandle;
static TickType_t xLastWakeTime1;
static TickType_t xLastWakeTime2;


static void vTask1( void *pvParameters );
static void vTask2( void *pvParameters );
static void vTask3( void *pvParameters );
static void vTask4( void *pvParameters );
static void PortF_Init(void);
static void PortE_Init(void);
static void PortB_Init(void);
static void PWM_Init(void);
static void GPIO_PortE_Handler(void);
static void GPIOB_Handler(void);
