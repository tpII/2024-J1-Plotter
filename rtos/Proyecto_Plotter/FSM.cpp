#include "headers/FSM.h"

typedef enum {STANDBY, AUTO_DRAWING, MANUAL_CONTROL} state_type;
state_type state = STANDBY; 

void FSM_update()
{	
	switch ( state )
	{
		case STANDBY:
      SERVO_update();
      ARM_update();
      
      if (DRAWING_MODULE_is_drawing()) state = AUTO_DRAWING; 
      else if (MQTT_is_manual_mode()) state = MANUAL_CONTROL;
		break;
		
		case AUTO_DRAWING:
      SERVO_update();
      ARM_update();
      DRAWING_MODULE_update();

      if (!DRAWING_MODULE_is_drawing()) state = STANDBY;
		break;

		case MANUAL_CONTROL:
      SERVO_update();
      ARM_update();
      JOYSTICK_update();

      if (!MQTT_is_manual_mode()) state = STANDBY;
		break;
		
		default:
		break;
	}
	
}
