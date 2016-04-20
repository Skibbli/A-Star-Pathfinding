#include "Path.h"
#include "allegro5\allegro_primitives.h"

// Constructor - initialises member variables
Path::Path(bool _playerPath)
{
	m_font = al_load_font("Arial.ttf", 14, 0);
	m_numOperations = 0;
	m_pathCalcTime = 0;
	m_pathMessage = "";
	m_playerPath = _playerPath;
}

// Destructor - frees up necessary objects to prevent memory leaks
Path::~Path()
{
	al_destroy_font(m_font);
	m_path.clear();
}

// Getters

bool Path::PathExists()
{
	if(m_path.empty())
		return false;

	else
		return true;
}
int Path::GetNumOps() { return m_numOperations; }
int Path::GetCalcTime() { return m_pathCalcTime; }
int Path::GetAlgoType() { return m_algoType; }

// Returns the next point on the path or a default vector if there are no more points
glm::vec2 Path::GetNextPoint()
{
	// If there are still points on the path return the next one...
	if(!m_path.empty())
	{
		glm::vec2 tempVec = m_path.back()->GetPos();
		m_path.pop_back();

		return tempVec;
	}

	// Otherwise return a default vector
	return glm::vec2(-1,-1);
}

// Setters

void Path::SetPathMessage(std::string _message) { m_pathMessage = _message; }
void Path::SetAlgoType(int _algoType) { m_algoType = _algoType; }
void Path::SetPathCalcTime(int _time) {	m_pathCalcTime = _time; }
void Path::SetNumOperations(int _numOps) { m_numOperations = _numOps; }

// Checks the next points on the path and returns true if any of them have changed
bool Path::CheckNextPoints()
{
	for(int i = m_path.size() - 1; i > m_path.size() - 6 && i >= 0; i--)
	{
		return m_path[i]->HasChanged();
	}

	return false;
}

// Smooths the path by removing any unnecessary points
void Path::SmoothPath(glm::vec2 &_pos, glm::vec2 &_dest)
{
	for(int i = m_path.size()-1; i > 1; i--)
	{
		int nextEle = i - 1;
		int startXPos = m_path[i]->GetTileX();
		int startYPos = m_path[i]->GetTileY();

		while(true)
		{
			// Checks the next point in the path and if it's the last point on the path end the function call
			if(m_path[nextEle]->GetNodeIndex() == m_path[0]->GetNodeIndex())
				return;

			int nextXPos = m_path[nextEle]->GetTileX();
			int nextYPos = m_path[nextEle]->GetTileY();

			// Checks the next point in the path and if either the x or y coordinates are the same as the point being checked remove it and check the point after it
			// This is because a grid system is used and if either of the coordinates are the same the line is straight and the point is not required
			// This also checks the next point to make sure it is on the same line. If it's not one the same line then we need to keep the original point otherwise
			// the path will cut the corner
			if((startXPos == nextXPos || startYPos == nextYPos) && (startXPos != m_path[nextEle+1]->GetTileX() && startYPos != m_path[nextEle+1]->GetTileY()))
			{
				m_path.erase(m_path.begin() + nextEle);
				nextEle--;
			}

			// If the point does not need to be removed move on to the next point
			else
				break;
		}
	}

	// If the objects start position is closer to the next waypoint than the first point on the path remove the first point in the path
	if(glm::distance(m_path.back()->GetPos(), m_path.at(m_path.size()-2)->GetPos()) > glm::distance(_pos, m_path.at(m_path.size()-2)->GetPos()))
	{
		m_path.pop_back();
	}

	// Remove the end point in the path - this is because in all scenarios we want the object to move straight to the goal from the previous tile
	// and not to the middle of the goal tile and then to the goal
	m_path.erase(m_path.begin());
}

// Adds a node to the back of the path
void Path::AddNodeToBack(Node *_newPoint) { m_path.push_back(_newPoint); }

// Renders the path to the screen and some information about the generated path
void Path::DrawPath()
{
	if(!m_path.empty())
	{
		for(int i = 0; i < m_path.size() - 1; i++)
			al_draw_line(m_path.at(i)->GetPos().x, m_path.at(i)->GetPos().y, m_path.at(i+1)->GetPos().x, m_path.at(i+1)->GetPos().y, al_map_rgb(0,0,0), 2);
	}

	if(m_playerPath)
	{
		al_draw_textf(m_font, al_map_rgb(0,0,0), 1020, 400, 0, "%s", m_pathMessage.c_str());

		al_draw_textf(m_font, al_map_rgb(0,0,0), 1020, 430, 0, "Operations to find path: %i", m_numOperations);
		al_draw_textf(m_font, al_map_rgb(0,0,0), 1020, 445, 0, "Time taken to find path: %i milliseconds", m_pathCalcTime);
	}
}