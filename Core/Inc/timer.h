#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

#define TIMER_CYCLE 10 // Chu kỳ ngắt phần cứng (10ms)

extern int timer_traffic_flag;
extern int timer_7seg_flag;
extern int timer_blink_flag;

void setTimerTraffic(int duration_ms);
void setTimer7Seg(int duration_ms);
void setTimerBlink(int duration_ms);

void timerRun(void);

#endif /* INC_TIMER_H_ */
