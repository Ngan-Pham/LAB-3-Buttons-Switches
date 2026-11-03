#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "main.h"

void display7SEG(int num);
void update7SEG(int index);
void updateLedBuffer(int val1, int val2);

void setTrafficRoad1(int red, int yellow, int green);
void setTrafficRoad2(int red, int yellow, int green);
void toggleRedLeds(void);
void toggleYellowLeds(void);
void toggleGreenLeds(void);
void turnOffAllTrafficLeds(void);

#endif /* INC_LED_DISPLAY_H_ */
