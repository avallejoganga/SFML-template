#pragma once
#include "Components.h"
#include <string>
#include <tuple>
#include "EntityMemoryPool.h"


class Entity
{
	size_t			m_id;	

public:

	Entity(size_t id)
		: m_id(id) {}

	template<typename T, typename... TArgs>
	const T& add(TArgs&&... mArgs) const
	{
		return EntityMemoryPool::Instance().addComponent<T>(m_id, std::forward<TArgs>(mArgs)...);
	}

	template<typename T, typename... TArgs>
	T& add(TArgs&&... mArgs)
	{
		return EntityMemoryPool::Instance().addComponent<T>(m_id, std::forward<TArgs>(mArgs)...);
	}


	template<typename T>
	const T& get() const
	{
		return EntityMemoryPool::Instance().getComponent<T>(m_id);
	}

	template<typename T>
	T& get()
	{
		return EntityMemoryPool::Instance().getComponent<T>(m_id);
	}
	
	template <typename T>
	bool has() const
	{
		return EntityMemoryPool::Instance().hasComponent<T>(m_id);
	}	

	size_t id()	const
	{
		return m_id;	
	}	
	

	const std::string& tag() const
	{
		return EntityMemoryPool::Instance().getTag(m_id);
	}
};

 