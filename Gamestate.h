#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

class GamestateManager;

//Base class that all other Gamestates derive from
//e.g. Menu, Table etc
class Gamestate
{
	public:
		//Constructor and Destructor. Virtual gamestate to ensure
		//derived classes define their own destructors
		Gamestate();
		virtual ~Gamestate();
		
		//Update and render pure virtual functions that the derived
		//classes must define themselves
		virtual bool Update() = 0;
		virtual void Render() = 0;

	protected:
		bool gameRunning;

		//Allegro variables common to all Gamestates
		ALLEGRO_BITMAP *m_screen;
		ALLEGRO_BITMAP *m_bgImage;

		//Pointer to the gamestate manager that is common to all Gamestates.
		//Each derived class takes in a pointer to the state manager in the constructor
		GamestateManager *m_stateManager;

	private:
		
};

#endif