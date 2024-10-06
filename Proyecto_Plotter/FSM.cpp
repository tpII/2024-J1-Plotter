#include "FSM.h"

void FSM_update();

typedef enum {STATE_A, STATE_B} state_type;
state_type state = STATE_A; 

static bool new_state = 1;         

void MEF_Update()
{	
	switch ( state )
	{
		case STATE_A:
			if (new_state == 1)
			{
            //Inicializa el estado
			}
      
      //Cambia el estado
      if (false)
      {
        state = STATE_B;
        new_state = 1; 
      }

		break;
		
		case STATE_B:
      if (false)
      {
        state = STATE_A;
        new_state = 1; 
      }     
		break;
		
		default:
		break;
	}
	
}
