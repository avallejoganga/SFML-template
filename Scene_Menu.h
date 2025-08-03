#pragma once
#include "Scene.h"
#include "GameEngine.h"

class Scene_Menu : public Scene
{
	std::string m_title;
	sf::Text m_menuText;
	std::vector<std::string> m_levelPaths;
	int m_selectedMenuIndex;
	void init();

public:
	Scene_Menu(GameEngine& gameEngine);
	void update() override;
	void sDoAction(const Action& action) override;
	void onEnd() override;
	void sRender() override;
};

