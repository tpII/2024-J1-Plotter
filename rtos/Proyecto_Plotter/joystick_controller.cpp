
#include "headers/joystick_controller.h"

//Lecturas de X,Y cuando el joystick esta inactivo, y rango de valores ignorados
#define X_STANDBY 800
#define Y_STANDBY 3200
#define DEADZONE 300

static bool initialized_flag = false;

static int x_reading = X_STANDBY;
static int y_reading = Y_STANDBY;
static int stick_button_state = 0;

static bool shift = false;
static int shift_x = 0;
static int shift_y = 0;

void JOYSTICK_init()
{
  pinMode(SWPin, INPUT_PULLUP);

  analogSetWidth(10);        
  analogSetAttenuation(ADC_11db); 

  initialized_flag = true;
}

void JOYSTICK_update()
{
  if (initialized_flag)
    {
      shift = false;
      shift_x = 0;
      shift_y = 0;
  
      // Lee los valores del joystick
      x_reading = analogRead(VRxPin);
      y_reading = analogRead(VRyPin);
      stick_button_state = digitalRead(SWPin);  // Lee el boton (0 si se presiona, 1 si no)
      
      if (x_reading > X_STANDBY + 3*DEADZONE)
      {
        //Serial.println("Moviendo X+");
        shift_x = 1;
        shift = true;
      }

      if (x_reading < X_STANDBY - 2*DEADZONE)
      {
        //Serial.println("Moviendo X-");
        shift_x = -1;
        shift = true;
      }

      if (y_reading > Y_STANDBY + 2*DEADZONE)
      {
        //Serial.println("Moviendo Y-");
        shift_y = -1;
        shift = true;
      }

      if (y_reading < Y_STANDBY - 4*DEADZONE)
      {
        //Serial.println("Moviendo Y+");
        shift_y = 1;
        shift = true;
      }

      if (shift) ARM_shift_by(shift_x, shift_y);

      if (!stick_button_state)
      {
        //Serial.println("Stick Presionado");
        ARM_lift(!ARM_is_lifted());
      }
  }
}
