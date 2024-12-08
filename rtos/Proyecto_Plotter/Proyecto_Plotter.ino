#include "arm_controller.h"
#include "drawing_module.h"
#include "web_server.h"
#include "joystick_controller.h"
#include "scheduler.h"

void setup() 
{
  Serial.begin(115200);
  ARM_init();
  DRAWING_MODULE_init();
  WEB_SERVER_init();
  //JOYSTICK_init();
  SCHEDULER_init();
  Serial.println("Setup Complete");
}

void loop() 
{
  SCHEDULER_dispatch_tasks();
}
