#pragma once

#include <vector>

template <class T>
class ObjectPool
{
public:
	ObjectPool(size_t size)
		: m_objectPool()
	{
		m_objectPool.reserve(size);
	}

	void create(const T& object)
	{
		for (auto& i : m_objectPool)
		{
			if (!i.isInUse())
			{
				i = object;
			}
		}
	}

	void render()
	{
		//Render all in use objects?
	}

	//return m_objectPool for collisions adn st uff? 
	

private:
	std::vector<T> m_objectPool;
};