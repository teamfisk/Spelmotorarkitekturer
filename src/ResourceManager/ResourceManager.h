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
	static const ResourceHandle<T>& Load(const std::string& path, unsigned int part);
	//static ResourceHandle Load(UID path, unsigned int part);
	static void Collect();

private:
	static std::unordered_map<std::string, Resource**> m_Instances;
};

template <typename T>
const ResourceHandle<T>& ResourceManager::Load(const std::string& path, unsigned int part /*currently unused? */)
{
	auto it = m_Instances.find(path);
	if (it == m_Instances.end()) {
		Resource** instancePointer = new Resource*;
		*instancePointer = new T(path);
		m_Instances[path] = instancePointer;
		return ResourceHandle<T>(instancePointer);
	} else {
		return ResourceHandle<T>(it->second);
	}
}

#endif
