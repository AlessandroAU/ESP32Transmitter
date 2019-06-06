#pragma once 

struct EepromSettingsStruct {
  uint16_t eepromVersionNumber;
  int ADCvaluesMid[8];
  int ADCvaluesMin[8];
  int ADCvaluesMax[8];

  void setup();
  void load();
  void save();
  void defaults();
};

const struct {
  uint16_t eepromVersionNumber = EEPROM_VERSION_NUMBER;

  int ADCvaluesMid[8] = {1662, 1662, 1662, 1662, 1662, 1662, 1662, 1662};
  int ADCvaluesMin[8] =    {96,   96,   96,   96,   96,   96,   96,   96};
  int ADCvaluesMax[8] =    {3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400};
} EepromDefaults;

extern EepromSettingsStruct EepromSettings;
