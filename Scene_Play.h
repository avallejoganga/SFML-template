#pragma once

#include "Scene.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include <map>	
#include <memory>

#include "EntityManager.h"

class Scene_Play : public Scene
{
	struct PlayerConfig
	{
		float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
		std::string weapon;
	};

protected:
	bool			m_drawTextures = true;
	bool			m_drawCollision = false;
	bool			m_drawGrid = false;
	int				jumpTime, jumpMax, airTime;
	std::string		m_levelPath;
	PlayerConfig	m_playerConfig;	
	const Vec2f		m_gridSize = { 64, 64 };
	sf::Text		m_gridText;

public:

	Scene_Play(GameEngine& engine, const std::string& levelPath);
	void init(const std::string& levelPath);

	void loadLevel(const std::string& filename);

	void update() override;
	void spawnPlayer();
	void spawnBullet(std::shared_ptr<Entity> entity);
	

	void sAnimation();
	void sCollision();
	void sDoAction(const Action& action) override;
	void sGravity();
	void sLifeSpan();
	void sMovement();
	void sRender() override;

	void onEnd() override;
	void drawLine(const Vec2f& p1, const Vec2f p2);

	Vec2f gridToMidPixel(float x, float y, std::shared_ptr<Entity> entity);
	std::shared_ptr<Entity> player();
};

