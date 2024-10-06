
#include "servo_controller.h"
#include "web_server.h"
#include "scheduler.h"

void setup() 
{
  Serial.begin(115200);
  SERVO_init();
  //WEB_SERVER_init();
  //SCHEDULER_init();

  Serial.println("Setup Complete");
}

void loop() 
{
  //Testing 
  //WEB_SERVER_update();
  //SERVO_update();

  //SCHEDULER_dispatch_tasks();
}
