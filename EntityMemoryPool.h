#pragma once
#include <tuple>
#include "Components.h"

class Entity;

typedef std::tuple<
	std::vector<CTransform>,
	std::vector<CBoundingBox>,
	std::vector<CInput>,
	std::vector<CLifespan>,
	std::vector<CAnimation>,
	std::vector<CGravity>,
	std::vector<CState>,
	std::vector<CShape>,
	std::vector<CPathfinding>,
	std::vector<CFollow>
> EntityComponentVectorTuple;

constexpr int MAX_ENTITIES = 10000;

class EntityMemoryPool
{
	size_t m_numEntities;
	EntityComponentVectorTuple	m_pool;
	std::vector<std::string>	m_tags;
	std::vector<bool>			m_active;

	EntityMemoryPool(size_t maxEntities);

public:

	

	static EntityMemoryPool& Instance();

	template<typename T, typename... TArgs>
	T& addComponent(size_t id, TArgs&&... mArgs)
	{
		auto& component = std::get<std::vector<T>>(m_pool)[id];
		component = T(std::forward<TArgs>(mArgs)...);
		component.exists = true;
		return component;
	}

	Entity addEntity(const std::string& tag);

	template<typename T>
	const bool& hasComponent(size_t entityId) const
	{
		return getComponent<T>(entityId).exists;
	}

	const bool getActive(size_t entityID) const;


	template<typename T>
	const T& getComponent(size_t entityID) const
	{
		return std::get<std::vector<T>>(m_pool)[entityID];
	}

	template<typename T>
	T& getComponent(size_t entityID)
	{
		return std::get<std::vector<T>>(m_pool)[entityID];
	}

	const std::string& getTag(size_t entityID) const;
	


	
	
	void resetEntity(size_t id);

	void resizeAllVectors(size_t size);
};

