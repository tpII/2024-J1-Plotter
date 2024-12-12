#include "headers/arm_controller.h"
#include "headers/drawing_module.h"
#include "headers/MQTT.h"
#include "headers/joystick_controller.h"
#include "headers/scheduler.h"
#include "headers/FSM.h"

//Pines para activar el modo control manual
#define BRIDGE_PIN1 27
#define BRIDGE_PIN2 25

void setup() 
{
  Serial.begin(115200);

  //Inicializa el joystick si se conectan los pines
  pinMode(BRIDGE_PIN1, INPUT_PULLDOWN);
  pinMode(BRIDGE_PIN2, OUTPUT);
  digitalWrite(BRIDGE_PIN2, HIGH);
  if (digitalRead(BRIDGE_PIN1) == 1) 
  {
    Serial.println("Inicializando modo manual");
    JOYSTICK_init(); 
  } 
  else 
  {
    Serial.println("Inicializando MQTT");
    MQTT_init(); 
  }
  digitalWrite(BRIDGE_PIN2, LOW);

  DRAWING_MODULE_init();
  SCHEDULER_init();
  Serial.println("Setup Complete");
}

void loop() 
{
  SCHEDULER_dispatch_tasks();
}
