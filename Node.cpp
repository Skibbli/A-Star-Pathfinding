#include "Node.h"

// Constructor - initialises member variables
Node::Node()
{
	m_fCost = 0;
	m_gCost = 0;
	m_pathParent = nullptr;
	m_changedSincePath = false;
	m_hasEnemy = false;
	m_enemyAdjacent = false;
}

// Destructor - deletes necessary objects to prevent memory leaks
Node::~Node() {}

// Getters

bool Node::HasChanged() { return m_changedSincePath; }
bool Node::IsTraversable() { return m_traversable; }
bool Node::GetEnemyOn() { return m_hasEnemy; }
bool Node::GetEnemyAdj() { return m_enemyAdjacent; }

int Node::GetNodeIndex() { return m_nodeIndex; }
int Node::GetTileType() { return m_tileType; }
int Node::GetTileX() { return m_tileX; }
int Node::GetTileY() { return m_tileY; }

float Node::GetFCost() { return m_fCost; }
float Node::GetGCost() { return m_gCost; }
float Node::GetTileCost() {	return m_terrainCost; }

Node* Node::GetParent() { return m_pathParent; }
glm::vec2 Node::GetPos() { return m_pos; }

// Setters

void Node::SetNodeIndex(int _nodeIndex) { m_nodeIndex = _nodeIndex; }
void Node::UpdateFCost(float _distToEnd) { m_fCost = m_gCost + _distToEnd; }

// Updates the G cost of this node. Takes in the G and terrain cost of the parent so that the G cost of this
// node can use half of the terrain cost of the previous node and half of the terrain cost of this node. This
// gives a more accurate path when moving over changing terrain
void Node::UpdateGCost(float _gCost, float _parentTerrainCost, float _parentGCost, bool _isPlayer)
{
	if(_isPlayer && m_hasEnemy)
		m_gCost = ((_gCost/2) * m_terrainCost * 100.0f) + ((_gCost/2 * _parentTerrainCost)) + _parentGCost;

	else if(_isPlayer && m_enemyAdjacent)
		m_gCost = ((_gCost/2) * m_terrainCost * 50.0f) + ((_gCost/2 * _parentTerrainCost)) + _parentGCost;

	else
		m_gCost = ((_gCost/2) * m_terrainCost) + ((_gCost/2 * _parentTerrainCost)) + _parentGCost;
}

void Node::UpdateParent(Node *_parent) { m_pathParent = _parent; }

// Updates the tile type and flags that the tile has been changed
// which is checked when a path is generated.
void Node::UpdateTerrain(int _terrainType)
{
	m_tileType = _terrainType;

	m_changedSincePath = true;

	switch(m_tileType)
	{
		case 0: m_terrainCost = 1.0f;
			break;
		case 1: m_terrainCost = 0.5f;
			break;
		case 2: m_terrainCost = 2.0f;
			break;
		case 3: m_terrainCost = 10.0f;
			break;
	}

	// If the terrain is number 3 it is not traversable and the flag is set
	if(m_tileType == 3)
		m_traversable = false;
	else
		m_traversable = true;
}

// Increments the enemy counter on the node and states that it has been changed
void Node::AddEnemy()
{
	m_numEnemies++;
	m_changedSincePath = true;
}

// Decrements the enemy counter on the node and states that it has been changed
// If the counter is at zero it returns false and if there are enemies still on
// the node it returns true
bool Node::RemoveEnemy()
{
	m_numEnemies--;

	if(m_numEnemies == 0)
	{
		m_changedSincePath = true;
		return false;
	}
	else
		return true;
}

// Toggles whether this node is adjacent to a node containing an enemy and states the node has been changed
void Node::ToggleEnemyAdjacent()
{
	m_enemyAdjacent = !m_enemyAdjacent;
	m_changedSincePath = true;
}

// Adds data to the node object based on the provided parameters
void Node::CreateNode(int _nodeIndex, int _tileType, float _xPos, float _yPos, int _mapWidth)
{
	m_pos.x = _xPos;
	m_pos.y = _yPos;

	m_nodeIndex = _nodeIndex;
	m_tileType = _tileType;
	m_tileX = _nodeIndex % _mapWidth;
	m_tileY = _nodeIndex / _mapWidth;

	if(_tileType == 3)
	{
		m_traversable = false;
	}

	else
		m_traversable = true;

	switch(m_tileType)
	{
		case 0: m_terrainCost = 1.0f;
			break;
		case 1: m_terrainCost = 0.5f;
			break;
		case 2: m_terrainCost = 2.0f;
			break;
		case 3: m_terrainCost = 10.0f;
			break;
	}
}

// Resets the data on the node used by the pathfinding algorithm
void Node::ResetNode()
{
	m_fCost = 0;
	m_gCost = 0;
	m_pathParent = nullptr;
	m_changedSincePath = false;
}
void Node::ClearNode()
{
	m_enemyAdjacent = false;
	m_hasEnemy = false;
}