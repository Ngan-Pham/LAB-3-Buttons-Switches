#include "timer.h"
#include "input_reading.h"
#include "led_display.h"

int timer_traffic_counter = 0;
int timer_traffic_flag = 0;

int timer_blink_counter = 0;
int timer_blink_flag = 0;

// Biến quét 4 LED giống hệt Exercise 4 trong Lab 2:
// 25 * 10ms = 250ms (0.25s mỗi LED -> 4 LED = 1.0s)
int index_led = 1;
int led_counter = 25;

void setTimerTraffic(int duration_ms) {
    timer_traffic_counter = duration_ms / TIMER_CYCLE;
    timer_traffic_flag = 0;
}

void setTimerBlink(int duration_ms) {
    timer_blink_counter = duration_ms / TIMER_CYCLE;
    timer_blink_flag = 0;
}

void timerRun(void) {
    if (timer_traffic_counter > 0) {
        timer_traffic_counter--;
        if (timer_traffic_counter <= 0) timer_traffic_flag = 1;
    }
    if (timer_blink_counter > 0) {
        timer_blink_counter--;
        if (timer_blink_counter <= 0) timer_blink_flag = 1;
    }
}

// Ngắt phần cứng TIM2 gọi mỗi 10ms (giống hệt Program 4.1 trong Lab 2)
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        button_reading(); // Đọc và chống rung nút
        timerRun();       // Giảm các timer mềm

        led_counter--;
        if (led_counter <= 0) {
            led_counter = 25;
            update7SEG(index_led);
            index_led++;
            if (index_led >= 4) {
                index_led = 0;
            }
        }
    }
}
