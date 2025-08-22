#include "Scene_Pathfinding.h"
#include <iostream>
#include <random>

Scene_Pathfinding::Scene_Pathfinding(GameEngine& engine) : Scene(engine)
{
	init();

	registerAction(sf::Keyboard::G, "TOGGLE_GRID");
	registerAction(sf::Keyboard::F, "FOLLOW");


}



void Scene_Pathfinding::init()
{
	m_entities = EntityManager();
	m_gridNodes = Vec2f(GridWidth, GridHeight);

	m_gridText.setCharacterSize(12);
	m_gridText.setFont(m_game.getAssets().getFont("FontPixel"));
	m_gridText.setFillColor(sf::Color::White);

	m_nodes = Nodes(GridWidth * GridHeight);

	int count = 0;
	for (size_t i = 0; i < GridWidth; i++)
	{
		for (size_t j = 0; j < GridHeight; j++)
		{
			m_nodes.position[i * GridHeight + j] = Vec2f(i, j);
			m_nodes.available[i * GridHeight + j] = true;
		}
	}

	createObstacles();


	dirs = { Vec2f(0, 1), 	Vec2f(1, 0), Vec2f(0, -1),  Vec2f(-1, 0) };


	Vec2f grid = gridToPixel(Vec2f(0, 0));



	//**test
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> cellDist(0, m_nodes.available.size() - 1);
	std::uniform_real_distribution<float> posDist(0.f, 64);
	std::uniform_real_distribution<float> speed(2.f, 6.f);

	for (int i = 0; i < 800; i++)
	{		
		size_t gridID;
		do
		{
			gridID = cellDist(gen);
		} while (!m_nodes.available[gridID]); 

		Vec2f grid = m_nodes.position[gridID];
		Vec2f cellOrigin = gridToPixel(grid);

		
		Vec2f pixel = cellOrigin + Vec2f(posDist(gen), posDist(gen));

		Entity e = m_entities.addEntity("X");
		auto& transform = e.add<CTransform>(pixel, Vec2f(0, 0), speed(gen));
		auto& shape = e.add<CShape>(12, 30);
		auto& follow = e.add<CFollow>();
		e.add<CPathfinding>();

		shape.shape.setOrigin(Vec2f(shape.shape.getRadius(), shape.shape.getRadius()));
		shape.shape.rotate(45);
		shape.shape.setFillColor(sf::Color::Green);
	}


	//**test**
}

Vec2f Scene_Pathfinding::gridToPixel(const Vec2f& grid)
{
	int x = grid.x * m_gridSize.x + m_gridSize.x / 2;
	int y = m_game.window().getSize().y - (grid.y * m_gridSize.y) - m_gridSize.y / 2;

	return Vec2f(x, y);
}

void Scene_Pathfinding::createObstacles()
{
	for (size_t i = 2; i < 20; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			m_nodes.available[i * GridHeight + j] = false;
			Vec2f pos = gridToPixel(m_nodes.position[i * GridHeight + j]);

			Entity e = m_entities.addEntity("Obstacle");
			e.add<CTransform>(pos, Vec2f(0, 0), 0);

			auto& shape = e.add<CShape>(46, 4).shape;
			shape.setFillColor(sf::Color::Red);
			shape.setOrigin(shape.getRadius(), shape.getRadius());
			shape.setRotation(45.f);
		}

		if (i % 2 != 0)
		{
			i += 6;
		}
	}

	for (size_t i = 6; i < 16; i++)
	{
		for (size_t j = 11; j >= 2; j--)
		{
			m_nodes.available[i * GridHeight + j] = false;
			Vec2f pos = gridToPixel(m_nodes.position[i * GridHeight + j]);

			Entity e = m_entities.addEntity("Obstacle");
			e.add<CTransform>(pos, Vec2f(0, 0), 0);

			auto& shape = e.add<CShape>(46, 4).shape;
			shape.setFillColor(sf::Color::Red);
			shape.setOrigin(shape.getRadius(), shape.getRadius());
			shape.setRotation(45.f);
		}

		if (i % 2 != 0)
		{
			i += 6;
		}
	}
}


