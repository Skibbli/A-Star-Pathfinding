#include "Level.h"

// Constructor - initialises member variables
Level::Level(GamestateManager *_stateManager)
{
	m_mapWidth = 1000;
	m_mapHeight = 1000;
	m_map = new Map(m_mapWidth, m_mapHeight);
	m_player = new Player(m_map);

	m_enemies.push_back(new Enemy(glm::vec2(150.0f,850.0f), 300.0f, m_map, m_player));
	m_enemies.push_back(new Enemy(glm::vec2(450.0f,350.0f), 350.0f, m_map, m_player));
	m_enemies.push_back(new Enemy(glm::vec2(800.0f,250.0f), 400.0f, m_map, m_player));
	m_enemies.push_back(new Enemy(glm::vec2(100.0f,750.0f), 500.0f, m_map, m_player));
	m_enemies.push_back(new Enemy(glm::vec2(750.0f,850.0f), 200.0f, m_map, m_player));

	m_stateManager = _stateManager;

	m_currTileType = -1;
	m_mouseDown = false;
	m_paused = true;
	m_enemiesActive = false;

	m_activeTileType = "";
	m_algoMessage = "";
	m_font = al_load_font("Arial.ttf", 14, 0);

	// Creates the event queue used for getting keyboard input and registers the keyboard
	m_eventQueue = al_create_event_queue();
	al_register_event_source(m_eventQueue, al_get_keyboard_event_source());
	al_register_event_source(m_eventQueue, al_get_mouse_event_source());
}

// Destructor - cleans up necessary objects to prevent memory leaks
Level::~Level()
{
	delete m_map;
	delete m_player;
	for(Enemy* enemy : m_enemies)
	{
		delete(enemy);
	}
	m_enemies.clear();
	al_destroy_font(m_font);
}

// Update function that is called every game loop - processes player input and performs the
// necessary actions
bool Level::Update()
{
	// Gets the next event in the queue
	al_get_next_event(m_eventQueue, &m_event);

	// If the event is a keyboard button being pressed down then perform the relevant action
	if(m_event.type == ALLEGRO_EVENT_KEY_DOWN)
	{		
		switch(m_event.keyboard.keycode)
		{
			case ALLEGRO_KEY_ESCAPE: gameRunning = false;
				break;
			case ALLEGRO_KEY_1 :
				{
					m_currTileType = 0;
					m_activeTileType = "Grass";
				}
					break;
			case ALLEGRO_KEY_2:
				{
					m_currTileType = 1;
					m_activeTileType = "Dirt";
				}
					break;
			case ALLEGRO_KEY_3:
				{
					m_currTileType = 2;
					m_activeTileType = "Water";
				}
					break;
			case ALLEGRO_KEY_4:
				{
					m_currTileType = 3;
					m_activeTileType = "Hole";
				}
					break;
			case ALLEGRO_KEY_Q:
				{
					m_currTileType = -1;
					m_activeTileType = "";
				}
					break;
			case ALLEGRO_KEY_G:
				{
					m_map->ToggleGrid();
				}
					break;
			case ALLEGRO_KEY_V:
				{
					m_map->ToggleTileVals();
				}
					break;
			case ALLEGRO_KEY_S:
				{
					m_currTileType =  -2;
					m_activeTileType = "Start Point";
				}
					break;
			case ALLEGRO_KEY_E:
				{
					m_currTileType = -3;
					m_activeTileType = "End Point";
				}
					break;
			case ALLEGRO_KEY_C:
				{
					m_player->ClearPath();
				}
					break;
			case ALLEGRO_KEY_P:
				{
					m_paused = !m_paused;
				}
				break;
			case ALLEGRO_KEY_X:
				{
					m_map->ToggleDiags();
				}
				break;
			case ALLEGRO_KEY_A:
				{
					if(m_player->HasDestination())
					{
						m_map->UpdateEdgeList();
						m_player->ClearPath();
						m_player->RequestPath(0);
					}

					else
						m_algoMessage = "No end point specified";
				}
				break;
			case ALLEGRO_KEY_D:
				{
					if(m_player->HasDestination())
					{
						m_map->UpdateEdgeList();
						m_player->ClearPath();
						m_player->RequestPath(1);
					}

					else
						m_algoMessage = "No end point specified";
				}
				break;
			case ALLEGRO_KEY_Z:
				{
					m_enemiesActive = !m_enemiesActive;
					m_player->ClearPath();
					m_map->UpdateEdgeList();
					m_map->ResetMap();
				}
				break;
		}
	}

	// If the event is the mouse button being pressed toggle whether the mouse is pressed or not
	else if(m_event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		m_mouseDown = true;

	else if(m_event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		m_mouseDown = false;

	// If the mouse button is pressed and a tile type has been selected change the tile at the mouse position
	if(m_mouseDown && m_currTileType != -1)
	{
		if(m_event.mouse.x > 0 && m_event.mouse.x < m_mapWidth && m_event.mouse.y > 0 && m_event.mouse.y < m_mapHeight)
		{
			if(m_currTileType == -2)
			{
				m_player->SetPosition(glm::vec2(m_event.mouse.x, m_event.mouse.y));
				m_currTileType = -1;
				m_activeTileType = "";
			}

			else if(m_currTileType == -3)
			{
				m_player->SetDestination(glm::vec2(m_event.mouse.x, m_event.mouse.y));
				m_currTileType = -1;
				m_activeTileType = "";
			}

			else
				m_map->ChangeTile(m_event.mouse.x, m_event.mouse.y, m_currTileType);
		}
	}

	// Call the update function for the player and all enemies in the level if the simulation is not paused
	if(!m_paused)
	{
		m_player->Update();

		if(m_enemiesActive)
		{
			for(Enemy* enemy : m_enemies)
			{
				enemy->Update();
			}
		}
	}

	// Returns whether the game still needs to be run or not to the gamestate manager
	return gameRunning;
}

// Renders all necessary information to the screen and calls the render function of the map, player and enemies
void Level::Render()
{
	m_map->DrawMap();

	al_draw_textf(m_font, al_map_rgb(0,0,0), 1020, 20, 0, "Selected tile type: %s", m_activeTileType.c_str());
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 35, 0, "Press 1 to choose GRASS tile");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 50, 0, "Press 2 to choose ROAD tile");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 65, 0, "Press 3 to choose DIRT tile");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 80, 0, "Press 4 to choose MOUNTAIN tile");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 95, 0, "Press S to place the start point");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 110, 0, "Press E to place the end point");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 125, 0, "Press Q to cancel tile placement");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 140, 0, "Press ESC to quit the program");

	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 180, 0, "Press G to toggle the grid");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 195, 0, "Press V to toggle the tile costs");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 210, 0, "Press X to toggle allowing diagonal movement");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 225, 0, "Press Z to toggle enemies");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 240, 0, "Press P to pause/unpause player movement");

	al_draw_textf(m_font, al_map_rgb(0,0,0), 1020, 300, 0, "Diagonal moves allowed: %i", m_map->DiagsAllowed());

	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 325, 0, "Press A to generate an A Star path");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 340, 0, "Press D to generate a Dijkstra path");
	al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 355, 0, "Press C to clear the path");

	if(m_paused)
		al_draw_text(m_font, al_map_rgb(0,0,0), 1020, 500, 0, "PAUSED");

	// Draw player and enemies
	m_player->Render();

	if(m_enemiesActive)
	{
		for(Enemy* enemy : m_enemies)
		{
			enemy->Render();
		}
	}
}