#include "Map.h"
#include <chrono>

// Constructor - initialises member variables
Map::Map(int _mapWidth, int _mapHeight)
{
	m_mapWidth = _mapWidth;
	m_mapHeight = _mapHeight;

	m_showGrid = true;
	m_showTileVals = false;
	m_allowDiags = true;
	
	m_baseTiles = al_load_bitmap("Base Tiles.png");
	m_font = al_load_font("Arial.ttf", 14, 0);

	// Loads the map data from the provided text file and updates the node links
	// for each node in the map
	LoadMap();
	UpdateEdgeList();
}

// Destructor - cleans up necessary objects to prevent memory leaks
Map::~Map()
{
	al_destroy_bitmap(m_baseTiles);

	for(int i = 0; i < m_numYTiles; ++i)
	{
		delete [] m_mapNodes[i];
		delete [] m_nodeLinks[i];
	}

	delete [] m_mapNodes;
	delete [] m_nodeLinks;
	al_destroy_font(m_font);
}

// Getters

// Returns whether the node at the given position is traversable
bool Map::IsPointTraversable(glm::vec2 &_point) { return m_mapNodes[(int)(_point.y / m_tileHeight)][(int)(_point.x / m_tileWidth)].IsTraversable(); }
bool Map::DiagsAllowed() { return m_allowDiags; }

// Returns the index of the node at the given position
int Map::GetNodeIndex(glm::vec2 _pos) {	return m_mapNodes[(int)(_pos.y / m_tileHeight)][(int)(_pos.x / m_tileWidth)].GetNodeIndex(); }

// Setters

// Updates the tile at the specified coordinates to the given type and then
// updates the links of the surrounding nodes
void Map::ChangeTile(float _xPos, float _yPos, int _tileType)
{
	int tempX = _xPos / m_tileWidth;
	int tempY = _yPos / m_tileHeight;
	m_mapNodes[tempY][tempX].UpdateTerrain(_tileType);

	// If the node being updated is not in the top row update the links for the nodes above it
	if(tempY > 0)
	{
		for(int n = tempX-1; n <= tempX+1; n++)
		{
			if(n >= 0 && n < m_numXTiles)
			{
				UpdateSingleNodeEdgeList(n, tempY-1);
			}
		}
	}

	// If the node being updated is not in the left-most column update the links for the nodes to the left of it
	if(tempX > 0)
	{
		UpdateSingleNodeEdgeList(tempX-1, tempY);
	}

	// If the node being updated is not in right-most top row update the links for the nodes to the right of it
	if(tempX < m_numXTiles - 1)
	{
		UpdateSingleNodeEdgeList(tempX+1, tempY);
	}

	// // If the node being updated is not in the bottom row update the links for the nodes below it
	if( tempY < m_numYTiles - 1)
	{
		for(int n = tempX-1; n <= tempX+1; n++)
		{
			if(n >= 0 && n < m_numXTiles)
			{
				UpdateSingleNodeEdgeList(n, tempY+1);
			}
		}
	}		
}

// Loads the data to populate the map nodes from the provided text file
void Map::LoadMap()
{
	std::ifstream inFile;
	std::string inData;

	// Opens the text file to read the map data
	inFile.open("Base Map.txt");

	int numRows = 0;
	int numColumns = 0;

	// Reads the data in the text file and counts how many rows of data there are
	while (std::getline(inFile, inData))
	{
		numRows++;
	}

	// Resets the stream point and reads one line of data
	inFile.clear();
	inFile.seekg(0);

	std::getline(inFile, inData);

	// Counts how many columns of data there are based on how many numbers are in the line of data
	// This only works if the numbers are single digits
	for (auto n : inData)
	{
		if (isdigit(n))
			numColumns++;
	}

	m_numXTiles = numColumns;
	m_numYTiles = numRows;

	// Calculates the width and height of each tile based on the provided map file
	m_tileWidth = m_mapWidth / (float)m_numXTiles;
	m_tileHeight = m_mapHeight / (float)m_numYTiles;

	int rowNum = 0;
	int colNum = 0;
	int tempInt = 0;

	int tempCount = 0;
	int counter = 0;

	// Dynamically creates the 2D array of pointers to the nodes based on how big the map in the text file is
	// Also creates the vectors of node links that contains each nodes' pointers to adjacent nodes
	m_mapNodes = new Node*[numRows];
	m_nodeLinks = new vector<Node*>*[numRows];
	
	for(int i = 0; i < numRows; i++)
	{
		m_mapNodes[i] = new Node[numColumns];
		m_nodeLinks[i] = new vector<Node*>[numColumns];
	}

	// Reset stream pointer
	inFile.seekg(0);

	// Reads from the file based on how many rows of data there are and for each row read the numbers one at a time and
	// load the data into the node for that point in the map
	while(rowNum != numRows)
	{
		std::getline(inFile, inData);
		std::stringstream tempStream(inData);

		while (!tempStream.eof())
		{
			tempStream >> tempInt;
			m_mapNodes[rowNum][colNum].CreateNode(counter, tempInt, colNum * m_tileWidth + m_tileWidth * 0.5, rowNum * m_tileHeight + m_tileHeight * 0.5, m_numXTiles);

			colNum++;
			counter++;
		}

		colNum = 0;
		rowNum++;
	}
}

