#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include <ctype.h>
#include <stdbool.h>
#include <semphr.h>

#define SENSOR1 (0x00020) //PB5
#define SENSOR2 (0x00040) //PB6

static TickType_t tcross ;
static int inter ;
static xTaskHandle xTask1Handle;
static xTaskHandle xTask2Handle;
static xTaskHandle xTask3Handle;
static xTaskHandle xTask4Handle;
static TickType_t xLastWakeTime1;
static TickType_t xLastWakeTime2;
static TickType_t xLastWakeTime3;
static TickType_t xLastWakeTime4;
static char state;

static void vTask1( void *pvParameters );
static void vTask2( void *pvParameters );
static void vTask3( void *pvParameters );
static void vTask4( void *pvParameters );
static void PortF_Init(void);
static void PortE_Init(void);
static void PortB_Init(void);
