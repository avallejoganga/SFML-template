#pragma once
#include<map>
#include <iostream>
#include "Scene.h"
#include "Assets.h"
#include "Action.h"
#include "Scene_Play.h"
#include "Scene_Menu.h"

class GameEngine
{
	std::map<std::string, std::shared_ptr<Scene>>	m_sceneMap;
	sf::RenderWindow								m_window;
	Assets											m_assets;
	std::string										m_currentScene;
	bool											m_running;

	void init(const std::string& path);

public:

	GameEngine(const std::string& path);	

	bool isRunning();
	void update();
	void run();
	void quit();
	void changeScene();
	void sUserInput();
	void changeScene(const std::string& name, std::shared_ptr<Scene> scene);
	void changeScene(const std::string& name);

	Assets& getAssets();
	sf::RenderWindow& window();
	std::shared_ptr<Scene> currentScene();
};