// Renders the map to the screen along with the grid and tile values if necessary
void Map::DrawMap()
{
	for (int y = 0; y < m_numYTiles; y++)
	{
		for(int x = 0; x < m_numXTiles; x++)
		{
			al_draw_scaled_bitmap(m_baseTiles, 75 * m_mapNodes[y][x].GetTileType(), 0, 75, 75, x * m_tileWidth, y * m_tileHeight, m_tileWidth, m_tileHeight, 0);

			if(m_showTileVals)
				al_draw_textf(m_font, al_map_rgb(0,0,255), x * m_tileWidth + m_tileWidth * 0.3, y * m_tileHeight + m_tileHeight * 0.3, 0, "%f", m_mapNodes[y][x].GetTileCost());
		}		
	}

	// Draws a grid over the top of the map if it is active
	if(m_showGrid)
	{
		for (int y = 0; y < m_numYTiles; y++)
		{
			for (int x = 0; x < m_numXTiles; x++)
			{
				al_draw_line(x * m_tileWidth, 0, x * m_tileWidth, m_numYTiles * m_tileHeight, al_map_rgb(0,0,0), 1); 
			}

			al_draw_line(0, y * m_tileHeight, m_numXTiles * m_tileWidth, y * m_tileHeight, al_map_rgb(0,0,0), 1); 
		}
	}
}

void Map::ToggleGrid() { m_showGrid = !m_showGrid; }
void Map::ToggleTileVals() { m_showTileVals = !m_showTileVals; }
void Map::ToggleDiags() { m_allowDiags = !m_allowDiags; }


// Adds an enemy to the node given and updates each neighbouring node to say it is adjacent
// to an enemy
void Map::AddEnemyToNode(glm::vec2 _pos)
{
	int tempX = _pos.x/m_tileWidth;
	int tempY = _pos.y/m_tileHeight;
	
	m_mapNodes[tempY][tempX].AddEnemy();

	for(Node* node : m_nodeLinks[tempY][tempX])
	{
		node->ToggleEnemyAdjacent();
	}
}

// Removes an enemy from the node given and if there are no more enemies on that node
// then the neighbouring nodes are updated to say it is no longer adjacent to an enemy
void Map::RemoveEnemyFromNode(glm::vec2 _pos)
{
	int tempX = _pos.x/m_tileWidth;
	int tempY = _pos.y/m_tileHeight;

	bool temp = m_mapNodes[tempY][tempX].RemoveEnemy();

	if(!temp)
	{
		for(Node* node : m_nodeLinks[tempY][tempX])
		{
			node->ToggleEnemyAdjacent();
		}
	}
}

