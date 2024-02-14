#include "beep.h"

void beep_on() { HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET); }

void beep_off() { HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET); }

void beep_once(int time) {
  beep_on();
  HAL_Delay(time);
  beep_off();
}

void beep_init() {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = BEEP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void beep_sound_start() {
  beep_once(200);
  HAL_Delay(200);
  beep_once(200);
  HAL_Delay(200);
  beep_once(200);
}

void beep_sound_end() { beep_once(400); }

void beep_on_time(uint16_t interval) {}
