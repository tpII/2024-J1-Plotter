#include "FSM.h"

void FSM_update();

typedef enum {STANDBY, AUTO_DRAWING, MANUAL_CONTROL} state_type;
state_type state = STANDBY; 

static bool new_state = 1;         

void MEF_Update()
{	
	switch ( state )
	{
		case STANDBY:
			if (new_state == 1)
			{
            //Inicializa el estado
			}
      
      //Cambia el estado
      if (false)
      {
        state = AUTO_DRAWING;
        new_state = 1; 
      }

		break;
		
		case AUTO_DRAWING:
      if (false)
      {
        state = STANDBY;
        new_state = 1; 
      }     
		break;

		case MANUAL_CONTROL:
      if (false)
      {
        state = STANDBY;
        new_state = 1; 
      }     
		break;
		
		default:
		break;
	}
	
}
