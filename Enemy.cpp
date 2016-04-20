#include "Enemy.h"
#include <cstdlib>
#include <time.h>

// Constructor - initialises member variables
Enemy::Enemy(glm::vec2 _spawnPoint, float _range, Map *_map, Player *_player)
{
	m_destination = glm::vec2(-1,-1);
	m_sprite = al_load_bitmap("Enemy.png");
	m_spriteWidth = al_get_bitmap_width(m_sprite);
	m_spriteHeight = al_get_bitmap_height(m_sprite);
	m_pathRequestTimer = 20.1f;
	m_targettingPlayer = false;
	m_player = _player;
	m_isPlayer = false;
	m_map = _map;
	m_pos = _spawnPoint;
	m_prevPos = m_pos;
	m_spawnPoint = _spawnPoint;
	m_range = _range;
	srand(time(NULL));
}

//Destructor - deletes necessary objects to prevent memory leaks
Enemy::~Enemy() { al_destroy_bitmap(m_sprite); }

// Sets the destination of the enemy
void Enemy::SetDestination(glm::vec2 _dest) { m_destination = _dest; }

// Generates a random position until it finds one that is valid and reachable and then
// requests a path to that position
void Enemy::GenerateRandomTarget()
{
	bool goodTarget = false;

	while(!goodTarget && !m_hasPath)
	{
		bool inBounds = false;

		glm::vec2 target, rangeTarget;

		while(!goodTarget)
		{
			// Generate a random position vector within the bounds of the range of this enemy and its spawn point
			target = glm::vec2(rand() % (int)((m_range*2) - m_range), rand() % (int)((m_range*2) - m_range));
			rangeTarget = glm::vec2(m_spawnPoint.x + target.x, m_spawnPoint.y + target.y);

			// If the position generated is on the map, at least a specified distance away from the spawn point
			// and is traversable i.e. not an obstacle then the point is good and the program continues
			if((rangeTarget.x > 0 && rangeTarget.x < 1000 && rangeTarget.y > 0 && rangeTarget.y < 1000) && glm::distance(rangeTarget, m_pos) > 100.0f)
			{
				if(m_range > glm::distance(m_spawnPoint, rangeTarget) && m_map->IsPointTraversable(rangeTarget))
					goodTarget = true;
			}
		}

		// A path is requested to the point generated above. If a path is found the program ends, otherwise the function
		// starts again with a new point
		m_destination = rangeTarget;

		RequestPath(0);

		if(!m_hasPath)
			ClearPath();		
	}	
}

// Update loop that gets called every time the game loops. Calculates movement
// for the enemy and updates their position.
void Enemy::Update()
{
	MoveEntity();

	TimedUpdate();

	// If the enemy does not have a path and the timer is above the limit request
	// a new path to a random position and reset the time.
	if(!m_hasPath)
	{
		if(m_pathRequestTimer > 20.0f)
		{
			GenerateRandomTarget();
			m_pathRequestTimer = 0;
		}

		// Otherwise increment the timer in line with the FPS of the program
		else
			m_pathRequestTimer += 0.03333333;
	}
}

void Enemy::TimedUpdate()
{
	// This code is for getting the enemy to seek the player if it gets too close, however it isn't working correctly
	
	/*if(glm::distance(m_pos, m_spawnPoint) > 300.0f)
	{
		m_targettingPlayer = false;
		m_destination = m_spawnPoint;
		RequestPath(1);
	}

	else if(glm::distance(m_pos, m_player->GetPosition()) < 200.0f && m_destination != m_spawnPoint)
	{
		m_targettingPlayer = true;
	}
	
	if(glm::distance(m_pos, m_spawnPoint) > 300.0f)
	{
		m_targettingPlayer = false;
		m_destination = m_spawnPoint;
		RequestPath(1);
	}
	
	if(m_targettingPlayer)
	{
		m_destination = m_player->GetPosition();
		RequestPath(1);
	}*/

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

	// If the node the enemy is currently on is different from the node they were on last timed check, update the old node to
	// say the enemy is no longer there and update the node it is currently on to say there is an enemy on it
	if(m_map->GetNodeIndex(m_pos) != m_map->GetNodeIndex(m_prevPos))
	{
		m_map->RemoveEnemyFromNode(m_prevPos);
		m_map->AddEnemyToNode(m_pos);
		m_prevPos = m_pos;
	}	
}

// Renders the enemy to the screen and draws its path if it has one
void Enemy::Render()
{
	al_draw_bitmap(m_sprite, m_pos.x - m_spriteWidth / 2, m_pos.y - m_spriteHeight / 2, 0);

	if(m_path != nullptr)
		m_path->DrawPath();
}