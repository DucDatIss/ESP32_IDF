#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

// Just using one in two
//#define USING_FREERTOS
#define NOT_USING_FREERTOS

typedef enum
{
    LED_OFF,
    LED_ON
} Led_Single_State;

typedef enum
{
    RELEASED,
    PRESSED
} Push_Button_State;

#define LED_PIN_1 GPIO_NUM_15
#define LED_PIN_2 GPIO_NUM_4
#define LED_PIN_BOARD GPIO_NUM_2

#define PUSH_BUTTON_PIN GPIO_NUM_25

void Gpio_Setup_Pin(void);

#ifdef NOT_USING_FREERTOS

uint8_t statePushButton = 0;
int stateSystem = 0;

void Gpio_State_Led(uint8_t state);
void Notify_System_Run(void);

void app_main(void)
{
    Gpio_Setup_Pin();

    while (1)
    {
        statePushButton = gpio_get_level(PUSH_BUTTON_PIN);

        Gpio_State_Led(statePushButton);

        //Notify_System_Run();

        vTaskDelay(1);
    }
}

void Gpio_State_Led(uint8_t state)
{
    if (state == PRESSED)
    {
        gpio_set_level(LED_PIN_1, LED_ON);
    }
    else if (state == RELEASED)
    {
        gpio_set_level(LED_PIN_1, LED_OFF);
    }
}

void Notify_System_Run(void)
{
    static int cout = 0;

    cout++;
    if(cout==100)
    {
        cout=0;
    }
    else
    {
        return;
    }

    if(stateSystem == 1)
    {
        stateSystem = 0;
        gpio_set_level(LED_PIN_BOARD, LED_ON);
    }
    else
    {
        stateSystem = 1;
        gpio_set_level(LED_PIN_BOARD, LED_OFF);
    }
}

#endif

#ifdef USING_FREERTOS

void Task_Button(void* pvParameters);
void Task_Led(void* pvParameters);

void app_main(void)
{
    Gpio_Setup_Pin();

    xTaskCreate(Task_Button, "Button", 100, NULL, 1, NULL);
    xTaskCreate(Task_Led, "Led", 100, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1)
    {
        // statePushButton = gpio_get_level(PUSH_BUTTON_PIN);

        // Gpio_State_Led(statePushButton);

        // vTaskDelay(1);
    }
}

void Task_Button(void* pvParameters)
{
    while(1)
    {
        statePushButton = gpio_get_level(PUSH_BUTTON_PIN);

        vTaskDelay(1);
    }
}

void Task_Led(void* pvParameters)
{
    while(1)
    {
        if (statePushButton == PRESSED)
        {
            gpio_set_level(LED_PIN, LED_ON);
        }
        else if (statePushButton == RELEASED)
        {
            gpio_set_level(LED_PIN, LED_OFF);
        }

        vTaskDelay(2);
    }
}

#endif

void Gpio_Setup_Pin(void)
{
    // output
    gpio_reset_pin(LED_PIN_1);
    gpio_reset_pin(LED_PIN_2);
    gpio_reset_pin(LED_PIN_BOARD);

    gpio_set_direction(LED_PIN_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_PIN_2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_PIN_BOARD, GPIO_MODE_OUTPUT);

    // input
    gpio_reset_pin(PUSH_BUTTON_PIN);

    gpio_set_direction(PUSH_BUTTON_PIN, GPIO_MODE_INPUT);
}





