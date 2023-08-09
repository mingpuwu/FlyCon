#include "led.h"
#include "gpio.h"

int LedBlink()
{
    int i = 0;
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
    i = 0;
    while(i<100000)
    {
        i++;
    }

    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
    i = 0;
    while(i<100000)
    {
        i++;
    }

    return 0;
}
