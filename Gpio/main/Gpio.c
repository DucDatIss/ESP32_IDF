#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

typedef enum {
    LED_OFF,
    LED_ON
}Led_Single_State;

typedef enum {
    RELEASED,
    PRESSED
}Push_Button_State;

#define LED_PIN GPIO_NUM_15
#define PUSH_BUTTON_PIN GPIO_NUM_25

uint8_t statePushButton = 0;

void Gpio_Setup_Pin(void);
void Gpio_State_Led(uint8_t state);

void app_main(void)
{
    Gpio_Setup_Pin();

    while(1) {
        statePushButton = gpio_get_level(PUSH_BUTTON_PIN);

        Gpio_State_Led(statePushButton);

        vTaskDelay(1);
    }
}

void Gpio_Setup_Pin(void) {
    gpio_reset_pin(LED_PIN);
    gpio_reset_pin(PUSH_BUTTON_PIN);

    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(PUSH_BUTTON_PIN, GPIO_MODE_INPUT);
}

void Gpio_State_Led(uint8_t state) {
    if(state == PRESSED) {
        gpio_set_level(LED_PIN, LED_ON);
    } 
    else if(state == RELEASED) {
        gpio_set_level(LED_PIN, LED_OFF);
    } 
}

