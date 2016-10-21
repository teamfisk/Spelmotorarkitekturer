#ifndef ResourceManager_h__
#define ResourceManager_h__

#include <unordered_map>
#include <string>
#include <typeinfo>
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
	typedef std::size_t ResourceTypeHash;
	typedef std::unordered_map<std::string, Resource**> InstanceMap;

	static std::unordered_map<ResourceTypeHash, InstanceMap> m_Instances;
};

template <typename T>
ResourceHandle<T> ResourceManager::Load(const std::string& path, unsigned int part)
{
	const ResourceTypeHash typeHash = typeid(T).hash_code();
	auto instanceMap = m_Instances[typeHash];

	Resource** instancePointer = nullptr;
	auto it = instanceMap.find(path);
	if (it == instanceMap.end()) {
		instancePointer = new Resource*;
		*instancePointer = nullptr;
		instanceMap[path] = instancePointer;
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
