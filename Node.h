#ifndef NODE_H
#define NODE_H

#include "glm\glm.hpp"
#include "allegro5\allegro.h"
#include "allegro5\allegro_font.h"
#include <vector>

class Enemy;

class Node
{
	public:
		// Constructor and destructors
		Node();
		~Node();

		// Getters
		bool HasChanged();
		bool IsTraversable();
		bool GetEnemyOn();
		bool GetEnemyAdj();

		int GetNodeIndex();
		int GetTileType();
		int GetTileX();
		int GetTileY();

		float GetFCost();
		float GetGCost();
		float GetTileCost();

		Node* GetParent();
		glm::vec2 GetPos();

		// Setters
		void SetNodeIndex(int _nodeIndex);		
		void UpdateFCost(float _distToEnd);		
		void UpdateGCost(float _gCost, float _parentTerrainCost, float _parentGCost, bool _isPlayer);
		void UpdateParent(Node *_parent);
		void UpdateTerrain(int _terrainType);

		void AddEnemy();
		bool RemoveEnemy();		
		void ToggleEnemyAdjacent();

		void CreateNode(int _nodeIndex, int _tileType, float _xPos, float _yPos, int _mapWidth);		
		void ResetNode();
		void ClearNode();
		
	private:
		glm::vec2 m_pos;

		bool m_traversable;
		bool m_changedSincePath;
		bool m_hasEnemy, m_enemyAdjacent;

		int m_nodeIndex;
		int m_tileType;
		int m_tileX, m_tileY;
		int m_numEnemies;
		
		float m_terrainCost;
		float m_fCost, m_gCost;

		Node *m_pathParent;
};

#endif