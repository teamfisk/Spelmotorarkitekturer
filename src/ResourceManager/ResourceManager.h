#ifndef ResourceManager_h__
#define ResourceManager_h__

#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <functional>
#include "ResourceHandle.h"
#include "ResourceBundle.h"

class ResourceManager
{
private:
	ResourceManager();

public:
	template <typename T>
    static void RegisterBundleFormat();
	static void RegisterBundle(const std::string& path);
	template <typename T>
	static ResourceHandle<T> Load(const std::string& path);
	//static ResourceHandle Load(UID path, unsigned int part);
	static void Collect();

private:
	typedef std::size_t ResourceTypeHash;
	typedef std::unordered_map<std::string, Resource**> InstanceMap;

	static std::vector<std::function<ResourceBundle*(const std::string&)>> m_BundleFormatFactoryFunctions;
	static std::list<ResourceBundle*> m_Bundles;
	static std::unordered_map<ResourceTypeHash, InstanceMap> m_Instances;
};

template <typename T>
void ResourceManager::RegisterBundleFormat()
{
	m_BundleFormatFactoryFunctions.push_back([](const std::string& path) { return new T(path); });
}

template <typename T>
ResourceHandle<T> ResourceManager::Load(const std::string& path)
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

	// Create the resource instance if it doesn't exist in cache
	if (*instancePointer == nullptr) {
		// Find the first bundle that serves our request
		std::shared_ptr<ResourceBundle::Block> block = nullptr;
		for (auto& bundle : m_Bundles) {
			block = bundle->Search(path);
			if (block != nullptr) {
				break;
			}
		}
		if (block != nullptr) {
			// Create the instance
			*instancePointer = new T(block);
		} else {
			LOG_ERROR("Failed to load resource \"%s\": Not found", path.c_str());
		}
	}

	return ResourceHandle<T>(instancePointer);
}


#endif
