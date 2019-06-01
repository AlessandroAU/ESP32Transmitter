static SemaphoreHandle_t timer_sem;

hw_timer_t * timer_SendTask = NULL;

//hw_timer_t SX127xDriver::timer = NULL;
portMUX_TYPE timerMux_SendTask = portMUX_INITIALIZER_UNLOCKED;

TaskHandle_t SendTask_handle = NULL;

uint32_t HandlePacketSend_StartTime = 0;
uint32_t HandlePacketSend_StartTime_Prev = 0;

uint32_t CallBackTime_Prev = 0;


void IRAM_ATTR HandlePacketSend_ISR()
{
  static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(timer_sem, &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken)
  {
    HandlePacketSend_StartTime_Prev = HandlePacketSend_StartTime;
    portYIELD_FROM_ISR(); // this wakes up sample_timer_task immediately
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

    int16_t callbacktime = ReadFrSky_2way();
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

  initFrSky_2way();
  state = FRSKY_BIND_DONE;
  delay(200);
  timer_SendTask = timerBegin(1, 80, true);
  timerAttachInterrupt(timer_SendTask, &HandlePacketSend_ISR, true);
  timerAlarmWrite(timer_SendTask, 1000000, true);
  timerAlarmEnable(timer_SendTask);
}
