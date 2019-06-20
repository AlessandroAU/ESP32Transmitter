#pragma once 

struct EepromSettingsStruct {
  uint16_t eepromVersionNumber;
  int ADCvaluesMid[8];
  int ADCvaluesMin[8];
  int ADCvaluesMax[8];
  int reverseChannel[8];

  void setup();
  void load();
  void save();
  void defaults();
};

const struct {
  uint16_t eepromVersionNumber = EEPROM_VERSION_NUMBER;

  int ADCvaluesMid[8] = {1755, 1807, 1728, 1778, 1662, 1662, 1662, 1662};
  int ADCvaluesMin[8] = {1080, 1072, 1072, 1132,   96,   96,   96,   96};
  int ADCvaluesMax[8] = {2494, 2512, 2510, 2465, 3400, 3400, 3400, 3400};
  int reverseChannel[8] = {0, 1, 0, 1, 1, 1, 1, 0};
} EepromDefaults;

extern EepromSettingsStruct EepromSettings;
