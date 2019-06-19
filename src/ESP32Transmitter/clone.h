#pragma once
#include "Tasks.h"

#define NumConcurrentPkts 3 //number of concurrent binding packets needed before addr is considered 'good'

#define CloningTimeout 10000 //number of milliseconds before this cloning function times out
#define PacketLen 17

uint8_t HeaderFrskyD = {0x11, 0x03, 0x01 };
uint8_t PacketAddrs[NumConcurrentPkts][2] = {0};
uint8_t PacketNumCounter = 0;
uint8_t inData[PacketLen] = {0};

bool CompareAddrArray(uint8_t *Data) { //compares the packets to check we recv several packets with the same addr bytes
  bool result = true;
  for (int i; i < NumConcurrentPkts - 1; i++) {
    if (PacketAddrs[i][0] == PacketAddrs[i + 1][0] and PacketAddrs[i + 1][1] == PacketAddrs[i + 1][1]) {
      //do nothing if true
    } else {
      result = false;
      return result;
    }
    return result;
  }
}

bool CheckHeadersFrskyD(uint8_t *Data) { //check to see if the packet is an Frsky packet by the headers.
  bool result = true;
  for (int i; i < 3; i++) {
    if (Data[i] == !HeaderFrskyD[i]) {
      result = false;
      break;
    }
  }
  return result;
}

uint16_t BeginCloningFrskyDaddr() {

  if ( SendTask_handle != NULL )
  {
    vTaskDelete(SendTask_handle); // stop the RF sending task
  }

  CC2500_Strobe(CC2500_SIDLE);  // Go to idle...
  CC2500_WriteReg(CC2500_0A_CHANNR, 0x00);
  CC2500_WriteReg(CC2500_23_FSCAL3, 0x89);
  CC2500_Strobe(CC2500_SFRX);

  Serial.println("Begin Cloning....");

  StartTime = millis();

  while (millis() < StartTimew + CloningTimeout) {
    delay(10);
    int len = CC2500_ReadReg(CC2500_3B_RXBYTES | CC2500_READ_BURST) & 0x7F;

    if (len && (len <= (0x0E + 3)))     //Telemetry frame is 17 ????
    {
      packet_count = 0;
      CC2500_ReadData(inData, len);
      for (int i = 0; i < PacketLen; i++) {
        Serial.print(inData[i]);
      }
      Serial.println();
      if (CheckHeadersFrskyD(inData)) {

        Serial.println("Frsky D Header Found");
        PacketAddrs[PacketNumCounter][0] = inData[3];
        PacketAddrs[PacketNumCounter][1] = inData[4];

        if (PacketNumCounter == NumConcurrentPkts) {
          if (CheckHeadersFrskyD(inData)) {
            Serial.print("Got Address!: ")
            Serial.print(PacketAddrs[0][0], HEX);
            Serial.println(PacketAddrs[0][1], HEX);
            uint16_t addr = PacketAddrs[0][0] + (PacketAddrs[0][1] << 8);
            return addr; //returns the address that we detected 
          } else {
            PacketNumCounter = 0;
          }
        } else {
          PacketNumCounter++;
        }
      }
    }
  }
  return 0; // return of zero means that cloning has failed
}
