#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "glm/glm.hpp"
#include "Path.h"
#include "allegro5\allegro.h"
#include "Map.h"

class BaseEntity
{
	public:
		// Constructor and destructor
		BaseEntity();
		~BaseEntity();

		// Getters
		glm::vec2 GetPosition();
		glm::vec2 GetDestination();

		// Setters
		void SetPosition(glm::vec2 _startPos);
		
		void MoveEntity();

		void RequestPath(int _algoType);
		void ClearPath();

		// Functions to be defined by classes inheriting from this class
		virtual void Update();
		virtual void TimedUpdate();
		virtual void Render();

	protected:
		bool m_hasPath;
		bool m_isPlayer;

		float m_spriteWidth;
		float m_spriteHeight;
		float m_maxVel;
		float m_velocity;
		float m_timePassed;

		glm::vec2 m_pos;
		glm::vec2 m_nextPoint;
		glm::vec2 m_destination;
		glm::vec2 m_targetVel;
		glm::vec2 m_vecVel;
		glm::vec2 m_steering;
		
		Path *m_path;
		Map *m_map;

		ALLEGRO_BITMAP *m_sprite;
};

#endif