#include "beep.h"

/*基本状态管理 数据结构*/ 
struct Beep_States {
  uint16_t state; //状态
  int16_t time;   //持续时间
} beep_states[BEEP_MAX_STATES];
uint16_t beep_states_head = 0;
uint16_t beep_states_tail = 0;
uint16_t beep_states_size = 0;
void beep_states_push(uint16_t state, int16_t time) {
  if (beep_states_size == BEEP_MAX_STATES)
    return;
  beep_states[beep_states_tail].state = state;
  beep_states[beep_states_tail].time = time;
  beep_states_tail = (beep_states_tail + 1) % BEEP_MAX_STATES;
  beep_states_size++;
}
void beep_states_pop() {
  if (beep_states_size == 0)
    return;
  beep_states_size--;
  beep_states_head = (beep_states_head + 1) % BEEP_MAX_STATES;
}

/*private*/
void beep_on() { HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET); }

void beep_off() { HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET); }

void beep_once(int16_t time) {
  beep_states_push(BEEP_STATE_ON, time);
  beep_states_push(BEEP_STATE_OFF, time);
}

/*public*/
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
  beep_once(200);
  beep_once(200);
}

void beep_sound_end() {
  //结束音效
  beep_once(400);
}

void beep_on_time(uint16_t interval) {
  //状态队列为空，不处理
  if (beep_states_size == 0)
    return;
  if (beep_states[beep_states_head].time > 0) {
    //当前状态没结束
    beep_states[beep_states_head].time -= interval;
  } else {
    //当前状态结束
    beep_states_pop();
  }
  //同步状态
  if (beep_states[beep_states_head].state == BEEP_STATE_ON) {
    beep_on();
  } else {
    beep_off();
  }
}
