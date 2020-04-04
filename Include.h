#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

static void vTask1( void *pvParameters );
static void vTask2( void *pvParameters );
static void vTask3( void *pvParameters );
static void PortF_Init(void);

static xTaskHandle xTask1Handle;
static xTaskHandle xTask2Handle;
static xTaskHandle xTask3Handle;
static TickType_t xLastWakeTime1;
static TickType_t xLastWakeTime2;