Vec2f Scene_Pathfinding::pixelToGrid(const Vec2f& pixel)
{
	int x = (pixel.x - m_gridSize.x / 2) / m_gridSize.x;
	int y = (m_game.window().getSize().y - pixel.y - m_gridSize.y / 2) / m_gridSize.y;

	return Vec2f(x, y);
}

void Scene_Pathfinding::update()
{
	m_entities.update();

	//sPathfinding();
	sMouse();
	sFollow();
	sMovement();
	sRender();
}

void Scene_Pathfinding::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{		
		if (action.name() == "TOGGLE_GRID")
		{
			m_drawGrid = !m_drawGrid;
		}	
		else if(action.name() == "FOLLOW")
		{
			m_mouseMove = !m_mouseMove;
			//followSomeone(Vec2f(4, 5));
		}
	}
	else if (action.type() == "END")
	{
		
	}
}

void Scene_Pathfinding::sMouse()
{
	if (m_mouseMove)
	{
		sf::Vector2i pixel = sf::Mouse::getPosition(m_game.window());	
		sf::Vector2f mousePos = m_game.window().mapPixelToCoords(pixel);
		Vec2f gridVec = pixelToGrid(mousePos);
		size_t grid = gridVec.x * GridHeight + gridVec.y;

		if (grid != m_mouseGrid && m_nodes.available[grid] 
			&& gridVec.x >= 0 && gridVec.y >= 0)
		{
			m_mouseGrid = grid;
			std::cout << gridVec.x << "," << gridVec.y << std::endl;
 			followSomeone(m_nodes.position[m_mouseGrid]);
		}
	}
}

void Scene_Pathfinding::sRender()
{
	m_game.window().clear();

	for (Entity e : m_entities.getEntities())
	{
		if (e.has<CShape>())
		{
			auto& shape = e.get<CShape>().shape;
			auto& position = e.get<CTransform>().pos;
			shape.setPosition(position.x, position.y);

			m_game.window().draw(shape);
		}
	}

	if (m_drawGrid)
	{
		static sf::RenderTexture gridTexture;
		static bool initialized = false;

		if (!initialized)
		{
			int width = 100 * m_gridSize.x;
			int height = 12 * m_gridSize.y;

			gridTexture.create(width, height);
			gridTexture.clear(sf::Color::Transparent);


			sf::VertexArray grid(sf::Lines);
			for (int i = 0; i <= 100; ++i)
			{
				grid.append(sf::Vertex(sf::Vector2f(i * m_gridSize.x, 0), sf::Color::White));
				grid.append(sf::Vertex(sf::Vector2f(i * m_gridSize.x, height), sf::Color::White));
			}
			for (int j = 0; j <= 12; ++j)
			{
				grid.append(sf::Vertex(sf::Vector2f(0, j * m_gridSize.y), sf::Color::White));
				grid.append(sf::Vertex(sf::Vector2f(width, j * m_gridSize.y), sf::Color::White));
			}
			gridTexture.draw(grid);


			for (int i = 0; i < 100; ++i)
			{
				for (int j = 0; j <= 12; ++j)
				{
					m_gridText.setPosition(i * m_gridSize.x, height - (j + 1) * m_gridSize.y);
					m_gridText.setString("(" + std::to_string(i) + ", " + std::to_string(j) + ")");
					gridTexture.draw(m_gridText);
				}
			}

			gridTexture.display();
			initialized = true;
		}

		sf::Sprite gridSprite(gridTexture.getTexture());
		gridSprite.setPosition(-m_camera_offset.x, -m_camera_offset.y);
		m_game.window().draw(gridSprite);
	}


	m_game.window().display();
}

void Scene_Pathfinding::sMovement()
{
	for (Entity e : m_entities.getEntities())
	{		

		if (e.has<CTransform>())
		{
			auto& transform = e.get<CTransform>();
			transform.pos += transform.velocity * transform.speed;
		}
	}
}

