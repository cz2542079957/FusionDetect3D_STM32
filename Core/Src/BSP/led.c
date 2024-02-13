#include "led.h"

// led状态 0:关闭 1:常亮 2:延迟关闭
uint8_t led_state = 0;
// led保持点亮时间
uint16_t led_keep_on_time = 0;

void led_on() { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); }

void led_off() { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); }

void led_init() {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void led_keep_on() {
  led_state = LED_KEEP_ON_MODE;
  led_on();
}

void led_delay(uint16_t time) {
  led_keep_on_time = time;
  led_state = LED_DELAY_MODE;
  led_on();
}
void led_on_time(uint16_t interval) {
  if (led_state == LED_DELAY_MODE) {
    led_keep_on_time -= interval;
    if (led_keep_on_time <= 0) {
      led_off();
      led_state = LED_OFF;
    }
  } else if (led_state == LED_KEEP_ON_MODE) {
    led_on();
  } else {
    led_off();
  }
}
