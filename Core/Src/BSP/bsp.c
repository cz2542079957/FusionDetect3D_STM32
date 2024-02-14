#include "bsp.h"
//是否开启校验和
int enable_check_sum = DEFAULT_CHECK_SUM;

void bsp_init() {
  // 串口通讯初始化
  uart_init();
  // 板载按键1初始化
  key1_init();
  //  led灯初始化
  led_init();
  // 蜂鸣器初始化
  beep_init();
}

//各硬件功能融合组成服务
void on_time_service() {
  //按键key1短按、长按功能
  static uint8_t last_key1_state = KEY1_RELEASE;
  if (key1_get_state() == KEY1_PRESS) {
    //检测为按下
    beep_once(30);
  } else if (key1_get_state() == KEY1_LONG_PRESS &&
             last_key1_state != KEY1_LONG_PRESS) {
    //检测长按
    led_flash_with_interval(10, 30);
    led_keep_on();
  }
  last_key1_state = key1_get_state();
}

void on_time(uint16_t interval) {
  // led
  led_on_time(interval);
  // beep
  beep_on_time(interval);
  // key1
  key1_on_time(interval);
  // uart
  uart_on_time(interval);
  // 串口数据解析
  uart_data_paser();
  uart_data_publisher();

  // 服务
  on_time_service();
}

//用于解析串口命令
void uart_data_paser() {
  uint8_t index = uart_buffer.head;
  uint16_t size = uart_buffer.size;
  //去除损坏帧
  while (!(uart_buffer.data[index] == FRAME_HEAD1 &&
           uart_buffer.data[uart_get_index(index + 1)] == FRAME_HEAD2) &&
         uart_buffer.size > 0) {
    index = uart_get_index(index + 1);
    uart_buffer.size--;
  }
  uart_buffer.head = index;
  //开始处理数据
  while (uart_buffer.size > MIN_FRAME_LENGTH) {
    //包头比对
    if (uart_buffer.data[index] == FRAME_HEAD1 &&
        uart_buffer.data[uart_get_index(index + 1)] == FRAME_HEAD2) {
      //帧长度
      uint8_t frame_length =
          uart_buffer.data[uart_get_index(index + 2)] + FRAME_HEAD_LENGTH;
      if (uart_buffer.size < frame_length) {
        //剩余数据不完整
        return;
      }
      //获取功能字
      uint8_t function = uart_buffer.data[uart_get_index(index + 3)];
      //校验和
      if (enable_check_sum) {
      }

      printf("frame_length: %d\n", frame_length);

      switch (function) {
      case 0x01:
        //自动发送设置

        break;
      case 0x11:
        //打开蜂鸣器
        break;
      }

      uart_buffer.head = uart_get_index(uart_buffer.head + frame_length);
      uart_buffer.size -= frame_length;
    }

    // printf("uart_buffer.size: %u\n", uart_buffer.size);
    // printf("uart_buffer.head: %u\n", uart_buffer.head);
    // printf("uart_buffer.tail: %u\n", uart_buffer.tail);
  }
}

//用于打包发送各种数据
void uart_data_publisher() {
  // printf("uart_buffer.size: %d\n", uart_buffer.size);
}