// Generates a path and allocates it to the provided path pointer
Path* Map::GetPath(glm::vec2 _startPos, glm::vec2 _endPos, int _algoType, bool _isPlayer)
{
	Path *newPath;
	newPath = new Path(_isPlayer);
	int tempNumOps = 0;

	// Create pointers to the nodes at the start position and the destination
	Node *startPoint = &m_mapNodes[(int)(_startPos.y / m_tileHeight)][(int)(_startPos.x / m_tileWidth)];
	Node *endPoint = &m_mapNodes[(int)(_endPos.y / m_tileHeight)][(int)(_endPos.x / m_tileWidth)];
	bool pathNeeded = true;

	// Initial simple checks to make sure the start and end points are valid
	if(startPoint->GetNodeIndex() == endPoint->GetNodeIndex())
	{
		newPath->SetPathMessage("You are already at your destination!");
		return newPath;
	}

	if(!startPoint->IsTraversable())
	{
		newPath->SetPathMessage("Invalid start point - please choose another!");
		return newPath;
	}

	if(!endPoint->IsTraversable())
	{
		newPath->SetPathMessage("Invalid end point - please choose another!");
		return newPath;
	}
	
	Node *parentNode;
	parentNode = startPoint;

	Node *childNode;

	// Cleans up all nodes on the map to ensure all costs have been reset
	PathCleanup();

	// Records the time at the point that the algorithm starts generating the path
	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

	// Adds parent(start position) node to the open node list and updates its cost
	m_openNodeList.push_back(parentNode);
	m_openNodeList.front()->UpdateFCost(DistBetweenNodes(*m_openNodeList.front(), *endPoint));

	bool pathFound = false;

	while(pathFound == false)
	{
		float lowestCost = 100000000.0f;
		float tempCost = 0;
		int indexCounter = 0;
		int loopCounter = 0;
		int loopSize = 0;

		// Counts how many nodes are on the open node list and loops over it to find the one with the cheapest F or G cost
		// depending on what type of algorithm is being used
		loopSize = m_openNodeList.size();

		while(loopCounter < loopSize)
		{
			if(_algoType == 0)
				tempCost = m_openNodeList[loopCounter]->GetFCost();

			else if(_algoType == 1)
				tempCost = m_openNodeList[loopCounter]->GetGCost();

			if(tempCost < lowestCost)
			{
				lowestCost = tempCost;
				indexCounter = loopCounter;
			}

			loopCounter++;
		}

		childNode = m_openNodeList[indexCounter];
		
		// Adds a pointer to the lowest cost node into the closed list
		m_closedNodeList.push_back(childNode);
		m_openNodeList.erase(m_openNodeList.begin() + indexCounter);

		tempNumOps++;

		// Checks if the last node added to the closed list is the goal and if it is continues out of the while loop
		if(m_closedNodeList.back()->GetNodeIndex() == endPoint->GetNodeIndex())
		{
			pathFound = true;
			continue;
		}

		int tempX = childNode->GetTileX();
		int tempY = childNode->GetTileY();

		bool onClosedList = false;
		bool onOpenList = false;

		// Checks each neighbour of the last node added to the closed list against the closed and open lists.
		for(Node *neighbour : m_nodeLinks[tempY][tempX])
		{
			onClosedList = false;
			onOpenList = false;
			
			// For A* algorithm
			if(_algoType == 0)
			{
				// Checks the current neighbour against the closed list and if it's on there this neighbour is skipped
				for(Node* closedNode : m_closedNodeList)
				{
						if(neighbour->GetNodeIndex() == closedNode->GetNodeIndex())
						{
							onClosedList = true;
							continue;
						}
				}

				if(onClosedList == false)
				{
					// Checks the current neighbour against the open node list. If it's on there the G cost on the node is compared to the G cost of
					// the path to get there using the current path. If the current path G cost is smaller the G and F costs on the neighbour are
					// updated and the parent is changed to the current node. Otherwise the neighbour is skipped
					for(Node* openNode : m_openNodeList)
					{
							if(neighbour->GetNodeIndex() == openNode->GetNodeIndex())
							{
								onOpenList = true;
							
								if(neighbour->GetGCost() > (glm::distance(neighbour->GetPos(), childNode->GetPos()) * neighbour->GetTileCost()) + childNode->GetGCost())
								{
									openNode->UpdateGCost(glm::distance(neighbour->GetPos(), childNode->GetPos()), childNode->GetTileCost(), childNode->GetGCost(), _isPlayer); 
									openNode->UpdateFCost(DistBetweenNodes(*openNode, *endPoint));
									openNode->UpdateParent(childNode);
								}

								continue;
							}
					}			

					// If the current neighbour is not on either list it is added to the open list and the costs and parent are updated
					if(onOpenList == false)
					{
						neighbour->UpdateGCost(glm::distance(neighbour->GetPos(), childNode->GetPos()), childNode->GetTileCost(), childNode->GetGCost(), _isPlayer);
						neighbour->UpdateFCost(DistBetweenNodes(*neighbour, *endPoint));
						neighbour->UpdateParent(childNode);
						m_openNodeList.push_back(neighbour);
					}
				}
			}

			// For Dijkstra's algorithm
			if(_algoType == 1)
			{
				for(Node* closedNode : m_closedNodeList)
				{
					// Checks the current neighbour against the closed list and if it's on there this neighbour is skipped
					if(neighbour->GetNodeIndex() == closedNode->GetNodeIndex())
					{
						if(neighbour->GetGCost() > (glm::distance(neighbour->GetPos(), childNode->GetPos()) * neighbour->GetTileCost()) + childNode->GetGCost())
						{
							closedNode->UpdateGCost(glm::distance(neighbour->GetPos(), childNode->GetPos()), childNode->GetTileCost(), childNode->GetGCost(), _isPlayer);
							closedNode->UpdateParent(childNode);
						}

						onClosedList = true;
						continue;
					}
				}

				if(onClosedList == false)
				{
					// Checks the current neighbour against the open node list. If it's on there the G cost on the node is compared to the G cost of
					// the path to get there using the current path. If the current path G cost is smaller the G and F costs on the neighbour are
					// updated and the parent is changed to the current node. Otherwise the neighbour is skipped
					for(Node* openNode : m_openNodeList)
					{
							if(neighbour->GetNodeIndex() == openNode->GetNodeIndex())
							{
								onOpenList = true;
							
								if(neighbour->GetGCost() > (glm::distance(neighbour->GetPos(), childNode->GetPos()) * neighbour->GetTileCost()) + childNode->GetGCost())
								{
									openNode->UpdateGCost(glm::distance(neighbour->GetPos(), childNode->GetPos()), childNode->GetTileCost(), childNode->GetGCost(), _isPlayer); 
									openNode->UpdateParent(childNode);
								}

								continue;
							}
					}			

					// If the current neighbour is not on either list it is added to the open list and the costs and parent are updated
					if(onOpenList == false)
					{
						neighbour->UpdateGCost(glm::distance(neighbour->GetPos(), childNode->GetPos()), childNode->GetTileCost(), childNode->GetGCost(), _isPlayer);
						neighbour->UpdateParent(childNode);
						m_openNodeList.push_back(neighbour);
					}
				}
			}
	
		}

		parentNode = childNode;

		// If the open list is empty at this point it means no path could be found to the destination
		// The message on the path is updated and the function returns to the caller
		if(m_openNodeList.empty())
		{
			newPath->SetPathMessage("No path found (probably caused by broken code...)");

			parentNode = nullptr;
			childNode = nullptr;

			return newPath;
		}
	}

	// The child node is pointed at the back of the closed list and each time the pointed to node is added
	// to the path object and the child pointer is updated to point at the parent. This continues until the
	// child node points at nullptr, at which point the loop ends
	childNode = m_closedNodeList.back();

	while(childNode != nullptr)
	{
		newPath->AddNodeToBack(childNode);
		childNode = childNode->GetParent();
	}

	// Records the time at the point the path has been generated
	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();

	// Updates the details on the path object
	switch(_algoType)
	{
		case 0:
			{
				newPath->SetPathMessage("A Star");
				newPath->SetAlgoType(0);
			}
			break;
		case 1:
			{
				newPath->SetPathMessage("Dijkstra");
				newPath->SetAlgoType(1);
			}
			break;
	}

	// Calculates the time it took to generate the path
	int duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

	// Updates details on the path and smooths it (removes unnecessary nodes)
	newPath->SetPathCalcTime(duration);
	newPath->SetNumOperations(tempNumOps);
	//newPath->SmoothPath(_startPos, _endPos);

	parentNode = nullptr;
	childNode = nullptr;

	// Returns a pointer to the path object generated
	return newPath;
}

