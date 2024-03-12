#include "McuGPIO.h"

#include "leds.h"
#include "gpio.h"

static McuGPIO_Handle_t myPin;

void Gpio_Test(void) {
    static int lowCounter = 0;

    if(McuGPIO_IsLow(myPin)) {
        lowCounter++;
        Leds_Neg(LEDS_RED);
        Leds_Neg(LEDS_GREEN);
        Leds_Neg(LEDS_BLUE);
    }
}

void Gpio_Init(void) {
    McuGPIO_Config_t config;
    McuGPIO_GetDefaultConfig(&config);
    config.isInput = true;
    config.hw.pin = 11;
    config.hw.pull = McuGPIO_PULL_UP;

    myPin = McuGPIO_InitGPIO(&config);
    if(myPin == NULL) {
        for(;;) { /* ERROR */ }
    }
}

void Gpio_Deinit(void) {
    myPin = McuGPIO_DeinitGPIO(myPin);
}