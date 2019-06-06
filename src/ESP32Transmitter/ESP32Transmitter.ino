#include "HardwareConfig.h"
#include "Tasks.h"
//#include "FrskyD.ino"
//#include "FrskyX.h"
#include "FrskyCommon.h"
#include "ADC.h"




uint16_t ReadFrSky_2way();

#define DEBUG_SERIAL 1

uint32_t lastcallback = 0;
uint32_t nextcallbackdelay = 0;



void setup() {

#ifdef OLED
  oledSetup();
#endif

  Serial.begin(115200);
  Serial.println("Booting....");
  InitHardwarePins();
  InitADCtimer();

  //InitFailsafe();
  //option = FORCE_FRSKYX_TUNING;

  RX_num = 2;
  MProtocol_id_master = 0x12345678;

  MProtocol_id = RX_num + MProtocol_id_master;
  set_rx_tx_addr(MProtocol_id);
  //BIND_IN_PROGRESS;

  //initFrSky_2way();
  //initFrSkyX();

  //Channel_data = 1750;
  //protocol_init();
  remote_callback = ReadFrSky_2way; 
  StartSendTask();
  

}


void loop() {
  //delay(10);
  //  if (micros() > nextcallbackdelay + lastcallback) {
  Channel_data[5] = ADCvalues[0];
  //Serial.println(Channel_data[5]);
  //    //    Serial.print(micros() - lastcallback);
  //    //    Serial.print(" - ");
  //    //    Serial.println(nextcallbackdelay);
  //
  //
  //    nextcallbackdelay = (uint16_t)ReadFrSky_2way();
  //    lastcallback = micros();
  //    //Serial.println(".");

  //    Channel_data[5] = Channel_data[5] + 1;
  //    if (Channel_data[5] == 2000) {
  //      Channel_data[5] = 0;
  //    }

  //Channel_data[5] = random(1000, 2000);

}
