#pragma once
#include "EntityManager.h"
#include "Action.h"
#include <iostream>
#include "Physics.h"

class GameEngine;

class Scene
{
protected:
	GameEngine&						m_game;
	EntityManager					m_entities;
	std::map<int, std::string>		m_actionMap;
	Vec2f							m_camera_offset = { 0, 0 };
	int								m_frame;
	bool							m_paused;

public:
	
	Scene(GameEngine& engine): m_game(engine), m_frame(0), m_paused(false){ }
	virtual void update() = 0;
	virtual void sDoAction(const Action& action) = 0;
	virtual void sRender() = 0;	 

	virtual void simulate(int frames)
	{
		for (size_t i = 0; i < frames; i++)
		{
			update();
		}
	}

	
	virtual void registerAction(int inputKey, const std::string& action)
	{
		m_actionMap[inputKey] = action;
	}

	virtual void onEnd() = 0;
	std::map<int, std::string>& getActionMap()
	{
		return m_actionMap;
	};

};

