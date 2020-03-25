#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

void PortF_Init(void);

static void vTask1( void *pvParameters );
static void vTask2( void *pvParameters );

xTaskHandle xTask1Handle;
xTaskHandle xTask2Handle;