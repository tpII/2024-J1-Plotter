#include "FSM.h"

void FSM_update();

typedef enum {STANDBY, AUTO_DRAWING, MANUAL_CONTROL} state_type;
state_type state = STANDBY; 

static bool new_state = 1;

//Estos flags que pertenezcan donde corresponda
volatile bool flag_drawing= false;
volatile bool flag_manual= false;

void MEF_Update()
{	
	switch ( state )
	{
		case STANDBY:
			if (new_state == 1)
			{
        //Acciones de inicio
        new_state= 0;
			}
      
      //Cambia el estado
      if (flag_drawing)   //En vez de un flag podría ser una función que chequee la "casilla"
      {
        state = AUTO_DRAWING;
        new_state = 1; 
      }
      else if (flag_manual)
      {
        state= MANUAL_CONTROL;
        new_state = 1;
      }

		break;
		
		case AUTO_DRAWING:
      if (new_state == 1)
      {
        //Acciones de inicio
        new_state= 0;
      }
      ARM_update();
      if (!flag_drawing)
      {
        state= STANDBY;
        new_state = 1;
      }
		break;

		case MANUAL_CONTROL:
      if (new_state == 1)
      {
        //Acciones de inicio
        new_state= 0;
      }
      JOYSTICK_update();
      if (!flag_manual)
      {
        state = STANDBY;
        new_state = 1; 
      }     
		break;
		
		default:
		break;
	}
	
}
