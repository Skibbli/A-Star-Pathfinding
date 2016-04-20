#ifndef PLAYER_H
#define PLAYER_H

#include "BaseEntity.h"

class Player : public BaseEntity
{
	public:
		// Constructor and destructor
		Player(Map *_map);
		~Player();

		// Getters
		bool HasDestination();

		// Setters
		void SetDestination(glm::vec2 _dest);

		// Functions to be defined by this class
		virtual void Update();
		virtual void TimedUpdate();
		virtual void Render();

	private:
		bool m_hasDestination;

		ALLEGRO_BITMAP *m_goalSprite;
};

#endif