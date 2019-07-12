#include "HardwareConfig.h"
#include "Tasks.h"
//#include "FrskyD.ino"
//#include "FrskyX.h"
#include "FrskyCommon.h"
#include "ADC.h"


#include "settings_eeprom.h"

//#define USE_BETAFLIGHT_BIND_INFO // use bind info dumped from betaflight, adjust variables in FrskyCommon.h aswell. 

#define INC_BUTTONS // define if button (keypad button) code is to be included

uint16_t ReadFrSky_2way();

//#define DEBUG_SERIAL

uint32_t lastcallback = 0;
uint32_t nextcallbackdelay = 0;

#ifdef DEBUG_SERIAL
uint32_t nextDebugSerialPrintTime = 0;
uint16_t intervalDebugSerialPrintTime = 1000; // ms
#endif

void setup() {

  EepromSettings.setup();
  EepromSettings.defaults();

#ifdef OLED
  oledSetup();
#endif

#ifdef DEBUG_SERIAL
  Serial.begin(115200);
  Serial.println("Booting....");

#endif

  InitHardwarePins();
  InitADCtimer();

  //InitFailsafe();
  //option = FORCE_FRSKYX_TUNING;

//  BIND_IN_PROGRESS;

  RX_num = 00;
  MProtocol_id_master = ESP.getEfuseMac();//0x12345678;

  MProtocol_id = RX_num + MProtocol_id_master;

#ifdef USE_BETAFLIGHT_BIND_INFO
  int frsky_spi_tx_id[2] = {111, 222};
  MProtocol_id = frsky_spi_tx_id[1] * 256 + frsky_spi_tx_id[0];
#endif
  
  set_rx_tx_addr(MProtocol_id);

  StartFrskyD();

  BIND_IN_PROGRESS;


}


void loop() {

#ifdef DEBUG_SERIAL
  if (nextDebugSerialPrintTime < millis()) {
    nextDebugSerialPrintTime += intervalDebugSerialPrintTime;

    Serial.println(state);

    for (int i = 0; i < 8; i++) {
      Serial.print("Channel_data ");
      Serial.print(i);
      Serial.print(" = ");
      Serial.println(Channel_data[i]);
      
      Serial.print("ADCvalues ");
      Serial.print(i);
      Serial.print(" = ");
      Serial.println(ADCvalues[i]);
    }
    Serial.println("");
  }
#endif

}
