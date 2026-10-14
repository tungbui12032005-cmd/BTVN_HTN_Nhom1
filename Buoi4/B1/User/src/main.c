#include "FreeRTOS.h"
#include "queue.h"
#include "stm32f10x.h"
#include "task.h"
#include <stdio.h>

void LedConfig() {
  GPIO_InitTypeDef GPIO;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO);

  // Khoi tao muc thap cho ca 3 LED
  GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
}

void LedToggle(uint16_t GPIO_Pin) {
  if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin)) {
    GPIO_ResetBits(GPIOA, GPIO_Pin);
  } else {
    GPIO_SetBits(GPIOA, GPIO_Pin);
  }
}

void LED_TASK(void *pvParameters) {
  uint32_t count = 0;

  while (1) {
    // LED3 (PA2): dao trang thai moi 50ms (chu ky 100ms -> 10Hz)
    LedToggle(GPIO_Pin_2);

    // LED2 (PA1): dao trang thai moi 500ms (chu ky 1000ms -> 1Hz)
    if (count % 10 == 0) {
      LedToggle(GPIO_Pin_1);
    }

    // LED1 (PA0): dao trang thai moi 5000ms (chu ky 10000ms -> 0.1Hz)
    if (count % 100 == 0) {
      LedToggle(GPIO_Pin_0);
    }

    count++;
    if (count >= 100) {
      count = 0;
    }

    vTaskDelay(50); // Delay 50ms (UCLN cua 50ms, 500ms, 5000ms)
  }
}

int main() {
  SystemInit();
  LedConfig();

  xTaskCreate(LED_TASK, "LED_TASK", 128, NULL, 1, NULL);
  vTaskStartScheduler();
  while (1)
    ;
}