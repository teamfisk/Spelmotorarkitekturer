#ifndef ResourceManager_h__
#define ResourceManager_h__

#include <list>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <thread>
#include <mutex>
#include "ResourceHandle.h"
#include "ResourceBundle.h"

class ResourceManager
{
private:
	ResourceManager();

public:
	static void Initialize();

	template <typename T>
    static void RegisterBundleFormat();
	static void RegisterBundle(const std::string& path);
	template <typename T>
	static ResourceHandle<T> Load(const std::string& path);
	// TODO: template <typename T> static ResourceHandle<T> Load(std::shared_ptr<ResourceBundle::Block> block);
	//static ResourceHandle Load(UID path, unsigned int part);
	static void Collect();
    template <typename T>
    static void Free(ResourceHandle<T>& handle);

	// Process items waiting for finalization in async queue. Call this every frame!
	static void ProcessAsyncQueue();

private:
    static std::thread::id m_MainThreadID;

	typedef std::size_t ResourceTypeHash_t;

    // Background loading
	struct AsyncLoadJob
	{
		Resource** Handle;
		std::function<void()> FactoryFunction;
		//std::shared_ptr<ResourceBundle::Block> Block;
	};
   	static std::queue<AsyncLoadJob> m_AsyncLoadQueue;
	static std::mutex m_AsyncLoadQueueMutex;
	static std::queue<AsyncLoadJob> m_AsyncFinalizationQueue;
	static std::mutex m_AsyncFinalizationQueueMutex;
	static std::thread m_AsyncLoadThread;
	static void asyncLoadThreadFunction();

    // Bundles
	typedef std::function<ResourceBundle*(const std::string&)> BundleFormatFactoryFunction_t;
	static std::vector<BundleFormatFactoryFunction_t> m_BundleFormatFactoryFunctions;
	static std::list<ResourceBundle*> m_Bundles;

	// Instance map
    struct InstanceInfo
    {
		InstanceInfo()
		{
			Handle = new Resource*;
			*Handle = nullptr;
		}
        Resource** Handle;
        unsigned int ReferenceCount = 0;
		std::size_t MemoryUsage = 0;
    };
	typedef std::unordered_map<std::string, InstanceInfo> InstanceMap_t;
	static std::unordered_map<ResourceTypeHash_t, InstanceMap_t> m_Instances;

	template <typename T>
	static void loadSync(std::shared_ptr<ResourceBundle::Block> block, InstanceInfo& instance);
	template <typename T>
	static void loadAsync(std::shared_ptr<ResourceBundle::Block> block, InstanceInfo& instance);

	// The background loading thread
};

template <typename T>
void ResourceManager::RegisterBundleFormat()
{
	m_BundleFormatFactoryFunctions.push_back([](const std::string& path) { return new T(path); });
}

template <typename T>
ResourceHandle<T> ResourceManager::Load(const std::string& path)
{
	const ResourceTypeHash_t typeHash = typeid(T).hash_code();
	InstanceMap_t& instanceMap = m_Instances[typeHash];

	// Check the cache for the resource and create the resource instance if it doesn't exist in cache
    InstanceInfo& instance = instanceMap[path];
	if (*instance.Handle == nullptr) {
		// Find the first bundle that serves our request
		std::shared_ptr<ResourceBundle::Block> block = nullptr;
		for (auto& bundle : m_Bundles) {
			block = bundle->Search(path);
			if (block != nullptr) {
				break;
			}
		}
		if (block != nullptr) {
			// Only allow threaded loading on main thread
			auto currentThread = std::this_thread::get_id();
			if (currentThread == m_MainThreadID) {
				loadAsync<T>(block, instance);
			} else {
				loadSync<T>(block, instance);
			}
		} else {
			LOG_ERROR("Failed to load resource \"%s\": Not found", path.c_str());
		}
	}

	return ResourceHandle<T>(instance.Handle, &instance.ReferenceCount);
}

template <typename T>
void ResourceManager::loadSync(std::shared_ptr<ResourceBundle::Block> block, InstanceInfo& instance)
{
    // Create the instance
    *instance.Handle = new T(block);
	(*instance.Handle)->Finalize();
	(*instance.Handle)->m_Finalized = true;
}

template <typename T>
void ResourceManager::loadAsync(std::shared_ptr<ResourceBundle::Block> block, InstanceInfo& instance)
{
    AsyncLoadJob job;
	job.Handle = instance.Handle;
	job.FactoryFunction = [block, instance]() {
		*instance.Handle = new T(block);
	};
	m_AsyncLoadQueueMutex.lock();
    m_AsyncLoadQueue.push(std::move(job));
	m_AsyncLoadQueueMutex.unlock();
}

template <typename T>
void ResourceManager::Free(ResourceHandle<T>& handle)
{
    Resource** instanceHandle = handle.m_Instance;
    // Invalidate the handle
    handle.invalidate();
    // Free the resource
    if (*instanceHandle != nullptr) {
		(*instanceHandle)->Destroy();
        delete *instanceHandle;
        *instanceHandle = nullptr;
    }
}

#endif
