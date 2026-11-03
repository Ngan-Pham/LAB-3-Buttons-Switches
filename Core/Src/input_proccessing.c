#include "input_processing.h"
#include "input_reading.h"
#include "led_display.h"
#include "timer.h"

enum SystemMode {
    MODE_NORMAL = 1,
    MODE_MODIFY_RED = 2,
    MODE_MODIFY_AMBER = 3,
    MODE_MODIFY_GREEN = 4
};

enum LightColor {
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW
};

static enum SystemMode current_mode = MODE_NORMAL;
static enum LightColor state_road1 = COLOR_RED;
static enum LightColor state_road2 = COLOR_GREEN;

static int red_time = 5;
static int yellow_time = 2;
static int green_time = 3;

static int temp_red_time = 5;
static int temp_yellow_time = 2;
static int temp_green_time = 3;

static int count_road1 = 0;
static int count_road2 = 0;

void fsm_init(void) {
    current_mode = MODE_NORMAL;
    state_road1 = COLOR_RED;
    state_road2 = COLOR_GREEN;
    count_road1 = red_time;
    count_road2 = green_time;

    setTrafficRoad1(1, 0, 0);
    setTrafficRoad2(0, 0, 1);

    updateLedBuffer(count_road1, count_road2);
    setTimerTraffic(1000);
}

void fsm_traffic_normal(void) {
    if (timer_traffic_flag == 1) {
        setTimerTraffic(1000);
        count_road1--;
        count_road2--;

        // Chuyển trạng thái cho Đường 1
        if (count_road1 <= 0) {
            if (state_road1 == COLOR_RED) {
                state_road1 = COLOR_GREEN;
                count_road1 = green_time;
                setTrafficRoad1(0, 0, 1);
            } else if (state_road1 == COLOR_GREEN) {
                state_road1 = COLOR_YELLOW;
                count_road1 = yellow_time;
                setTrafficRoad1(0, 1, 0);
            } else {
                state_road1 = COLOR_RED;
                count_road1 = red_time;
                setTrafficRoad1(1, 0, 0);
            }
        }

        // Chuyển trạng thái cho Đường 2
        if (count_road2 <= 0) {
            if (state_road2 == COLOR_GREEN) {
                state_road2 = COLOR_YELLOW;
                count_road2 = yellow_time;
                setTrafficRoad2(0, 1, 0);
            } else if (state_road2 == COLOR_YELLOW) {
                state_road2 = COLOR_RED;
                count_road2 = red_time;
                setTrafficRoad2(1, 0, 0);
            } else {
                state_road2 = COLOR_GREEN;
                count_road2 = green_time;
                setTrafficRoad2(0, 0, 1);
            }
        }

        updateLedBuffer(count_road1, count_road2);
    }
}

void fsm_for_input_processing(void) {
    // Xử lý Button 1 (PB0): Chuyển Mode (Mode 1 -> 2 -> 3 -> 4 -> 1)
    if (is_button_pressed(0)) {
        if (current_mode == MODE_NORMAL) {
            current_mode = MODE_MODIFY_RED;
            // Bắt đầu phiên chỉnh sửa: sao chép giá trị hiện tại sang biến tạm
            temp_red_time = red_time;
            temp_yellow_time = yellow_time;
            temp_green_time = green_time;
            turnOffAllTrafficLeds();
            setTimerBlink(250);
            updateLedBuffer(temp_red_time, 2);
        } else if (current_mode == MODE_MODIFY_RED) {
            current_mode = MODE_MODIFY_AMBER;
            turnOffAllTrafficLeds();
            setTimerBlink(250);
            updateLedBuffer(temp_yellow_time, 3);
        } else if (current_mode == MODE_MODIFY_AMBER) {
            current_mode = MODE_MODIFY_GREEN;
            turnOffAllTrafficLeds();
            setTimerBlink(250);
            updateLedBuffer(temp_green_time, 4);
        } else if (current_mode == MODE_MODIFY_GREEN) {
            // Chuyển từ Mode 4 về Mode 1 mà không nhấn Save: huỷ toàn bộ các thay đổi
            fsm_init();
            return;
        }
        return;
    }

    switch (current_mode) {
        case MODE_NORMAL:
            fsm_traffic_normal();
            break;

        case MODE_MODIFY_RED:
            if (timer_blink_flag == 1) {
                setTimerBlink(250);
                toggleRedLeds();
            }
            if (is_button_pressed(1)) {
                temp_red_time++;
                if (temp_red_time > 99) temp_red_time = 1;
                updateLedBuffer(temp_red_time, 2);
            }
            if (is_button_pressed(2)) {
                // Nhấn Save (PB2): lưu lại toàn bộ các mode đã chỉnh sửa nhưng giữ nguyên trạng thái hiện tại
                red_time = temp_red_time;
                yellow_time = temp_yellow_time;
                green_time = temp_green_time;
            }
            break;

        case MODE_MODIFY_AMBER:
            if (timer_blink_flag == 1) {
                setTimerBlink(250);
                toggleYellowLeds();
            }
            if (is_button_pressed(1)) {
                temp_yellow_time++;
                if (temp_yellow_time > 99) temp_yellow_time = 1;
                updateLedBuffer(temp_yellow_time, 3);
            }
            if (is_button_pressed(2)) {
                // Nhấn Save (PB2): lưu lại toàn bộ các mode đã chỉnh sửa nhưng giữ nguyên trạng thái hiện tại
                red_time = temp_red_time;
                yellow_time = temp_yellow_time;
                green_time = temp_green_time;
            }
            break;

        case MODE_MODIFY_GREEN:
            if (timer_blink_flag == 1) {
                setTimerBlink(250);
                toggleGreenLeds();
            }
            if (is_button_pressed(1)) {
                temp_green_time++;
                if (temp_green_time > 99) temp_green_time = 1;
                updateLedBuffer(temp_green_time, 4);
            }
            if (is_button_pressed(2)) {
                // Nhấn Save (PB2): lưu lại toàn bộ các mode đã chỉnh sửa nhưng giữ nguyên trạng thái hiện tại
                red_time = temp_red_time;
                yellow_time = temp_yellow_time;
                green_time = temp_green_time;
            }
            break;
    }
}
