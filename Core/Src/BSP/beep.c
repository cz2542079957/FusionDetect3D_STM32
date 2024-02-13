#include "beep.h"

// private
void beep_on()
{
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
}

void beep_off()
{
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
}

// public
void beep_sound_start()
{
    for (int i = 0; i < 10; i++)
    {
        beep_on();
        HAL_Delay(50);
        beep_off();
    }
}

void beep_sound_end() {}
