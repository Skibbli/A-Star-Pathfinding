#include "GamestateManager.h"

GamestateManager::GamestateManager() {}

//Deletes all previous states and adds provided state to the list
//Used if new state is to be the main state and the previous state
//is no longer required e.g. from menu to playing a level
void GamestateManager::ChangeState(Gamestate *_state)
{
	for(unsigned int i = 0; i < m_gameStates.size(); i++)
	{
		delete m_gameStates.at(i);
		m_gameStates.clear();
	}

	AddState(_state);
}

//Adds the provided state to the list of states
void GamestateManager::AddState(Gamestate *_state) { m_gameStates.push_back( _state ); }

//Removes the last state added to the list of states
void GamestateManager::RemoveLastState()
{
	delete m_gameStates.back();
	m_gameStates.pop_back();
}

//Checks if there is a state in the list and if there is calls update
//function from that state. If that update loop returns false it means
//the game has been quit and this is then conveyed to the main game loop.
//If quit is not initiated the function returns false and play continues
bool GamestateManager::Update()
{
	bool gameRunning = true;

	if(m_gameStates.size() > 0)
	{
		gameRunning = m_gameStates.back()->Update();
	}

	else
		gameRunning = false;

	return gameRunning;
}

//Calls draw on all states in the list
void GamestateManager::Render()
{
	for( unsigned int i = 0; i < m_gameStates.size(); i++ )
	{
		m_gameStates.at(i)->Render();
	}
}

//Destructor - cleans up list of states by calling delete on each
//state in the list
GamestateManager::~GamestateManager()
{
	for(unsigned int i = 0; i < m_gameStates.size(); i++)
	{
		delete m_gameStates.at(i);
	}
}