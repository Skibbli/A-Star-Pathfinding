#include "BaseEntity.h"

// Constructor - initialises member variables
BaseEntity::BaseEntity()
{
	m_hasPath = false;
	m_path = nullptr;
	m_timePassed = 0.0f;
	m_velocity = 0.0f;
	m_vecVel = glm::vec2(0.0f,0.0f);
	m_maxVel = 0.15f;
}

// Destructor - deletes necessary objects to prevent memory leaks
BaseEntity::~BaseEntity() {}

// Getters

glm::vec2 BaseEntity::GetPosition() { return m_pos; }
glm::vec2 BaseEntity::GetDestination() { return m_destination; }

// Setters

// Sets the position of the base entity
// If it has a path delete it and request a new one from the updated position
void BaseEntity::SetPosition(glm::vec2 _pos)
{
	m_pos = _pos;

	if(m_hasPath)
	{
		int tempAlgo = m_path->GetAlgoType();
		ClearPath();
		RequestPath(tempAlgo);
	}
}

// Moves entity along the path
void BaseEntity::MoveEntity()
{
	// Calls code we only want to update at set intervals
	if(m_timePassed > 1.0f)
	{
		TimedUpdate();
		m_timePassed = 0.0f;
	}

	else
		m_timePassed += 0.03333333;

	if(m_hasPath)
	{
		// If the entity is within the specified distance of the destination clear their path
		if(glm::distance(m_pos, m_destination) < 5.0f)
		{
			ClearPath();
		}

		// If the player is within the specified distance to the point in the path get the next point
		// If there is no next point then the next point is the destination so update the next point to that
		else if(glm::distance(m_nextPoint, m_pos) < 25)
		{
			m_nextPoint = m_path->GetNextPoint();

			if(m_nextPoint == glm::vec2(-1,-1))
				m_nextPoint = m_destination;
		}

		// Calculates the vector from the current position to the destination and normalises it
		m_targetVel = glm::normalize(m_nextPoint - m_pos) * m_maxVel;
		m_steering = m_targetVel - m_vecVel;
		m_steering.x *= 0.05f;
		m_steering.y *= 0.05f;
		
		// If the entity is not at maximum velocity increase its speed
		if(m_velocity < m_maxVel)
			m_velocity += 0.01;

		// Applies the steering force to the current velocity
		m_vecVel += m_steering;

		// Updates the enemy position based on direction and velocity
		m_pos += m_vecVel;
	}
}

// Requests a path to the base entity's destination from it's current position
void BaseEntity::RequestPath(int _algoType)
{
	// If there is already a path delete it
	if(m_path != nullptr)
		delete m_path;

	// Request a new path from the map, providing the start position, end position and type of algorithm we want to use
	m_path = m_map->GetPath(m_pos, m_destination, _algoType, m_isPlayer);

	// If there is a path on the path object update the next point for the entity to head to and it's current/target heading
	if(m_path->PathExists())
	{
		m_hasPath = true;
		m_nextPoint = m_path->GetNextPoint();
		m_targetVel = glm::normalize(m_nextPoint - m_pos) * m_maxVel;
		m_steering = m_targetVel - m_vecVel;
	}
}

// Clears the previous path object and all necessary flags on the object
void BaseEntity::ClearPath()
{
	delete m_path;
	m_path = nullptr;
	m_hasPath = false;
	m_velocity = 0;
	m_targetVel = glm::vec2(0.0f,0.0f);
	m_steering = glm::vec2(0.0f,0.0f);
	m_vecVel = glm::vec2(0.0f,0.0f);
}

// Functions defined by inheriting classes
void BaseEntity::Update() {}
void BaseEntity::TimedUpdate() {}
void BaseEntity::Render() {}