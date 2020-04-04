#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include <stdbool.h>
#include <ctype.h>
#define mask1 (0x01)

static void PortB_Init(void);
static void GPIOPortB_Handler(void);

static xTaskHandle PDTaskHandle;
static int tcross ;
static bool PD ;



