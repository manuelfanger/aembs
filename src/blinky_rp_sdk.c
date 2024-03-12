#include "McuWait.h"

#include "hardware/gpio.h"

#include "blinky_rp_sdk.h"

#if 0
static void delay_nop(void) {
    for(volatile int i=0; i<5000000; i++) {
        __asm("nop");
    }
}
#endif

void blinky_rp_sdk(void) {
    const unsigned int LED_PIN = PICO_DEFAULT_LED_PIN; // GPIO 25
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    for(;;) {
        gpio_put(LED_PIN, 0); // pin low
        McuWait_Waitms(500);
        gpio_put(LED_PIN, 1); // pin high
        McuWait_Waitms(500);
    }
    return;
}