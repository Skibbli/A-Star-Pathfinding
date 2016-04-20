#ifndef LEVEL_H
#define LEVEL_H

#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "allegro5\allegro.h"
#include <allegro5\allegro_font.h>
#include <string>
#include "Gamestate.h"

class GamestateManager;

class Level : public Gamestate
{
	public:
		// Constructor and destructor
		Level(GamestateManager *_stateManager);
		~Level();

		// Inherited functions to be defined by this class
		virtual bool Update();
		virtual void Render();

	private:
		int m_currTileType;
		int m_mapWidth, m_mapHeight;

		bool m_mouseDown, m_paused, m_enemiesActive;

		GamestateManager *m_stateManager;

		Map *m_map;
		Player *m_player;
		std::vector<Enemy*> m_enemies;

		std::string m_activeTileType;
		std::string m_algoMessage;

		ALLEGRO_EVENT m_event;
		ALLEGRO_EVENT_QUEUE *m_eventQueue;
		ALLEGRO_FONT *m_font;
};

#endif