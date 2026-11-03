#include "input_reading.h"

#define DURATION_FOR_AUTO_INCREASING 100 // 100 * 10ms = 1 giây
#define BUTTON_IS_PRESSED            GPIO_PIN_RESET
#define BUTTON_IS_RELEASED           GPIO_PIN_SET

static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceBuffer1[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceBuffer2[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

static uint8_t flagForButtonPress1s[NO_OF_BUTTONS] = {0, 0, 0};
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS] = {0, 0, 0};

// Cờ báo nút vừa được ấn (chỉ tích cực 1 lần nhấn)
static uint8_t button_flag[NO_OF_BUTTONS] = {0, 0, 0};

void button_reading(void) {
    for (char i = 0; i < NO_OF_BUTTONS; i++) {
        debounceBuffer2[i] = debounceBuffer1[i];

        // Đọc 3 nút trên PB0, PB1, PB2
        if (i == 0) debounceBuffer1[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
        else if (i == 1) debounceBuffer1[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
        else if (i == 2) debounceBuffer1[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);

        if (debounceBuffer1[i] == debounceBuffer2[i]) {
            if (buttonBuffer[i] != debounceBuffer1[i]) {
                buttonBuffer[i] = debounceBuffer1[i];
                if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
                    button_flag[i] = 1; // Phát hiện sự kiện vừa nhấn
                }
            }

            if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
                if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
                    counterForButtonPress1s[i]++;
                } else {
                    flagForButtonPress1s[i] = 1;
                }
            } else {
                counterForButtonPress1s[i] = 0;
                flagForButtonPress1s[i] = 0;
            }
        }
    }
}

unsigned char is_button_pressed(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    if (button_flag[index] == 1) {
        button_flag[index] = 0; // Xóa cờ sau khi đọc (One-shot event)
        return 1;
    }
    return 0;
}

unsigned char is_button_pressed_1s(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    return (flagForButtonPress1s[index] == 1);
}