// Calculates and returns the distance between the two nodes provided. This distance is
// calculated as if a unit were walking along it so it only goes up, down, left, right or
// diagonal between single nodes and not diagonal across multiple nodes
float Map::DistBetweenNodes(Node &_first, Node &_second)
{
	float distance;

	distance = glm::distance(_first.GetPos(), _second.GetPos());

	if(_first.GetTileX() == _second.GetTileX() || _first.GetTileY() == _second.GetTileY())
		return distance;

	int dX = abs(_first.GetTileX() - _second.GetTileX());
	int dY = abs(_first.GetTileY() - _second.GetTileY());

	int diff = abs(dX - dY);

	if(dX > dY)
		distance = dY * sqrt(pow(m_tileWidth, 2) + pow(m_tileHeight, 2)) + diff * m_tileWidth;
	else if(dY > dX)
		distance = dX * sqrt(pow(m_tileWidth, 2) + pow(m_tileHeight, 2)) + diff * m_tileHeight;
	else
		distance = dY * sqrt(pow(m_tileWidth, 2) + pow(m_tileHeight, 2));

	return distance;
}

// Cycles through all of the nodes in the map and updates the list of edges
// for each node used by the pathfinding algorithm
void Map::UpdateEdgeList()
{
	// Loop over the nodeLinks vector and clear each list
	// Each node has its own vector of pointers to adjoining nodes
	// The X and Y values refer to the X and Y of the node in question
	for(int y = 0; y < m_numYTiles; y++)
	{
		for(int x = 0; x < m_numXTiles; x++)
		{
			UpdateSingleNodeEdgeList(x, y);
		}
	}
}

