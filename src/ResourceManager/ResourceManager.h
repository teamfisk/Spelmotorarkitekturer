#ifndef ResourceManager_h__
#define ResourceManager_h__

#include <unordered_map>
#include <string>
#include "ResourceHandle.h"

class ResourceManager
{
private:
	ResourceManager();

public:
	template <typename T>
	static ResourceHandle<T> Load(const std::string& path, unsigned int part);
	//static ResourceHandle Load(UID path, unsigned int part);
	static void Collect();

	static void ForceFree(unsigned int num = 1)
    {
		for (num; num > 0; --num) {
			auto leastActiveIt = m_ActivityStack.end();
			if (leastActiveIt != m_ActivityStack.begin()) {
				return;
			}
			leastActiveIt--;
			delete **leastActiveIt;
			**leastActiveIt = nullptr;
			m_ActivityStack.erase(leastActiveIt);
		}
    }

    static void Bump(const Resource* res)
	{
		auto& it = res->m_ActivityIterator;
		// Bump it to the top of the activity stack!
		std::move(it, it, m_ActivityStack.begin());
	}

private:
	static std::unordered_map<std::string, Resource**> m_Instances;
    static std::list<Resource**> m_ActivityStack;
};

template <typename T>
ResourceHandle<T> ResourceManager::Load(const std::string& path, unsigned int part)
{
	Resource** instancePointer = nullptr;
	auto it = m_Instances.find(path);
	if (it == m_Instances.end()) {
		instancePointer = new Resource*;
		*instancePointer = nullptr;
		m_Instances[path] = instancePointer;
	} else {
		instancePointer = it->second;
	}

	if (*instancePointer == nullptr) {
		*instancePointer = new T();
		(*instancePointer)->m_ActivityIterator = m_ActivityStack.insert(m_ActivityStack.end(), instancePointer);
	}

	return ResourceHandle<T>(instancePointer);
}

#endif
