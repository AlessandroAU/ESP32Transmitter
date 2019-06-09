#pragma once
#include "Tasks.h"

void cloneFrskyMaster() {
  if ( xHandle != NULL )
  {
    vTaskDelete(SendTask_handle);
  }

  int len = CC2500_ReadReg(CC2500_3B_RXBYTES | CC2500_READ_BURST) & 0x7F;
  if (len && (len <= (0x0E + 3)))     //Telemetry frame is 17
  {
    packet_count = 0;
    CC2500_ReadData(pkt, len);
  }
