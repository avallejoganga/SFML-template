#pragma once
#include "Entity.h"

using EntityVec = std::vector<std::shared_ptr<Entity>>;

class EntityManager
{
	
	EntityVec							m_entities;
	EntityVec							m_entitiesToAdd;
	std::map<std::string, EntityVec>	m_entityMap;
	size_t								m_totalEntities = 0;

	void removeDeadEntities(EntityVec& vec)
	{
		//Funcion que comprueba si una condición se cumple en cada objeto del vector, de cumplirse
		//se mueve al final de la lista y se recortan todos los elementos del final. Evita
		//operator invalidation
		vec.erase(
			std::remove_if(
				m_entities.begin(),
				m_entities.end(),
				[](const std::shared_ptr<Entity>& e) {
					return !e->isActive();
				}),
			m_entities.end());		
	}

public:

	EntityManager() = default;

	void update()
	{
		for (auto e : m_entitiesToAdd)
		{
			m_entities.push_back(e);
			m_entityMap[e->tag()].push_back(e);
		}

		m_entitiesToAdd.clear();

		removeDeadEntities(m_entities);

		for (auto& [tag, vec] : m_entityMap)
		{
			removeDeadEntities(vec);
		}
	}

	std::shared_ptr<Entity> addEntity(const std::string& tag)
	{
		//Estamos obligados a construirlo así y no con "make_shared" porque el constructor de Entity es privado.
		auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

		m_entitiesToAdd.push_back(entity);

		if (m_entityMap.find(tag) == m_entityMap.end()) { m_entityMap[tag] = EntityVec(); }

		return entity;
	}

	const EntityVec& getEntities()
	{
		return m_entities;
	}

	const EntityVec& getEntities(const std::string& tag)
	{
		if (m_entityMap.find(tag) == m_entityMap.end()) { m_entityMap[tag] = EntityVec(); }

		return m_entityMap[tag];
	}

	const std::map<std::string, EntityVec>& getEntityMap()
	{
		return m_entityMap;
	}
	
};

