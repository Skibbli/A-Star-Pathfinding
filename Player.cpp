#include "Player.h"
#include "allegro5\allegro_primitives.h"

// Constructor - initialises member variables
Player::Player(Map *_map)
{
	m_hasDestination = false;
	m_sprite = al_load_bitmap("Stickman.png");
	m_goalSprite = al_load_bitmap("Goal.png");
	m_spriteWidth = al_get_bitmap_width(m_sprite);
	m_spriteHeight = al_get_bitmap_height(m_sprite);
	m_pos = glm::vec2(200,100);
	m_map = _map;
	m_isPlayer = true;
}

// Destructor - cleans up all necessary objects to prevent memory leaks
Player::~Player()
{
	al_destroy_bitmap(m_sprite);
	al_destroy_bitmap(m_goalSprite);
	if(m_path != nullptr)
		delete m_path;	
}

// Getters

// Returns whether the player has a destination position
bool Player::HasDestination() { return m_hasDestination; }

// Setters

// Sets the players destination position
void Player::SetDestination(glm::vec2 _dest)
{
	m_destination = _dest;
	m_hasDestination = true;

	// If the player already has a path when the destination changes
	// the previous path is deleted and a new path is requested
	if(m_hasPath)
	{
		int tempAlgo = m_path->GetAlgoType();
		ClearPath();
		RequestPath(tempAlgo);
	}
}

// Update function that gets called every game loop. Calculates movement for the player
// and updates their position based on direction and velocity
void Player::Update()
{
	MoveEntity();
	TimedUpdate();
}

// Contains any code that needs to be run once per second (called by the main update loop)
void Player::TimedUpdate()
{
	if(m_hasPath)
	{
		// This checks the next few points in the path to see if they have changed. This is more efficient that generating
		// a new path every second but only checks if the path has changed and doesn't take into account if other terrain has
		// changed which might provide a better path.
		if(m_path->CheckNextPoints())
		{
			int tempAlgo = m_path->GetAlgoType();
			ClearPath();
			RequestPath(tempAlgo);
		}
	}
}

// Renders the player and its destination and path (if it has one) to the screen
void Player::Render()
{
	al_draw_bitmap(m_sprite, m_pos.x - m_spriteWidth / 2, m_pos.y - m_spriteHeight / 2, 0);

	if(m_hasDestination == true)
	{
		al_draw_bitmap(m_goalSprite, m_destination.x - m_spriteWidth / 2, m_destination.y - m_spriteHeight / 2, 0);
	}

	if(m_path != nullptr)
		m_path->DrawPath();
}