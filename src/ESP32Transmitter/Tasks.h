#pragma once
// Callback
typedef uint16_t (*void_function_t) (void);//pointer to a function with no parameters which return an uint16_t integer
void_function_t remote_callback = 0;
///////

static SemaphoreHandle_t timer_sem;

hw_timer_t * timer_SendTask = NULL;

portMUX_TYPE timerMux_SendTask = portMUX_INITIALIZER_UNLOCKED;

TaskHandle_t SendTask_handle = NULL;

uint32_t HandlePacketSend_StartTime = 0;
uint32_t HandlePacketSend_StartTime_Prev = 0;

uint32_t CallBackTime_Prev = 0;

void StartSendTask();
void StartFrskyX();
void StartFrskyD();

#ifdef INC_BUTTONS
void StartButtonTask():
#endif
