#include "Scene_Play.h"
#include <iostream>


Scene_Play::Scene_Play(GameEngine& engine, const std::string& path)
	: Scene(engine), m_levelPath(path)
{
	init(m_levelPath);
}

void Scene_Play::init(const std::string& path)
{

	registerAction(sf::Keyboard::Q,			"PAUSE");
	registerAction(sf::Keyboard::Escape,	"QUIT");
	registerAction(sf::Keyboard::T,			"TOGGLE_TEXTURE");
	registerAction(sf::Keyboard::C,			"TOGGLE_COLLISION");
	registerAction(sf::Keyboard::G,			"TOGGLE_GRID");
	registerAction(sf::Keyboard::D,			"RIGHT");
	registerAction(sf::Keyboard::A,			"LEFT");
	registerAction(sf::Keyboard::Space,		"JUMP");

	

	m_gridText.setCharacterSize(12);
	m_gridText.setFont(m_game.getAssets().getFont("FontPixel"));
	m_gridText.setFillColor(sf::Color::White);

	m_camera_offset.y = m_gridSize.y * 2;
	jumpMax = 60;


	loadLevel(path);
}



void Scene_Play::drawLine(const Vec2f& p1, const Vec2f p2)
{
	sf::Vertex line[] = { sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y) };
	m_game.window().draw(line, 2, sf::Lines);
}

Vec2f Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	Vec2 hSize = entity->get<CAnimation>().animation.getSize() / 2;
	int x = gridX * m_gridSize.x + hSize.x + m_camera_offset.x;
	int y = m_game.window().getSize().y - m_camera_offset.y - (gridY * m_gridSize.y) - hSize.y;



	Vec2f position = Vec2f(x, y);


	entity->get<CTransform>().pos = position;

	return position;
}

void Scene_Play::loadLevel(const std::string& filename)
{
	m_entities = EntityManager();
	spawnPlayer();

	std::ifstream file(filename);
	std::string str;

	while (file.good())
	{

		file >> str;

		if (str == "Tile")
		{
			std::string name;
			int x, y;

			file >> name >> x >> y;

			Vec2f size = Vec2f(m_game.getAssets().getAnimation(name).getSize().x,
				m_game.getAssets().getAnimation(name).getSize().y - 2);

			auto tile = m_entities.addEntity("tile");
			tile->add<CTransform>(Vec2f(0, 0), Vec2f(0, 0), 0);
			tile->add<CBoundingBox>(size);
			tile->add<CAnimation>(m_game.getAssets().getAnimation(name), true);
			gridToMidPixel(x, y, tile);
		}
		else if (str == "Decoration")
		{
			std::string name;
			int x, y;

			file >> name >> x >> y;

			auto tile = m_entities.addEntity("tile");
			tile->add<CTransform>(Vec2f(x, y), Vec2f(0, 0), 0);
			tile->add<CAnimation>(m_game.getAssets().getAnimation(name), true);
			gridToMidPixel(x, y, tile);
		}
	}



}

