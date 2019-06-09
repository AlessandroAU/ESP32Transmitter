#pragma once

#ifdef INC_BUTTONS

#include "HardwareConfig.h"

#define KEYPAD_UP 14
#define KEYPAD_DOWN 12
#define KEYPAD_RIGHT 13
#define KEYPAD_LEFT  15
#define KEYPAD_ENTER 0

TaskHandle_t buttonTask_Handle = NULL;

uint32_t ButtonSampleInterval = 0;
uint32_t ButtonsLastSampled = 0;

typedef void (*void_button_callback_function_t) (void);

void_button_callback_function_t CALLBACK_KEYPAD_UP_PRESSED = 0;
void_button_callback_function_t CALLBACK_KEYPAD_DOWN_PRESSED = 0;
void_button_callback_function_t CALLBACK_KEYPAD_RIGHT_PRESSED = 0;
void_button_callback_function_t CALLBACK_KEYPAD_LEFT_PRESSED = 0;
void_button_callback_function_t CALLBACK_KEYPAD_ENTER_PRESSED = 0;

void_button_callback_function_t CALLBACK_KEYPAD_UP_PRESSED_LONG = 0;
void_button_callback_function_t CALLBACK_KEYPAD_DOWN_PRESSED_LONG = 0;
void_button_callback_function_t CALLBACK_KEYPAD_RIGHT_PRESSED_LONG = 0;
void_button_callback_function_t CALLBACK_KEYPAD_LEFT_PRESSED_LONG = 0;
void_button_callback_function_t CALLBACK_KEYPAD_ENTER_PRESSED_LONG = 0;

uint16_t debounceDelay = 30; //how long the switch must change state to be considered
uint16_t longpressDelay = 400; //how long the switch must hold state to be considered a long press
uint16_t longpressInterval = 400; //how long the switch must hold long state to be reapeated.

/////need to debounce the buttons, here are some vars to do it///////
uint32_t KEYPAD_UP_LAST_PRESSED = 0;
uint32_t KEYPAD_DOWN_LAST_PRESSED = 0;
uint32_t KEYPAD_RIGHT_LAST_PRESSED = 0;
uint32_t KEYPAD_LEFT_LAST_PRESSED = 0;
uint32_t KEYPAD_ENTER_LAST_PRESSED = 0;

uint32_t KEYPAD_UP_LAST_PRESSED_LONG = 0;
uint32_t KEYPAD_DOWN_LAST_PRESSED_LONG = 0;
uint32_t KEYPAD_RIGHT_LAST_PRESSED_LONG = 0;
uint32_t KEYPAD_LEFT_LAST_PRESSED_LONG = 0;
uint32_t KEYPAD_ENTER_LAST_PRESSED_LONG = 0;

bool KEYPAD_UP_PREV_STATE = true; //active low, therefore true as default.
bool KEYPAD_DOWN_PREV_STATE = true; //active low, therefore true as default.
bool KEYPAD_RIGHT_PREV_STATE = true; //active low, therefore true as default.
bool KEYPAD_LEFT_PREV_STATE = true; //active low, therefore true as default.
bool KEYPAD_ENTER_PREV_STATE = true; //active low, therefore true as default.

bool KEYPAD_UP_IS_DOWN = false;
bool KEYPAD_DOWN_IS_DOWN = false;
bool KEYPAD_RIGHT_IS_DOWN = false;
bool KEYPAD_LEFT_IS_DOWN = false;
bool KEYPAD_ENTER_IS_DOWN = false;

bool KEYPAD_UP_IS_DOWNLONG = false;
bool KEYPAD_DOWN_IS_DOWNLONG = false;
bool KEYPAD_RIGHT_IS_DOWNLONG = false;
bool KEYPAD_LEFT_IS_DOWNLONG = false;
bool KEYPAD_ENTER_IS_DOWNLONG = false;

void initButtons();

void sampleButtonsTask(void *param);

void sampleButton_KEYPAD_UP();
void sampleButton_KEYPAD_DOWN();
void sampleButton_KEYPAD_RIGHT();
void sampleButton_KEYPAD_LEFT();
void sampleButton_KEYPAD_ENTER();

#endif
