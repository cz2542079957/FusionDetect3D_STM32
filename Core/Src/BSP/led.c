#include "led.h"

#include "stdio.h"

/*基本状态管理 数据结构*/
#define LED_MAX_STATES 128u
#define LED_STATE_ON 1
#define LED_STATE_OFF 0
struct LED_States {
  uint16_t state; //状态
  int16_t time;   //持续时间
} led_states[LED_MAX_STATES];
uint16_t led_states_head = 0;
uint16_t led_states_tail = 0;
uint16_t led_states_size = 0;
void led_states_push(int state, int time) {
  if (led_states_size == LED_MAX_STATES)
    return;
  led_states[led_states_tail].state = state;
  led_states[led_states_tail].time = time;
  led_states_tail = (led_states_tail + 1) % LED_MAX_STATES;
  led_states_size++;
}
void led_states_pop() {
  if (led_states_size == 0)
    return;
  led_states_size--;
  led_states_head = (led_states_head + 1) % LED_MAX_STATES;
}

/*private*/
void led_on() { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); }

void led_off() { HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); }

/*public*/
void led_init() {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void led_keep_on() { led_states_push(LED_STATE_ON, 100); }

void led_delay(uint16_t time) {
  led_states_push(LED_STATE_ON, time);
  led_states_push(LED_STATE_OFF, 100);
}
void led_flash(uint16_t num) {
  for (uint16_t i = 0; i < num; i++) {
    led_states_push(LED_STATE_ON, 70);
    led_states_push(LED_STATE_OFF, 70);
  }
}
void led_flash_with_interval(uint16_t num, uint16_t interval) {
  for (uint16_t i = 0; i < num; i++) {
    led_states_push(LED_STATE_ON, interval);
    led_states_push(LED_STATE_OFF, interval);
  }
}
void led_on_time(uint16_t interval) {
  // printf("debug: %u\n", led_states_size);
  //状态队列为空，不处理
  if (led_states_size == 0)
    return;
  if (led_states[led_states_head].time > 0) {
    //当前状态没结束
    led_states[led_states_head].time -= (int16_t)interval;
  } else {
    //当前状态结束
    led_states_pop();
    return;
  }
  //同步状态
  if (led_states[led_states_head].state == LED_STATE_ON) {
    led_on();
  } else {
    led_off();
  }
}