void Scene_Play::spawnPlayer()
{

	auto player = m_entities.addEntity("player");
	player->add<CAnimation>(m_game.getAssets().getAnimation("Stand"), true);
	player->add<CTransform>(Vec2f(224, 352), Vec2f(0, 0), 0);
	player->add<CBoundingBox>(Vec2f(48, 48));
	player->add<CState>("stand");
	player->add<CInput>();
	player->add<CGravity>(3);
	player->get<CState>().state = "run";

	gridToMidPixel(2, 3, player);

	//TODO añadir resto de componentes
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{

}

void Scene_Play::sAnimation()
{
	//TODO Complete Animation class

	//TODO for each entity call entity->get<CAnimation>().animation.update()

	for (auto e : m_entities.getEntities())
	{
		if (e->has<CAnimation>())
		{
			e->get<CAnimation>().animation.update();
		}
	}


	//if the animation is not repeated and it has ended destroy the entity

	auto& state = player()->get<CState>().state;
	auto& anim = player()->get<CAnimation>().animation;
	auto& input = player()->get<CInput>();

	if (!input.grounded && airTime > 2)
	{
		state = "jump";
	}
	else if (std::abs(player()->get<CTransform>().velocity.x) > 0)
	{
		state = "run";
	}
	else
	{
		state = "stand";
	}



	if (state == "run" && anim.getName() != "Run")
	{
		player()->add<CAnimation>(m_game.getAssets().getAnimation("Run"), true);

	}
	else if (state == "jump" && anim.getName() != "Jump")
	{
		player()->add<CAnimation>(m_game.getAssets().getAnimation("Jump"), true);
	}
	else if (state == "stand" && anim.getName() != "Stand")
	{
		player()->add<CAnimation>(m_game.getAssets().getAnimation("Stand"), true);
	}


	if (input.grounded)
	{
		if (input.left)
		{
			player()->get<CTransform>().scale.x = -1;
		}
		else if (input.right)
		{
			player()->get<CTransform>().scale.x = 1;
		}
	}

}

void Scene_Play::sCollision()
{
	//TODO check if player fallen of a hole
	//TODO player not moving outside the screen
	auto& pTransform = player()->get<CTransform>();
	auto& pInput = player()->get<CInput>();
	player()->get<CInput>().grounded = false;

	for (auto e : m_entities.getEntities())
	{
		if (e->has<CBoundingBox>() && e->tag() != "player")
		{
			Vec2f overlap = Physics::GetOverlap(player(), e);
			Vec2f prevOverlap = Physics::GetPreviousOverlap(player(), e);

			if (overlap.x > 0 && overlap.y > 0)
			{
				auto& eTransform = e->get<CTransform>();

				if (prevOverlap.y <= 0)
				{
					if (pTransform.prevPos.y < eTransform.prevPos.y)
					{						
						pTransform.pos.y -= overlap.y;
						pTransform.velocity.y = 0;
						pInput.canJump = true;
						pInput.grounded = true;
					}
					else
					{
						pTransform.pos.y += overlap.y;
						pTransform.velocity.y = 0;
					}
				}
			
				if (prevOverlap.x <= 0)
				{
					if (pTransform.prevPos.x < eTransform.prevPos.x)
					{						
						pTransform.pos.x -= overlap.x;
					}
					else
					{						
						pTransform.pos.x += overlap.x;
					}
				}
			}


		}
	}

}

void Scene_Play::sDoAction(const Action& action)
{

	if (action.type() == "START")
	{
		if (action.name() == "TOGGLE_TEXTURE")
		{
			m_drawTextures = !m_drawTextures;
		}
		else if (action.name() == "TOGGLE_COLLISION")
		{
			m_drawCollision = !m_drawCollision;
		}
		else if (action.name() == "TOGGLE_GRID")
		{
			m_drawGrid = !m_drawGrid;
		}
		else if (action.name() == "PAUSE")
		{
			m_game.changeScene("MENU", std::make_shared <Scene_Menu>(m_game));
		}
		else if (action.name() == "RIGHT")
		{
			player()->get<CInput>().right = true;
		}
		else if (action.name() == "LEFT")
		{
			player()->get<CInput>().left = true;
		}
		else if (action.name() == "JUMP")
		{
			if (player()->get<CInput>().canJump)
			{
				player()->get<CInput>().grounded = false;
				player()->get<CInput>().up = true;
				player()->get<CInput>().canJump = false;
				jumpTime = 0;
			}
		}
	}
	else if (action.type() == "END")
	{
		if (action.name() == "RIGHT")
		{
			player()->get<CInput>().right = false;
		}
		else if (action.name() == "LEFT")
		{
			player()->get<CInput>().left = false;
		}
		else if (action.name() == "JUMP")
		{
			player()->get<CInput>().up = false;
		}
	}
}

void Scene_Play::sGravity()
{
	auto& transform = player()->get<CTransform>();

	jumpTime++;

	if (!player()->get<CInput>().grounded)
	{
		airTime++;

		if (airTime > 2)
		{
			player()->get<CInput>().canJump = false;
		}

		if (jumpTime >= jumpMax || !player()->get<CInput>().up)
		{
			transform.velocity.y = 8;
		}
		else
		{
			transform.velocity.y = -6;
		}
	}
	else
	{
		airTime = 0;
	}



}

void Scene_Play::sLifeSpan()
{

}

void Scene_Play::sMovement()
{
	auto& input = player()->get<CInput>();
	auto& velocity = player()->get<CTransform>().velocity;
	auto& state = player()->get<CState>().state;

	if (input.right) { velocity.x = 5; }
	else if (input.left) { velocity.x = -5; }
	else { velocity.x = 0; }



	for (auto e : m_entities.getEntities())
	{
		if (e->has<CTransform>())
		{
			auto& transform = e->get<CTransform>();
			transform.prevPos = transform.pos;
			transform.pos += transform.velocity;
		}
	}
}

void Scene_Play::sRender()
{
	if (!m_paused) { m_game.window().clear(sf::Color(100, 100, 200)); }
	else { m_game.window().clear(sf::Color(50, 50, 150)); }

	auto& pPos = player()->get<CTransform>().pos;
	sf::View view = m_game.window().getView();
	int yView = m_game.window().getSize().y / 2 - m_camera_offset.y;

	if(pPos.x < m_gridSize.x * 12)
	{
		view.setCenter(m_game.window().getSize().x / 2 + m_camera_offset.x, yView);
	}
	else
	{
		view.setRotation(90);
		view.setCenter(pPos.x, yView);
	}
	
	m_game.window().setView(view);

	if (m_drawTextures)
	{
		for (auto e : m_entities.getEntities())
		{
			auto& transform = e->get<CTransform>();

			if (e->has<CAnimation>())
			{
				auto& animation = e->get<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				m_game.window().draw(animation.getSprite());
			}
		}
	}

	if (m_drawCollision)
	{
		for (auto e : m_entities.getEntities())
		{
			if (e->has<CBoundingBox>())
			{
				auto& box = e->get<CBoundingBox>();
				auto& transform = e->get < CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x, box.size.y));
				rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
				rect.setPosition(transform.pos.x, transform.pos.y);
				rect.setFillColor(sf::Color(0, 0, 0, 0));
				rect.setOutlineColor(sf::Color(255, 255, 255, 255));
				rect.setOutlineThickness(1);
				m_game.window().draw(rect);
			}
		}
	}

	if (m_drawGrid)
	{
		for (size_t i = 0; i < 300; i++)
		{
			drawLine(Vec2f(m_gridSize.x + i * m_gridSize.x, -m_camera_offset.y),
				Vec2f(m_gridSize.x + i * m_gridSize.x, m_game.window().getSize().y - m_camera_offset.y));

			for(int j = 12; j >= 0; j--)
			{
				m_gridText.setPosition(i * m_gridSize.x, m_game.window().getSize().y - j * m_gridSize.y - m_camera_offset.y - m_gridSize.y);
				m_gridText.setString("(" + std::to_string(i) + ", " + std::to_string(j) + ")");
				m_game.window().draw(m_gridText);
			}
			
			
		}

		for (size_t i = 0; i < 12; i++)
		{
			drawLine(Vec2f(-m_camera_offset.x, -m_camera_offset.y + m_gridSize.y + i * m_gridSize.y),
				Vec2f(m_gridSize.y * 100, -m_camera_offset.y + m_gridSize.y + i * m_gridSize.y));
		}		
		
		

		m_game.window().draw(m_gridText);
	}

	m_game.window().display();
}

std::shared_ptr<Entity> Scene_Play::player()
{
	auto& players = m_entities.getEntities("player");

	return players[0];
}

void Scene_Play::update()
{
	m_entities.update();

	//TODO implementar pausa

	sRender();
	sLifeSpan();
	sMovement();
	sCollision();
	sGravity();
	sAnimation();

}

void Scene_Play::onEnd()
{

}

