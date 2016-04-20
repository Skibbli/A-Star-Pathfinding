#ifndef ENEMY_H
#define ENEMY_H

#include "BaseEntity.h"
#include "Player.h"


class Enemy : public BaseEntity
{
	public:
		// Constructor and destructor
		Enemy(glm::vec2 _spawnPoint, float _range, Map *_map, Player *_player);
		~Enemy();

		// Getters

		// Setters
		void SetDestination(glm::vec2 _dest);

		void GenerateRandomTarget();

		// Functions implemented by this class
		virtual void Update();
		virtual void TimedUpdate();
		virtual void Render();		

	private:
		bool m_targettingPlayer;

		float m_range;
		float m_pathRequestTimer;

		glm::vec2 m_spawnPoint;
		glm::vec2 m_prevPos;

		Player *m_player;
};

#endif