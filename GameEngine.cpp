#include "GameEngine.h"


GameEngine::GameEngine(const std::string& path)
	:m_running(true)
{
	init(path);
}

void GameEngine :: init(const std::string& path)
{
	m_assets.loadFromFile(path);

	m_window.create(sf::VideoMode(1280, 768), "Mario time");	
	m_window.setFramerateLimit(60);

	
	//changeScene("PLAY", std::make_shared <Scene_Play>(*this, "Levels/level2.txt"));
	changeScene("MENU", std::make_shared <Scene_Menu>(*this));

	

	run();
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning()
{
	return m_running && m_window.isOpen();
}

sf::RenderWindow& GameEngine::window()
{
	return m_window;
}

void GameEngine::run()
{
	int x = 0;
	while (isRunning())
	{
		sUserInput();
		update();
	}
}

void GameEngine::quit()
{

}

void GameEngine::update()
{
	currentScene()->update();
}

Assets& GameEngine::getAssets()
{
	return m_assets;
}



void GameEngine::changeScene(const std::string& name, std::shared_ptr<Scene> scene)
{
	m_currentScene = name;
	m_sceneMap[name] = scene;
}

void GameEngine::changeScene(const std::string& name)
{
	m_currentScene = name;
}


void GameEngine::sUserInput()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			quit();
		}

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::X)
			{
				std::cout << "screenshot saved to " << "test.png" << std::endl;
				sf::Texture texture;

				texture.create(m_window.getSize().x, m_window.getSize().y);
				texture.update(m_window);

				if (texture.copyToImage().saveToFile("test.png"))
				{
					std::cout << "screenshot saved" << std::endl;
				}
			}			
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			

			if (currentScene()->getActionMap().find(event.key.code) ==
				currentScene()->getActionMap().end())
			{
				continue;
			}

			const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

			Action action(currentScene()->getActionMap().at(event.key.code), actionType);

			

			currentScene()->sDoAction(action);
		}
	}

	
}