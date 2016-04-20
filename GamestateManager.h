#ifndef GAMESTATE_MANAGER_H
#define GAMESTATE_MANAGER_H

#include "Gamestate.h"
#include <vector>

//This defines the Gamestate Manager that handles all of the state
//related functions. Through the gamestate manager update and rendering
//on all active states is called through a vector of gamestates.
class GamestateManager
{
	public:
		//Constructor and destructor
		GamestateManager();
		~GamestateManager();

		//Functions to change, add and remove gamestates
		void ChangeState( Gamestate *_state );
		void AddState( Gamestate *_state );
		void RemoveLastState();

		//Functions that call update and render on the active gamestate
		bool Update();
		void Render();

	private:
		std::vector < Gamestate* > m_gameStates;
};

#endif