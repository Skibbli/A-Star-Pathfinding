#ifndef PATH_H
#define PATH_H

#include <vector>
#include "Node.h"
#include "glm\glm.hpp"
#include <string>
#include "allegro5\allegro_font.h"

class Path
{
	public:
		// Constructor and destructor
		Path(bool _playerPath);
		~Path();

		// Getters
		bool PathExists();
		int GetNumOps();
		int GetCalcTime();
		int GetAlgoType();
		glm::vec2 GetNextPoint();		

		// Setters
		void SetPathMessage(std::string _message);
		void SetAlgoType(int _algoType);
		void SetPathCalcTime(int _time);
		void SetNumOperations(int _numOps);

		bool CheckNextPoints();
		void SmoothPath(glm::vec2 &_pos, glm::vec2 &_dest);
		void AddNodeToBack(Node *_newNode);
		void DrawPath();

	private:
		bool m_playerPath;

		int m_numOperations;
		int m_pathCalcTime;
		int m_algoType;

		std::vector<Node*> m_path;

		std::string m_pathMessage;

		ALLEGRO_FONT *m_font;
};

#endif