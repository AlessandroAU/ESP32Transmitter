#ifdef INC_BUTTONS
#include "buttons.h"

void initButtons() {
  pinMode(KEYPAD_UP, INPUT_PULLDOWN);
  pinMode(KEYPAD_DOWN, INPUT_PULLDOWN);
  pinMode(KEYPAD_RIGHT, INPUT_PULLDOWN);
  pinMode(KEYPAD_LEFT, INPUT_PULLDOWN);
  pinMode(KEYPAD_ENTER, INPUT_PULLDOWN);
}

void sampleButtonsTask() {
  uint32_t now = micros();

  sampleButton_KEYPAD_UP();
  sampleButton_KEYPAD_DOWN();
  sampleButton_KEYPAD_RIGHT();
  sampleButton_KEYPAD_LEFT();
  sampleButton_KEYPAD_ENTER();

  ButtonSampleInterval = now - ButtonsLastSampled;
  ButtonsLastSampled = now;
}

void sampleButton_KEYPAD_UP() {
  bool currState = digitalRead(KEYPAD_UP);
  uint32_t now = millis();

  if (currState == true and KEYPAD_UP_PREV_STATE == false) { //button release (rising edge), remember it's inversed for active low.
    if (KEYPAD_UP_IS_DOWN and (!KEYPAD_UP_IS_DOWNLONG)) {
      Serial.println("KEYPAD_UP pressed");
      CALLBACK_KEYPAD_UP_PRESSED();
    }
    KEYPAD_UP_IS_DOWN = false; //button has been released at some point
    KEYPAD_UP_IS_DOWNLONG = false;
  }
  //
  if (currState == false and KEYPAD_UP_PREV_STATE == true) { //falling edge, remember it's inversed
    KEYPAD_UP_LAST_PRESSED = millis();
  }

  if (currState == false and KEYPAD_UP_PREV_STATE == false and (millis() - KEYPAD_UP_LAST_PRESSED) > debounceDelay) {
    KEYPAD_UP_IS_DOWN = true;
  }

  if (KEYPAD_UP_IS_DOWN and ((millis() - KEYPAD_UP_LAST_PRESSED) > longpressDelay) and (millis() - KEYPAD_UP_LAST_PRESSED_LONG > longpressInterval)) {
    if (KEYPAD_UP_IS_DOWN) {
      Serial.println("KEYPAD_UP pressed long");
      KEYPAD_UP_LAST_PRESSED_LONG = now;
      KEYPAD_UP_IS_DOWNLONG = true;
      CALLBACK_KEYPAD_UP_PRESSED_LONG();
    }
  }
  KEYPAD_UP_PREV_STATE = currState;
}

void sampleButton_KEYPAD_DOWN() {
  bool currState = digitalRead(KEYPAD_DOWN);
  uint32_t now = millis();

  if (currState == true and KEYPAD_DOWN_PREV_STATE == false) { //button release (rising edge), remember it's inversed for active low.
    if (KEYPAD_DOWN_IS_DOWN and (!KEYPAD_DOWN_IS_DOWNLONG)) {
      Serial.println("KEYPAD_DOWN pressed");
      CALLBACK_KEYPAD_DOWN_PRESSED();
    }
    KEYPAD_DOWN_IS_DOWN = false; //button has been released at some point
    KEYPAD_DOWN_IS_DOWNLONG = false;
  }
  //
  if (currState == false and KEYPAD_DOWN_PREV_STATE == true) { //falling edge, remember it's inversed
    KEYPAD_DOWN_LAST_PRESSED = millis();
  }

  if (currState == false and KEYPAD_DOWN_PREV_STATE == false and (millis() - KEYPAD_DOWN_LAST_PRESSED) > debounceDelay) {
    KEYPAD_DOWN_IS_DOWN = true;
  }

  if (KEYPAD_DOWN_IS_DOWN and ((millis() - KEYPAD_DOWN_LAST_PRESSED) > longpressDelay) and (millis() - KEYPAD_DOWN_LAST_PRESSED_LONG > longpressInterval)) {
    if (KEYPAD_DOWN_IS_DOWN) {
      Serial.println("KEYPAD_DOWN pressed long");
      KEYPAD_DOWN_LAST_PRESSED_LONG = now;
      KEYPAD_DOWN_IS_DOWNLONG = true;
      CALLBACK_KEYPAD_DOWN_PRESSED_LONG();
    }
  }
  KEYPAD_DOWN_PREV_STATE = currState;
}