// Updates the edge links for the nodes at the input X and Y coordinates
void Map::UpdateSingleNodeEdgeList(int _nodeX, int _nodeY)
{
	m_nodeLinks[_nodeY][_nodeX].clear();
	
	// If the node is not on the top row...
	if(_nodeY > 0)
	{
		// ...and diagonal movement is allowed add pointers to the three nodes above it
		if(m_allowDiags)
		{
			for(int n = _nodeX-1; n <= _nodeX+1; n++)
			{
				if(n >= 0 && n < m_numXTiles)
				{
					if(m_mapNodes[_nodeY-1][n].IsTraversable())
						m_nodeLinks[_nodeY][_nodeX].push_back(&m_mapNodes[_nodeY-1][n]);
				}

			}
		}

		// ...otherwise add a pointer to the node directly above it
		else
		{
			if(m_mapNodes[_nodeY-1][_nodeX].IsTraversable())
						m_nodeLinks[_nodeY][_nodeX].push_back(&m_mapNodes[_nodeY-1][_nodeX]);
		}
	}

	// If the node is not in the left-most column add a pointer to the node to the left
	if(_nodeX > 0)
	{
		if(m_mapNodes[_nodeY][_nodeX-1].IsTraversable())
			m_nodeLinks[_nodeY][_nodeX].push_back(&m_mapNodes[_nodeY][_nodeX-1]);
	}

	// If the node is not in the left-most column add a pointer to the node to the right
	if(_nodeX < m_numXTiles - 1)
	{
		if(m_mapNodes[_nodeY][_nodeX+1].IsTraversable())
			m_nodeLinks[_nodeY][_nodeX].push_back(&m_mapNodes[_nodeY][_nodeX+1]);
	}

	// If the node is not in the bottom row...
	if( _nodeY < m_numYTiles - 1)
	{
		// ...and diagonal movement is allowed add pointers to the three nodes below it
		if(m_allowDiags)
		{
			for(int n = _nodeX-1; n <= _nodeX+1; n++)
			{
				if(n >= 0 && n < m_numXTiles)
				{
					if(m_mapNodes[_nodeY+1][n].IsTraversable())
						m_nodeLinks[_nodeY][_nodeX].push_back(&m_mapNodes[_nodeY+1][n]);
				}

			}
		}

		// otherwise add a pointer to the node directly below it
		else
		{
			if(m_mapNodes[_nodeY+1][_nodeX].IsTraversable())
						m_nodeLinks[_nodeY][_nodeX].push_back(&m_mapNodes[_nodeY+1][_nodeX]);
		}
	}					
}

// Loops over all nodes that have been used by the path finding algorithm and resets them
void Map::PathCleanup()
{
	for(Node* n : m_closedNodeList)
	{
		n->ResetNode();
	}

	for(Node* n : m_openNodeList)
	{
		n->ResetNode();
	}

	m_openNodeList.clear();
	m_closedNodeList.clear();
}
void Map::ResetMap()
{
	for(int y = 0; y < m_numYTiles; y++)
	{
		for(int x = 0; x < m_numXTiles; x++)
		{
			m_mapNodes[y][x].ClearNode();
		}
	}
}