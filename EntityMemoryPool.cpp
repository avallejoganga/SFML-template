#include "EntityMemoryPool.h"
#include "Entity.h"

EntityMemoryPool::EntityMemoryPool(size_t maxEntities)
{
	resizeAllVectors(maxEntities);
}



EntityMemoryPool& EntityMemoryPool::Instance()
{
	static EntityMemoryPool pool(MAX_ENTITIES);
	return pool;
}



Entity EntityMemoryPool::addEntity(const std::string& tag)
{
	size_t id = 0;

	for (size_t i = 0; i < MAX_ENTITIES; i++)
	{
		if (!m_active[i])
		{
			m_active[i] = true;
			id = i;
			m_tags[i] = tag;

			resetEntity(i);

			break;
		}
	}

	return Entity(id);
}





const bool EntityMemoryPool::getActive(size_t entityID) const
{
	return m_active[entityID];
}



const std::string& EntityMemoryPool::getTag(size_t entityID) const
{
	return m_tags[entityID];
}


void EntityMemoryPool::resetEntity(size_t id)
{
	std::apply
	([id](auto&... vecs)
		{
			((vecs[id] = typename std::decay_t<decltype(vecs)>::value_type()), ...);
		}, m_pool
	);
}

void EntityMemoryPool::resizeAllVectors(size_t size)
{
	std::apply
	([size](auto&... vecs)
		{
			(vecs.resize(size), ...);
		}, m_pool
	);

	m_tags.resize(size);
	m_active.resize(size);

}