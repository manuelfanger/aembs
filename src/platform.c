#include "McuLib.h"
#include "McuWait.h"
#include "McuGPIO.h"
#include "McuUtility.h"
#include "McuLED.h"
#include "McuRTOS.h"
#include "McuRTT.h"
#include "McuButton.h"
#include "McuDebounce.h"
#include "McuSystemView.h"
#include "McuSHT31.h"
#include "McuGenericI2C.h"
#include "McuI2CLib.h"

#include "boulder_images.h"
#include "sprite.h"
#include "gameModel.h"
#include "gameView.h"
#include "boulder.h"
#include "csem.h"
#include "Screensaver.h"
#include "SemLed.h"
#include "dtnOLED.h"

#include "gpio.h"
#include "leds.h"
#include "buttons.h"
#include "debounce.h"
#include "myTasks.h"
#include "game.h"
#include "application.h"
#include "sensor.h"
#include "i2cbus.h"

#include "platform.h"

uint32_t SystemCoreClock = 120000000;

void PL_Init(void)
{
    McuLib_Init();
    McuRTT_Init();
    McuSystemView_Init();
    McuWait_Init();
    McuGPIO_Init();
    McuLED_Init();
    McuRTOS_Init();
    McuBtn_Init();
    McuDbnc_Init();
    McuGenericI2C_Init();
    McuI2cLib_Init();
    McuSHT31_Init();
    // Sensor_Init();
    BTN_Init();
    Debounce_Init();
    Leds_Init();
    Gpio_Init();
    Gpio_Test();
    // MyTasks_Init();
    // Game_Init();
    // Application_Init();
    // I2CBus_Init();
    // Csem_Init();
    // Screensaver_Init();
    SemLed_Init();
    // DtnOLED_Init();
}

void PL_Deinit(void)
{
    // DtnOLED_Init();
    SemLed_Deinit();
    Screensaver_Deinit();
    // Csem_Deinit();
    I2CBus_Deinit();
    // Application_Deinit();
    Game_Deinit();
    // MyTasks_Deinit();
    Gpio_Deinit();
    Leds_Deinit();
    Debounce_Deinit();
    BTN_Deinit();
    Sensor_Deinit();
    McuSHT31_Deinit();
    McuI2cLib_Deinit();
    McuGenericI2C_Deinit();
    McuDbnc_Deinit();
    McuBtn_Deinit();
    McuRTOS_Deinit();
    McuLED_Deinit();
    McuGPIO_Deinit();
    McuWait_Deinit();
    McuSystemView_Deinit();
    McuRTT_Deinit();
    McuLib_Deinit();
}