void sampleButton_KEYPAD_RIGHT() {
  bool currState = digitalRead(KEYPAD_RIGHT);
  uint32_t now = millis();

  if (currState == true and KEYPAD_RIGHT_PREV_STATE == false) { //button release (rising edge), remember it's inversed for active low.
    if (KEYPAD_RIGHT_IS_DOWN and (!KEYPAD_RIGHT_IS_DOWNLONG)) {
      Serial.println("KEYPAD_RIGHT pressed");
      CALLBACK_KEYPAD_RIGHT_PRESSED();
    }
    KEYPAD_RIGHT_IS_DOWN = false; //button has been released at some point
    KEYPAD_RIGHT_IS_DOWNLONG = false;
  }
  //
  if (currState == false and KEYPAD_RIGHT_PREV_STATE == true) { //falling edge, remember it's inversed
    KEYPAD_RIGHT_LAST_PRESSED = millis();
  }

  if (currState == false and KEYPAD_RIGHT_PREV_STATE == false and (millis() - KEYPAD_RIGHT_LAST_PRESSED) > debounceDelay) {
    KEYPAD_RIGHT_IS_DOWN = true;
  }

  if (KEYPAD_RIGHT_IS_DOWN and ((millis() - KEYPAD_RIGHT_LAST_PRESSED) > longpressDelay) and (millis() - KEYPAD_RIGHT_LAST_PRESSED_LONG > longpressInterval)) {
    if (KEYPAD_RIGHT_IS_DOWN) {
      Serial.println("KEYPAD_RIGHT pressed long");
      KEYPAD_RIGHT_LAST_PRESSED_LONG = now;
      KEYPAD_RIGHT_IS_DOWNLONG = true;
      CALLBACK_KEYPAD_RIGHT_PRESSED_LONG();
    }
  }
  KEYPAD_RIGHT_PREV_STATE = currState;
}

void sampleButton_KEYPAD_LEFT() {
  bool currState = digitalRead(KEYPAD_LEFT);
  uint32_t now = millis();

  if (currState == true and KEYPAD_LEFT_PREV_STATE == false) { //button release (rising edge), remember it's inversed for active low.
    if (KEYPAD_LEFT_IS_DOWN and (!KEYPAD_LEFT_IS_DOWNLONG)) {
      Serial.println("KEYPAD_LEFT pressed");
      CALLBACK_KEYPAD_LEFT_PRESSED();
    }
    KEYPAD_LEFT_IS_DOWN = false; //button has been released at some point
    KEYPAD_LEFT_IS_DOWNLONG = false;
  }
  //
  if (currState == false and KEYPAD_LEFT_PREV_STATE == true) { //falling edge, remember it's inversed
    KEYPAD_LEFT_LAST_PRESSED = millis();
  }

  if (currState == false and KEYPAD_LEFT_PREV_STATE == false and (millis() - KEYPAD_LEFT_LAST_PRESSED) > debounceDelay) {
    KEYPAD_LEFT_IS_DOWN = true;
  }

  if (KEYPAD_LEFT_IS_DOWN and ((millis() - KEYPAD_LEFT_LAST_PRESSED) > longpressDelay) and (millis() - KEYPAD_LEFT_LAST_PRESSED_LONG > longpressInterval)) {
    if (KEYPAD_LEFT_IS_DOWN) {
      Serial.println("KEYPAD_LEFT pressed long");
      KEYPAD_LEFT_LAST_PRESSED_LONG = now;
      KEYPAD_LEFT_IS_DOWNLONG = true;
      CALLBACK_KEYPAD_LEFT_PRESSED_LONG();
    }
  }
  KEYPAD_LEFT_PREV_STATE = currState;
}

void sampleButton_KEYPAD_ENTER() {
  bool currState = digitalRead(KEYPAD_ENTER);
  uint32_t now = millis();

  if (currState == true and KEYPAD_ENTER_PREV_STATE == false) { //button release (rising edge), remember it's inversed for active low.
    if (KEYPAD_ENTER_IS_DOWN and (!KEYPAD_ENTER_IS_DOWNLONG)) {
      Serial.println("KEYPAD_ENTER pressed");
      CALLBACK_KEYPAD_ENTER_PRESSED();
    }
    KEYPAD_ENTER_IS_DOWN = false; //button has been released at some point
    KEYPAD_ENTER_IS_DOWNLONG = false;
  }
  //
  if (currState == false and KEYPAD_ENTER_PREV_STATE == true) { //falling edge, remember it's inversed
    KEYPAD_ENTER_LAST_PRESSED = millis();
  }

  if (currState == false and KEYPAD_ENTER_PREV_STATE == false and (millis() - KEYPAD_ENTER_LAST_PRESSED) > debounceDelay) {
    KEYPAD_ENTER_IS_DOWN = true;
  }

  if (KEYPAD_ENTER_IS_DOWN and ((millis() - KEYPAD_ENTER_LAST_PRESSED) > longpressDelay) and (millis() - KEYPAD_ENTER_LAST_PRESSED_LONG > longpressInterval)) {
    if (KEYPAD_ENTER_IS_DOWN) {
      Serial.println("KEYPAD_ENTER pressed long");
      KEYPAD_ENTER_LAST_PRESSED_LONG = now;
      KEYPAD_ENTER_IS_DOWNLONG = true;
      CALLBACK_KEYPAD_ENTER_PRESSED_LONG();
    }
  }
  KEYPAD_ENTER_PREV_STATE = currState;
}
#endif