void Scene_Pathfinding::sPathfinding()
{
	for (Entity e : m_entities.getEntities())
	{
		if (e.has<CPathfinding>())
		{
			auto& pathfinder = e.get<CPathfinding>();

			if (pathfinder.pathing)
			{
				auto& transform = e.get<CTransform>();

				Vec2f node = gridToPixel(m_nodes.position[pathfinder.path[pathfinder.currentNode]]);
				Vec2f desired = node - transform.pos;
				desired.normalize();

				float turnRate = 0.1f;
				transform.velocity = transform.velocity * (1.0f - turnRate) + desired * turnRate;
				transform.velocity.normalize();

				if (node.dist(transform.pos) < 25)
				{
					pathfinder.currentNode--;

					if (pathfinder.currentNode < 0)
					{
						pathfinder.pathing = false;
						transform.velocity = Vec2f(0, 0);
					}
				}
			}
		}
	}
}

void Scene_Pathfinding::sFollow()
{
	for (Entity e : m_entities.getEntities())
	{
		if (e.has<CFollow>())
		{
			auto& follow = e.get<CFollow>();

			if (follow.following)
			{
				auto& transform = e.get<CTransform>();

				if (follow.node == m_nodes.parent[follow.node])
				{
					follow.following = false;
					transform.velocity = Vec2f(0, 0);
					continue;
				}

				Vec2f dir = gridToPixel(m_nodes.position[m_nodes.parent[follow.node]]);

				if (transform.pos.dist(dir) < 25)
				{
					follow.node = m_nodes.parent[follow.node];
					dir = gridToPixel(m_nodes.position[m_nodes.parent[follow.node]]);
				}

				Vec2f desired = dir - transform.pos;
				desired.normalize();

				float turnRate = 0.15f;
				transform.velocity = transform.velocity * (1.0f - turnRate) + desired * turnRate;
				transform.velocity.normalize();
			}

		}

	}

	/*for (Entity e : m_entities.getEntities())
	{
		auto& transform = e.get<CTransform>();
		Vec2f grid = pixelToGrid(transform.pos);
		Vec2f parent = gridToPixel(m_nodes.position[m_nodes.parent[grid.x * GridHeight + grid.y]]);
		Vec2f direction = parent - transform.pos;
		transform.velocity = direction;
		transform.velocity.normalize();
	}*/
}

std::vector<size_t> Scene_Pathfinding::pathFind(const Vec2f& startVec, const Vec2f& finishVec)
{
	size_t start = startVec.x * GridHeight + startVec.y;
	size_t finish = finishVec.x * GridHeight + finishVec.y;
	std::vector<size_t> path;

	std::vector<size_t> openList;
	std::vector<size_t> closedList;


	m_nodes.currentCost[start] = 0;
	openList.push_back(start);

	while (openList.size() > 0)
	{
		size_t currentNode = openList[0];
		size_t openListIndex = 0;

		for (size_t i = 0; i < openList.size(); i++)
		{
			if (m_nodes.totalCost[currentNode] > m_nodes.totalCost[openList[i]])
			{
				currentNode = openList[i];
				openListIndex = i;
			}
		}

		openList.erase(openList.begin() + openListIndex);
		closedList.push_back(currentNode);

		if (currentNode == finish)
		{
			size_t pathNode = currentNode;

			while (pathNode != start)
			{
				path.push_back(pathNode);
				pathNode = m_nodes.parent[pathNode];
			}

			break;
		}

		for (Vec2f dir : dirs)
		{
			Vec2f position = m_nodes.position[currentNode] + dir;

			if (position.x < 0 || position.x >= m_gridNodes.x || position.y < 0 || position.y >= m_gridNodes.y)
			{
				continue;
			}

			size_t child = position.x * GridHeight + position.y;

			if (!m_nodes.available[child])
			{
				continue;
			}

			bool inClosed = false;

			for (size_t closed : closedList)
			{
				if (child == closed)
				{
					inClosed = true;
					break;
				}
			}

			if (inClosed)
			{
				continue;
			}


			float cost = dir.x + dir.y == 2 ? 1, 4 : 1;


			float currentCost = m_nodes.currentCost[currentNode] + cost;
			float estimatedCost = Physics::EuclideanDistance(m_nodes.position[child], m_nodes.position[finish]);
			float totalCost = currentCost + estimatedCost;

			bool shouldAdd = true;

			for (size_t open : openList)
			{
				if (child == open)
				{
					shouldAdd = false;

					if (m_nodes.totalCost[child] > totalCost)
					{
						m_nodes.currentCost[child] = currentCost;
						m_nodes.estimatedCost[child] = estimatedCost;
						m_nodes.totalCost[child] = totalCost;
						m_nodes.parent[child] = currentNode;
					}
					break;
				}
			}

			if (shouldAdd)
			{
				m_nodes.currentCost[child] = currentCost;
				m_nodes.estimatedCost[child] = estimatedCost;
				m_nodes.totalCost[child] = totalCost;
				m_nodes.parent[child] = currentNode;
				openList.push_back(child);
			}
		}

	}

	return path;
}


