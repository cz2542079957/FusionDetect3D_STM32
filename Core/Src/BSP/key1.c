#include "key1.h"
#include "stdio.h"

//当前状态(上一帧状态)
uint8_t state = KEY1_RELEASE;
//当前按下的时间
uint16_t current_press_time = 0;

int key1_is_pressed() {
  return (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET);
}

uint8_t key1_get_state() { return state; }

void key1_init() {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOD_CLK_ENABLE();
  GPIO_InitStruct.Pin = KEY1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStruct);
}

void key1_on_time(uint16_t interval) {
  if (state == KEY1_RELEASE) {
    printf("debug: release\n");
  } else if (state == KEY1_LONG_PRESS) {
    printf("debug: long press\n");
  } else {
    printf("debug: press\n");
  }
  if (key1_is_pressed()) {
    //按下状态
    current_press_time += interval;
    //判定为长按
    if (current_press_time >= KEY1_LONG_PRESS_TIME) {
      state = KEY1_LONG_PRESS;
    }
  } else {
    //判定为短按
    if (current_press_time < KEY1_LONG_PRESS_TIME && current_press_time > 0) {
      state = KEY1_PRESS;
    }
    //判定松开
    if (current_press_time == 0) {
      state = KEY1_RELEASE;
    }
    current_press_time = 0;
  }
}
