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

private:
	static std::unordered_map<std::string, Resource**> m_Instances;
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
		// Create the resource instance
		*instancePointer = new T(path);
	}

	return ResourceHandle<T>(instancePointer);
}

#endif
