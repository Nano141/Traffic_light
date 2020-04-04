#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#define SENSOR1 (0x0001)
#define SENSOR2 (0x0002)


static void vTask1( void *pvParameters );
static void vTask2( void *pvParameters );
static void vTask3( void *pvParameters );
static void vTask4( void *pvParameters );
static void PortF_Init(void);
static void PortE_Init(void);
static void PWM_Init(void);
static void GPIO_PortE_Handler();

static xTaskHandle xTask1Handle;
static xTaskHandle xTask2Handle;
static xTaskHandle xTask3Handle;
static xTaskHandle xTask4Handle;
static TickType_t xLastWakeTime1;
static TickType_t xLastWakeTime2;
