#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <Arduino.h>
#include <Ticker.h>

void SCHEDULER_init();
void SCHEDULER_dispatch_tasks();

#endif