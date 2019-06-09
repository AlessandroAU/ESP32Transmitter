#include "HardwareConfig.h"

/////need to debounce the buttons, here are some vars to do it///////
uint32_t button1_lastpressed = 0;
uint32_t button2_lastpressed = 0;
uint32_t button3_lastpressed = 0;

uint32_t button1_lastpressedLong = 0;
uint32_t button2_lastpressedLong = 0;

bool button1_prevState = true; //active high, therefore true as default.
bool button2_prevState = true; //active high, therefore true as default.
bool button3_prevState = true; //active high, therefore true as default.

bool button1_isDown = false; //is the button currently being held down?
bool button1_isDownLong = false; //is the button currently being held down?

bool button2_isDown = false; //is the button currently being held down?
bool button2_isDownLong = false; //is the button currently being held down?

uint16_t debounce_delay = 30; //how long the switch must change state to be considered
uint16_t longpress_delay = 400; //how long the switch must hold state to be considered a long press
uint16_t longpress_Interval = 400; //how long the switch must hold long state to be reapeated.
