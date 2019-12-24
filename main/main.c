#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "hpdl";

//  Blank Display     BL
//  Data Bus [0..6]   ╿
//      ╿ ╿ ╿ ╿ ╿ ╿ ╿ │
//    │ │ │ │ │ │ │ │ │
//  ╭─╨─╨─╨─╨─╨─╨─╨─╨─╨──╮
//  │                    │
//  │┌───┐┌───┐┌───┐┌───┐│
//  ││ ∷ ││ ∷ ││ ∷ ││ ∷ ││
//  ││ ∷ ││ ∷ ││ ∷ ││ ∷ ││
//  │└───┘└───┘└───┘└───┘│
//  │                    │
//  ├────────────────────┤
//  ╰─╥─╥─╥─╥─╥─╥─╥─╥─╥──╯
//    │ │ │ │ │ │ │ │ │
//                ╽ ╽
// Cursor Address 1 0
//
#define A0  32
#define A1  33

#define D0  2
#define D1  4
#define D2  16
#define D3  17
#define D4  18
#define D5  23
#define D6  19

#define BLANKING    25
#define DATA_WIDTH  7
#define ADDR_WIDTH  2

#define LOW   0
#define HIGH  1

const int DATA_PINS[] = { D6, D5, D4, D3, D2, D1, D0 };
const int ADDR_PINS[] = { A1, A0 };

int _hpdl_set_addr(int x) {
  if (x < 0 || x > 3) {
    ESP_LOGE(TAG, "ADDR {%d} out of bound.");
    return -1;
  }
  gpio_set_level(A0, (x & 0b01));
  gpio_set_level(A1, (x & 0b10));
  return 0;
}

void _hpdl_set_data(int value) {
  int x;
  for (int i = 0; i < DATA_WIDTH; i++) {
    x = value & (1 << i);
    gpio_set_level(DATA_PINS[i], x);
  }
}

void hpdl_initialize(void) {
  // Initialize all pins pulled low. Display would stay blank.
  for (int i = 0; i < DATA_WIDTH; i++) {
    gpio_set_direction(DATA_PINS[i], GPIO_MODE_OUTPUT);
    gpio_set_level(DATA_PINS[i], 0);
  }

  for (int i = 0; i < ADDR_WIDTH; i++) {
    gpio_set_direction(ADDR_PINS[i], GPIO_MODE_OUTPUT);
    gpio_set_level(ADDR_PINS[i], 0);
  }

  gpio_set_direction(BLANKING, GPIO_MODE_OUTPUT);
  gpio_set_level(BLANKING, 0);
  vTaskDelay(10 / portTICK_PERIOD_MS);
}


void hpdl_set_char(int pos, int character) {
  // Pull Blanking low, set the addr and data states, pull blanking high.
  // gpio_set_level(BLANKING, HIGH);
  // vTaskDelay(1 / portTICK_PERIOD_MS);
  _hpdl_set_addr(pos);
  _hpdl_set_data(character);
  vTaskDelay(1 / portTICK_PERIOD_MS);
  gpio_set_level(BLANKING, LOW);
  // Keep this state for 4 ms minimum and pull high.
  vTaskDelay(5 / portTICK_PERIOD_MS);
  gpio_set_level(BLANKING, HIGH);
}


void app_main(void) {
  hpdl_initialize();

  hpdl_set_char(0, '[');
  hpdl_set_char(1, '*');
  hpdl_set_char(2, ']');
  hpdl_set_char(3, ' ');

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  while (1) {
    for (int i = '!'; i <= '_'; i++) {
      for (int j = 0; j < 4; j++) {
        hpdl_set_char(j, i);
      }
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

