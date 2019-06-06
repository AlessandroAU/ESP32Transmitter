////Functions to Read RSSI from ADCs//////
#include <driver/adc.h>
#include <driver/timer.h>
#include "HardwareConfig.h"
//#include "Comms.h"
//#include <Wire.h>
//#include <Adafruit_INA219.h>
//#include "settings_eeprom.h"
#include "ADC.h"
//#include "Timer.h"
#include "UDP.h"

//Timer ina219Timer = Timer(1000);

//extern RXADCfilter_ RXADCfilter;
//extern ADCVBATmode_ ADCVBATmode;
//extern byte NumRecievers;


FilterBeLp2Slow FilterVBAT; //slow filter for VBAT readings

enum RXADCfilter_ {LPF_10Hz, LPF_20Hz, LPF_50Hz, LPF_100Hz};
RXADCfilter_ RXADCfilter = LPF_100Hz;

uint32_t ADCstartMicros;
uint32_t ADCfinishMicros;
uint16_t ADCcaptime;

uint32_t LastADCcall;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

SemaphoreHandle_t xBinarySemaphore;

bool BeginADCReading = false;
bool ADCreadingBusy = false;
byte currentADCpin = 0;

void ConfigureADC() {

  adc1_config_width(ADC_WIDTH_BIT_10);

  adc1_config_channel_atten(ADC1, ADC_ATTEN_6db);
  adc1_config_channel_atten(ADC2, ADC_ATTEN_6db);
  adc1_config_channel_atten(ADC3, ADC_ATTEN_6db);
  adc1_config_channel_atten(ADC4, ADC_ATTEN_6db);
  adc1_config_channel_atten(ADC5, ADC_ATTEN_6db);
  adc1_config_channel_atten(ADC6, ADC_ATTEN_6db);

}

void IRAM_ATTR ADCread( void * pvParameters ) {

  Serial.println("ADC Reading Task created...");

  while (1) {

    xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );
    uint32_t now = micros();
    //Serial.print(now - LastADCcall);
    //Serial.print(",");
    ADCstartMicros = now;
    LastADCcall = now;

    adcAttachPin(ADC1_GPIO);
    adcStart(ADC1_GPIO);
    while (adcBusy(ADC1_GPIO)) {
      NOP();
    }
    ADCReadingsRAW[0] = adcEnd(ADC1_GPIO);

    adcAttachPin(ADC2_GPIO);
    adcStart(ADC2_GPIO);
    while (adcBusy(ADC2_GPIO)) {
      NOP();
    }
    ADCReadingsRAW[1] = adcEnd(ADC2_GPIO);
    
    adcAttachPin(ADC3_GPIO);
    adcStart(ADC3_GPIO);
    while (adcBusy(ADC3_GPIO)) {
      NOP();
    }
    ADCReadingsRAW[2] = adcEnd(ADC3_GPIO);


    adcAttachPin(ADC4_GPIO);
    adcStart(ADC4_GPIO);
    while (adcBusy(ADC4_GPIO)) {
      NOP();
    }
    ADCReadingsRAW[3] = adcEnd(ADC4_GPIO);


    adcAttachPin(ADC5_GPIO);
    adcStart(ADC5_GPIO);
    while (adcBusy(ADC5_GPIO)) {
      NOP();
    }
    ADCReadingsRAW[4] = adcEnd(ADC5_GPIO);


    adcAttachPin(ADC6_GPIO);
    adcStart(ADC6_GPIO);
    while (adcBusy(ADC6_GPIO)) {
      NOP();
    }
    ADCReadingsRAW[5] = adcEnd(ADC6_GPIO);

    // Applying calibration
    //    if (!isCurrentlyCalibrating) {
    //      for (uint8_t i = 0; i < NumRecievers; i++) {
    //        uint16_t rawRSSI = constrain(ADCReadingsRAW[i], EepromSettings.RxCalibrationMin[i], EepromSettings.RxCalibrationMax[i]);
    //        ADCReadingsRAW[i] = map(rawRSSI, EepromSettings.RxCalibrationMin[i], EepromSettings.RxCalibrationMax[i], 800, 2700); // 800 and 2700 are about average min max raw values
    //      }
    //    }
    switch (RXADCfilter) {

      case LPF_10Hz:
        for (int i = 0; i < 6; i++) {
          ADCvalues[i] = Filter_10HZ[i].step(ADCReadingsRAW[i]);
        }
        break;

      case LPF_20Hz:
        for (int i = 0; i < 6; i++) {
          ADCvalues[i] = Filter_20HZ[i].step(ADCReadingsRAW[i]);
        }
        break;

      case LPF_50Hz:
        for (int i = 0; i < 6; i++) {
          ADCvalues[i] = Filter_50HZ[i].step(ADCReadingsRAW[i]);
        }
        break;

      case LPF_100Hz:
        for (int i = 0; i < 6; i++) {
          ADCvalues[i] = Filter_100HZ[i].step(ADCReadingsRAW[i]);
        }
        break;
    }

    for (int i = 0; i < 5; i++) {
      if (ADCvalues[i] >= EepromSettings.ADCvaluesMid[i]) {
        Channel_data[i] = (map(ADCvalues[i], EepromSettings.ADCvaluesMid[i], EepromSettings.ADCvaluesMax[i], 1500, 2000));
      } else {
        Channel_data[i] = (map(ADCvalues[i], EepromSettings.ADCvaluesMin[i], EepromSettings.ADCvaluesMid[i], 1000, 1499));
      }
    }
    
    Channel_data[5] = digitalRead(25) == HIGH ? 2000 : 1000;
    Channel_data[6] = digitalRead(26) == HIGH ? 2000 : 1000;
    Channel_data[7] = digitalRead(27) == HIGH ? 2000 : 1000;

    //    switch (ADCVBATmode) {
    //      case ADC_CH5:
    //        VbatReadingSmooth = ADCvalues[4];
    //        break;
    //      case ADC_CH6:
    //        VbatReadingSmooth = ADCvalues[5];
    //        break;
    //    }
    //ADCcaptime = micros() - ADCstartMicros;
    // Serial.println(ADCcaptime);
  }
}


void StartADCread() {
  Serial.println("Starting ADC reading task on core 1");
  xTaskCreatePinnedToCore(
    ADCread,   /* Function to implement the task */
    "ADCreader", /* Name of the task */
    10000,      /* Stack size in words */
    NULL,       /* Task input parameter */
    15,          /* Priority of the task */
    NULL,
    0);       /* Task handle. */
  //1);  /* Core where the task should run */
}

void IRAM_ATTR readADCs() {
  adcLoopCounter++;

  static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  /* un-block the interrupt processing task now */
  xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );
  if ( xHigherPriorityTaskWoken) {
    portENTER_CRITICAL_ISR(&timerMux);
    portYIELD_FROM_ISR(); // this wakes up sample_timer_task immediately
    portEXIT_CRITICAL_ISR(&timerMux);
  }
}

void InitADCtimer() {


  xBinarySemaphore = xSemaphoreCreateBinary();
  StartADCread();

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &readADCs, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);

}

void StopADCtimer() {
  //timerAttachInterrupt(timer, NULL, true); //This doesn't work, not sure why.
}
