#include "Scene_Menu.h"

Scene_Menu::Scene_Menu(GameEngine& gameEngine)
	:Scene(gameEngine), m_selectedMenuIndex(0)
{
	init();
}

void Scene_Menu::init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Enter, "PLAY");
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::Q, "REPLAY");

	m_title = "Menu Mario";

	m_levelPaths.push_back("Levels/level1.txt");
	m_levelPaths.push_back("Levels/level2.txt");

	m_menuText.setFont(m_game.getAssets().getFont("FontPixel"));
	m_menuText.setCharacterSize(64);
}

void Scene_Menu::update()
{
	m_entities.update();

	sRender();
}

void Scene_Menu::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		//TODO elegir item del menud
		if (action.name() == "UP")
		{
			m_selectedMenuIndex = (m_selectedMenuIndex - 1 + m_levelPaths.size()) % m_levelPaths.size();
		}
		else if (action.name() == "DOWN")
		{
			m_selectedMenuIndex = (m_selectedMenuIndex + 1 + m_levelPaths.size()) % m_levelPaths.size();
		}
		else if (action.name() == "PLAY")
		{			
			m_game.changeScene("PLAY", std::make_shared<Scene_Play>(m_game, m_levelPaths[m_selectedMenuIndex]));
		}
		else if (action.name() == "REPLAY")
		{
			m_game.changeScene("PLAY");
		}
		else if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
	
}

void Scene_Menu::sRender()
{
	//Pone el fondo azul
	m_game.window().setView(m_game.window().getDefaultView());
	m_game.window().clear(sf::Color(100, 100, 255));

	//dibuja el título
	m_menuText.setCharacterSize(48);
	m_menuText.setString(m_title);
	m_menuText.setFillColor(sf::Color::Black);
	m_menuText.setPosition(sf::Vector2f(10, 10));
	m_game.window().draw(m_menuText);

	//dibuja las opciones del menu
	for (size_t i = 0; i < m_levelPaths.size(); i++)
	{
		m_menuText.setString(m_levelPaths[i]);
		m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color::Blue);
		m_menuText.setPosition(sf::Vector2f(10, 110 + i * 72));
		m_game.window().draw(m_menuText);
	}	

	m_game.window().display();
}

void Scene_Menu::onEnd()
{

}
