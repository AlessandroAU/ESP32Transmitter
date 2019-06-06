#include <EEPROM.h>
#include "settings_eeprom.h"

struct EepromSettingsStruct EepromSettings;

void EepromSettingsStruct::setup() {
  EEPROM.begin(512);
  this->load();
}

void EepromSettingsStruct::load() {
  EEPROM.get(0, *this);
#ifdef DEBUG_SERIAL
  Serial.println("EEPROM LOADED");
#endif

  if (this->eepromVersionNumber != EEPROM_VERSION_NUMBER) {
    this->defaults();
#ifdef DEBUG_SERIAL
    Serial.println("EEPROM DEFAULTS LOADED");
#endif
  }
}

void EepromSettingsStruct::save() {
  EEPROM.put(0, *this);
  EEPROM.commit();
#ifdef DEBUG_SERIAL
  Serial.println("EEPROM SAVED");
#endif
}

void EepromSettingsStruct::defaults() {
  memcpy_P(this, &EepromDefaults, sizeof(EepromDefaults));
  EEPROM.put(0, *this);
  EEPROM.commit();
}
