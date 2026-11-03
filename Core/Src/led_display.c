#include "led_display.h"

// Bảng giải mã LED 7 đoạn Anode chung (mức 0 là SÁNG đoạn đó)
const uint8_t segment_code[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// Mảng 4 giá trị số hiển thị trên 4 LED (khởi tạo mặc định số 0)
int led_buffer[4] = {0, 0, 0, 0};

void display7SEG(int num) {
    if (num < 0 || num > 9) num = 0;
    uint32_t code = segment_code[num];
    // Xuất đồng thời cả 7 đoạn PA0 -> PA6 bằng thanh ghi BSRR (bit=1 -> RESET sáng, bit=0 -> SET tắt)
    uint32_t reset_pins = (code & 0x7FU);
    uint32_t set_pins   = ((~code) & 0x7FU);
    GPIOA->BSRR = (reset_pins << 16U) | set_pins;
}

void updateLedBuffer(int val1, int val2) {
    led_buffer[0] = (val1 / 10) % 10;
    led_buffer[1] = val1 % 10;
    led_buffer[2] = (val2 / 10) % 10;
    led_buffer[3] = val2 % 10;
}

// Quét 4 LED 7 đoạn giống hệt Program 3.1 trong Lab 2
void update7SEG(int index) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_SET);

    switch (index) {
        case 0:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
            display7SEG(led_buffer[0]);
            break;
        case 1:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
            display7SEG(led_buffer[1]);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
            display7SEG(led_buffer[2]);
            break;
        case 3:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
            display7SEG(led_buffer[3]);
            break;
        default:
            break;
    }
}

// Điều khiển cụm đèn giao thông (mức 0 sáng LED)
void setTrafficRoad1(int red, int yellow, int green) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, red ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, yellow ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, green ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void setTrafficRoad2(int red, int yellow, int green) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, red ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, yellow ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, green ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void toggleRedLeds(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7 | GPIO_PIN_10);
}

void toggleYellowLeds(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8 | GPIO_PIN_11);
}

void toggleGreenLeds(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9 | GPIO_PIN_12);
}

void turnOffAllTrafficLeds(void) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_SET);
}
