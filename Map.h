#ifndef MAP_H
#define MAP_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Node.h"
#include "glm\glm.hpp"
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <vector>
#include <memory>
#include "Path.h"

using namespace std;

class Map
{
	public:
		// Constructor and destructor
		Map(int _winWidth, int _winHeight);
		~Map();

		// Getters
		bool DiagsAllowed();
		bool IsPointTraversable(glm::vec2 &_point);
		int GetNodeIndex(glm::vec2 _pos);
		

		// Setters
		void ChangeTile(float _xPos, float _yPos, int _tileType);
		
		void LoadMap();
		void DrawMap();

		void ToggleGrid();
		void ToggleTileVals();
		void ToggleDiags();

		void AddEnemyToNode(glm::vec2 _pos);
		void RemoveEnemyFromNode(glm::vec2 _pos);
		
		Path* GetPath(glm::vec2 _startPos, glm::vec2 _endPos, int _algoType, bool _isPlayer);
		float DistBetweenNodes(Node &_first, Node &_second);
		void UpdateEdgeList();
		void UpdateSingleNodeEdgeList(int _nodeX, int _nodeY);

		void PathCleanup();
		void ResetMap();

	private:
		float m_mapWidth, m_mapHeight;
		float m_tileWidth, m_tileHeight;
		int m_numXTiles, m_numYTiles;

		bool m_showGrid, m_showTileVals, m_allowDiags;

		Node **m_mapNodes;
		vector<Node*> **m_nodeLinks;

		vector<Node*> m_openNodeList, m_closedNodeList;

		ALLEGRO_BITMAP *m_baseTiles;
		ALLEGRO_FONT *m_font;		
};

#endif