#include "McuRTOS.h"

#include "SemLed.h"

#include "leds.h"

static uint32_t taskParamRed = LEDS_RED;
static uint32_t taskParamGreen = LEDS_GREEN;
static uint32_t taskParamBlue = LEDS_BLUE;

static TaskHandle_t SemLedTaskHandleRed = NULL;
static TaskHandle_t SemLedTaskHandleGreen = NULL;
static TaskHandle_t SemLedTaskHandleBlue = NULL;

static SemaphoreHandle_t xSemaphore;

static void SemLedTaskRed(void *pv) {
    LEDS_Leds_e led = *(uint32_t*)pv;
    for(;;) {
        if( xSemaphore != NULL )
        {
            if( xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(10)) == pdTRUE )
            {
                Leds_Neg(led);
                vTaskDelay(pdMS_TO_TICKS(200));
                Leds_Neg(led);
                xSemaphoreGive( xSemaphore );
                vTaskDelay(pdMS_TO_TICKS(800));
            }
            else
            { }
        }
    }
}   

static void SemLedTaskGreen(void *pv) {
    LEDS_Leds_e led = *(uint32_t*)pv;
    for(;;) {
        if( xSemaphore != NULL )
        {
            if( xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(10)) == pdTRUE )
            {
                Leds_Neg(led);
                vTaskDelay(pdMS_TO_TICKS(200));
                Leds_Neg(led);
                xSemaphoreGive( xSemaphore );
                vTaskDelay(pdMS_TO_TICKS(800));
            }
            else
            { }
        }
    }
}

static void SemLedTaskBlue(void *pv) {
    LEDS_Leds_e led = *(uint32_t*)pv;
    for(;;) {
        if( xSemaphore != NULL )
        {
            if( xSemaphoreTake(xSemaphore, pdMS_TO_TICKS(10)) == pdTRUE )
            {
                Leds_Neg(led);
                vTaskDelay(pdMS_TO_TICKS(200));
                Leds_Neg(led);
                xSemaphoreGive(xSemaphore);
                vTaskDelay(pdMS_TO_TICKS(800));
            }
            else
            { }
        }
    }
}



void SemLed_Init(void) {
    BaseType_t res;

    xSemaphore = xSemaphoreCreateBinary();
    if (xSemaphore == NULL) {
        /* Failed ! Not enough heap memory ? */
    } else {
    /* The semaphore can now be used . Calling xSemaphoreTake ()
    on the semaphore will fail until the semaphore has first been
    given . */
    }
    vQueueAddToRegistry(xSemaphore,"LedSemaphore");
    xSemaphoreGive(xSemaphore);

    res = xTaskCreate(SemLedTaskRed, "SemLedTaskRed", 600/sizeof(StackType_t), &taskParamRed, tskIDLE_PRIORITY+3, &SemLedTaskHandleRed);
    if (res!=pdPASS) {
        /* error! */
        for (;;) { 

        }
    }

    res = xTaskCreate(SemLedTaskGreen, "SemLedTaskGreen", 600/sizeof(StackType_t), &taskParamGreen, tskIDLE_PRIORITY+2, &SemLedTaskHandleGreen);
    if (res!=pdPASS) {
        /* error! */
        for (;;) {

        }
    }

    res = xTaskCreate(SemLedTaskBlue, "SemLedTaskBlue", 600/sizeof(StackType_t), &taskParamBlue, tskIDLE_PRIORITY+1, &SemLedTaskHandleBlue);
    if (res!=pdPASS) {
        /* error! */
        for (;;) {

        }
    }

}

void SemLed_Deinit(void) {

}