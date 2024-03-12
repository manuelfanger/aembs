#include <stdlib.h>
#include "blinky_rp_sdk.h"
#include "platform.h"
#include "McuRTOS.h"

int main(void)
{
	PL_Init();
	vTaskStartScheduler();
	//blinky_sdk_rp();
	PL_Deinit();
  return 0;
}

