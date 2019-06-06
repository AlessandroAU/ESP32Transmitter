#pragma once

#include <SPI.h>

#define ADC1 ADC1_CHANNEL_0
#define ADC2 ADC1_CHANNEL_3
#define ADC3 ADC1_CHANNEL_6
#define ADC4 ADC1_CHANNEL_7
#define ADC5 ADC1_CHANNEL_4
#define ADC6 ADC1_CHANNEL_5

#define ADC1_GPIO 36
#define ADC2_GPIO 39
#define ADC3_GPIO 34
#define ADC4_GPIO 35
#define ADC5_GPIO 32
#define ADC6_GPIO 33

#define TWO_POS_SWITCH_ONE 25
#define TWO_POS_SWITCH_TWO 26
#define TWO_POS_SWITCH_THREE 27
  
#define BEEPER 14

#define MOSI 23
#define MISO 19
#define SCK 18


/////////CC2500///////////////////////////////////////////
#define CC25_LNA_pin 2
#define CC25_PA_pin 4
#define CC25_CSn_pin  5
#define CC25_CSn_active digitalWrite(CC25_CSn_pin,LOW)  //CS pin is active low
#define CC25_CSn_inactive digitalWrite(CC25_CSn_pin,HIGH)
#define CC25_PA_on digitalWrite(CC25_PA_pin,HIGH)
#define CC25_PA_off digitalWrite(CC25_PA_pin,LOW)
#define CC25_LNA_on digitalWrite(CC25_LNA_pin,HIGH)
#define CC25_LNA_off digitalWrite(CC25_LNA_pin,LOW)
//////////////////////////////////////////////////////////


void InitHardwarePins() {

  pinMode(TWO_POS_SWITCH_ONE, INPUT_PULLDOWN);
  pinMode(TWO_POS_SWITCH_TWO, INPUT_PULLDOWN);
  pinMode(TWO_POS_SWITCH_THREE, INPUT_PULLDOWN);
  
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(CC25_CSn_pin, OUTPUT);
  pinMode(CC25_PA_pin, OUTPUT);
  pinMode(CC25_LNA_pin, OUTPUT);


  //pinMode(MISO, INPUT);
  pinMode(BEEPER, OUTPUT);
  digitalWrite(BEEPER, LOW);
  digitalWrite(CC25_PA_pin, HIGH);


  SPI.setFrequency(5000000);
  SPI.begin(SCK, MISO, MOSI, -1);
}

void SPI_Write(uint8_t command)
{
  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(command);
  SPI.endTransaction();
}

uint8_t SPI_Read(void)
{
  uint8_t val;
  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
  val = SPI.transfer(0x00);
  SPI.endTransaction();
  return val;
}