void Scene_Pathfinding::followSomeone(const Vec2f& folowee)
{
	size_t origin = folowee.x * GridHeight + folowee.y;

	std::vector<size_t> path;
	std::vector<size_t> openList;
	std::vector<size_t> closedList;


	m_nodes.currentCost[origin] = 0;
	openList.push_back(origin);
	m_nodes.parent[origin] = origin;

	while (openList.size() > 0)
	{
		size_t currentNode = openList[0];
		size_t openListIndex = 0;

		openList.erase(openList.begin() + openListIndex);
		closedList.push_back(currentNode);


		for (Vec2f dir : dirs)
		{
			Vec2f position = m_nodes.position[currentNode] + dir;

			if (position.x < 0 || position.x >= m_gridNodes.x || position.y < 0 || position.y >= m_gridNodes.y)
			{
				continue;
			}

			size_t child = position.x * GridHeight + position.y;

			if (!m_nodes.available[child])
			{
				continue;
			}

			bool inClosed = false;

			for (size_t closed : closedList)
			{
				if (child == closed)
				{
					inClosed = true;
					break;
				}
			}

			if (inClosed)
			{
				continue;
			}


			float cost = std::abs(dir.x) + std::abs(dir.y) == 2 ? 1.4 : 1;

			float currentCost = m_nodes.currentCost[currentNode] + cost;
			float estimatedCost = Physics::ManhattanDistance(m_nodes.position[origin], m_nodes.position[child]);
			float totalCost = currentCost + estimatedCost;

			bool shouldAdd = true;

			for (size_t open : openList)
			{
				if (child == open)
				{
					shouldAdd = false;

					if (m_nodes.totalCost[child] > totalCost)
					{
						m_nodes.currentCost[child] = currentCost;
						m_nodes.estimatedCost[child] = estimatedCost;
						m_nodes.totalCost[child] = totalCost;
						m_nodes.parent[child] = currentNode;
					}
					break;
				}
			}

			if (shouldAdd)
			{
				m_nodes.currentCost[child] = currentCost;
				m_nodes.estimatedCost[child] = estimatedCost;
				m_nodes.totalCost[child] = totalCost;
				m_nodes.parent[child] = currentNode;
				openList.push_back(child);
			}
		}

	}

	for (Entity e : m_entities.getEntities())
	{	
		if (e.has<CFollow>())
		{
			auto& follow = e.get<CFollow>();
			auto& transform = e.get<CTransform>();
			follow.following = true;
			Vec2f grid = pixelToGrid(transform.pos);	
			size_t gridID = grid.x * GridHeight + grid.y;			

			if (!m_nodes.available[gridID])
			{
				size_t right = (grid.x + 1) * GridHeight + grid.y;
				size_t left = (grid.x - 1) * GridHeight + grid.y;
				
				gridID = m_nodes.available[right] ? right : left;
			}
				
			follow.node = gridID;
		}
	}
}



std::vector<size_t> Scene_Pathfinding::startPath(Entity e, const Vec2f& destiny)
{
	auto& component = e.get<CPathfinding>();
	auto& transform = e.get<CTransform>();

	Vec2f start = pixelToGrid(e.get<CTransform>().pos);


	component.path = pathFind(start, destiny);
	component.pathing = true;
	component.currentNode = component.path.size() - 1;

	Vec2f node = gridToPixel(m_nodes.position[component.path[component.currentNode]]);

	transform.velocity = node - transform.pos;
	transform.velocity.normalize();

	return component.path;
}



void Scene_Pathfinding::onEnd()
{

}
