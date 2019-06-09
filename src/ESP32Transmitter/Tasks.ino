#include "Tasks.h"

void IRAM_ATTR HandleRFpacketSend_ISR()
{
  static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(timer_sem, &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken)
  {
    HandlePacketSend_StartTime_Prev = HandlePacketSend_StartTime;
    portYIELD_FROM_ISR(); // this wakes up the task immediately
    HandlePacketSend_StartTime = micros();
  }
}


void IRAM_ATTR SendTask(void *param) {
  while (1)
  {
    xSemaphoreTake(timer_sem, portMAX_DELAY);
    ///do task
    //Serial.println(micros() - HandlePacketSend_StartTime);
    //Serial.print(CallBackTime_Prev);
    //Serial.print(" - ");
    //Serial.println(HandlePacketSend_StartTime - HandlePacketSend_StartTime_Prev);

    int16_t callbacktime = remote_callback();

    CallBackTime_Prev = callbacktime;

    timerAlarmWrite(timer_SendTask, callbacktime, true);

  }
}

void StartSendTask()
{
  timer_sem = xSemaphoreCreateBinary();
  xTaskCreate(
    SendTask,          /* Task function. */
    "SendTask",        /* String with name of task. */
    2048,                 /* Stack size in words. */
    NULL,                 /* Parameter passed as input of the task */
    20,                   /* Priority of the task. */
    &SendTask_handle); /* Task handle. */

  timer_SendTask = timerBegin(1, 80, true);
  timerAttachInterrupt(timer_SendTask, &HandleRFpacketSend_ISR, true);
  timerAlarmWrite(timer_SendTask, 1000000, true);
  timerAlarmEnable(timer_SendTask);
}

void StartFrskyX() {
  initFrSkyX();
  remote_callback = ReadFrSkyX;
}

void StartFrskyD() {
  initFrSky_2way();
  remote_callback = ReadFrSky_2way;
}

void StartButtonTask() {
}
