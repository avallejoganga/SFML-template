#pragma once
#include "Entity.h"



class EntityManager
{	
	std::vector<Entity>		m_entities;	
	std::vector<Entity>		m_entitiesToAdd;

	void addPendingEntities()
	{
		for (size_t i = 0; i < m_entitiesToAdd.size(); i++)
		{
			m_entities.push_back(m_entitiesToAdd[i]);
		}

		m_entitiesToAdd.clear();
	}

	void removeDeadEntities()
	{
		//Funcion que comprueba si una condición se cumple en cada objeto del vector, de cumplirse
		//se mueve al final de la lista y se recortan todos los elementos del final. Evita
		//operator invalidation
		m_entities.erase(
			std::remove_if(
				m_entities.begin(),
				m_entities.end(),
				[](Entity e) {
					return !EntityMemoryPool::Instance().getActive(e.id());
				}),
			m_entities.end());
	}


public:

	EntityManager() = default;


	Entity addEntity(const std::string& tag)
	{
		Entity e = EntityMemoryPool::Instance().addEntity(tag);
		m_entitiesToAdd.push_back(e);
		return e;
	}
	

	const std::vector<Entity> getEntities()
	{
		return m_entities;
	}

	


	void update()
	{
		removeDeadEntities();
		addPendingEntities();		
	}
};

