#include "Scene.h"
#include "GameEngine.h"
#include "EntityManager.h"
#include <array>

struct Nodes
{
	std::vector<Vec2f>		position;

	std::vector<float>		totalCost;
	std::vector<float>		currentCost;
	std::vector<float>		estimatedCost;

	std::vector<size_t>		parent;
	std::vector<bool>		available;


	Nodes() = default;

	Nodes(int size)
	{
		position.resize(size);
		totalCost.resize(size);
		currentCost.resize(size);
		estimatedCost.resize(size);
		parent.resize(size);
		available.resize(size);
	}
	
};


static constexpr size_t GridWidth = 20;
static constexpr size_t GridHeight = 12;




class Scene_Pathfinding : public Scene
{
	EntityManager			m_entities;
	const Vec2f				m_gridSize = { 64, 64 };
	const Vec2f				m_gridNodes = { 20.0f, 12.0f };
	sf::Text				m_gridText;

	Nodes					m_nodes;
	std::vector<Vec2f>		dirs;

	size_t					m_mouseGrid = 0;

	bool					m_drawGrid = false;
	bool					m_mouseMove = false;


	void init();

public:
	Scene_Pathfinding(GameEngine& game);

	void update() override;

	std::vector<size_t> pathFind(const Vec2f& startVec, const Vec2f& finishVec);
	Vec2f gridToPixel(const Vec2f& grid);
	Vec2f pixelToGrid(const Vec2f& pixel);

	void sFollow();
	void sMouse();
	void sMovement();
	void sRender() override;
	void sDoAction(const Action& action) override;
	void sPathfinding();

	void createObstacles();
	void onEnd() override;
	std::vector<size_t> startPath(Entity e, const Vec2f& destiny);
	void followSomeone(const Vec2f& pointToFollow);
};



