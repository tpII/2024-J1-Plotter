#include "headers/scheduler.h"

Ticker msTicker;

// Flags utilizadas por el scheduler
volatile bool flag_LED = false;
volatile bool flag_MQTT = false;
volatile bool flag_FSM = false;

// Funcion del TICK
void onTick() 
{
  static uint8_t counter = 0;
  counter++;

  if (counter % 2 == 0) // cada 20ms
  {
    flag_FSM = true; 
  }

  if (counter % 10 == 0) //cada 100ms
  {
    flag_MQTT = true;
    flag_LED = true;
  }

  if (counter >= 100) //cada 1sec
  {
    counter = 0;
  }
}

void SCHEDULER_init()
{
  pinMode(2, OUTPUT);  //Debugging
  msTicker.attach(0.01, onTick);  // 10 ms
}

//Ejecuta funciones en funcion al estado de los flags
void SCHEDULER_dispatch_tasks() 
{
  if (flag_FSM) 
  {
    FSM_update();
    flag_FSM = 0;
  } 

  if (flag_MQTT)
  {
    MQTT_update();
    flag_MQTT = 0;
  }

  if (flag_LED)    
  {
    //Indica si se esta recibiendo informacion de la interfaz web (x tiempo desde el ultimo 'ping')
    if (MQTT_ping_status()) 
      { digitalWrite (2,HIGH); }
    else 
      { digitalWrite (2,LOW); }

    flag_LED = 0;
  } 
     
}
