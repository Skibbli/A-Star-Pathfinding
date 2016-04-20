#include <iostream>
#include "AllegroInit.h"
#include "Level.h"
#include "GamestateManager.h"

int main()
{
	// Loads and initialises Allegro
	AllegroInit allegro;

	GamestateManager stateManager;

	// Adds the simulation to the list of game states. A state manager was used to allow
	// for multiple simulations on the same program but unfortunately this was not implemented
	stateManager.AddState(new Level(&stateManager));

	bool gameRunning = true;

	// Main game loop
	while(gameRunning)
	{
		al_get_next_event(allegro.m_eventQueue, &allegro.m_event);

		if(allegro.m_event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			gameRunning = false;

		// A timer is set to run at 30 FPS - this function triggers every time the
		// timer ticks, running the program at 30 FPS
		else if(allegro.m_event.type == ALLEGRO_EVENT_TIMER)
		{
			// Updates and draws the current state
			gameRunning = stateManager.Update();
			stateManager.Render();

			al_flip_display();
			al_clear_to_color(al_map_rgb(255, 255, 255));
		}		
	}

	return 0;
}