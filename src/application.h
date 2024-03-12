#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "buttons_config.h"
#include "McuDebounce.h"

void queueTask(void *pv); 
void Application_Init(void);
void Application_Deinit(void);
void Application_OnButtonEvent(BTN_Buttons_e button, McuDbnc_EventKinds kind);

#endif