#include "ResourceManager.h"

std::thread::id ResourceManager::m_MainThreadID = std::this_thread::get_id();
std::size_t ResourceManager::m_MemoryLimit = 0;
std::atomic_size_t ResourceManager::m_AllocatedMemory(0);

std::queue<ResourceManager::AsyncLoadJob> ResourceManager::m_AsyncLoadQueue;
std::mutex ResourceManager::m_AsyncLoadQueueMutex;
std::queue<ResourceManager::AsyncLoadJob> ResourceManager::m_AsyncFinalizationQueue;
std::mutex ResourceManager::m_AsyncFinalizationQueueMutex;
std::thread ResourceManager::m_AsyncLoadThread;

std::vector<std::function<ResourceBundle*(const std::string&)>> ResourceManager::m_BundleFormatFactoryFunctions;
std::list<ResourceBundle*> ResourceManager::m_Bundles;
std::unordered_map<ResourceManager::ResourceTypeHash_t, ResourceManager::InstanceMap_t> ResourceManager::m_Instances;

void ResourceManager::Initialize(std::size_t memoryLimit)
{
	m_MemoryLimit = memoryLimit;
	m_AsyncLoadThread = std::thread(ResourceManager::asyncLoadThreadFunction);
}

void ResourceManager::RegisterBundle(const std::string& path)
{
	ResourceBundle* bundle = nullptr;
	// Try all registered bundle formats
	for (auto& f : m_BundleFormatFactoryFunctions) {
		try {
			bundle = f(path);
		} catch (const ResourceBundle::InvalidBundleFormat& ex) {
			continue;
		} catch (const std::exception& ex) {
			LOG_ERROR("Failed to register bundle \"%s\": %s", path.c_str(), ex.what());
            throw;
		}
		break;
	}

	if (bundle != nullptr) {
        m_Bundles.push_front(bundle);
	} else {
		LOG_ERROR("Failed to register bundle \"%s\": Unknown bundle format", path.c_str());
	}
}

void ResourceManager::Collect()
{
	for (auto& kv : m_Instances) {
		auto instanceMap = kv.second;
		for (auto& kv2 : instanceMap) {
			InstanceInfo& instance = kv2.second;
			Resource* res = *instance.Handle;
			if (res == nullptr) {
				continue;
			}
			if (instance.ReferenceCount == 0) {
				res->Destroy();
				delete res;
				*instance.Handle = nullptr;
				m_AllocatedMemory -= instance.MemoryUsage;
				LOG_INFO("ResourceManager: Garbage collected resource \"%s\"", kv2.first.c_str());
			}
		}
	}
}

void ResourceManager::ProcessAsyncQueue()
{
	while (!m_AsyncFinalizationQueue.empty()) {
		// Grab a job
		m_AsyncFinalizationQueueMutex.lock();
		auto job = m_AsyncFinalizationQueue.front();
		m_AsyncFinalizationQueue.pop();
		m_AsyncFinalizationQueueMutex.unlock();

		// Finalize it
		(*job.Instance->Handle)->Finalize();
		(*job.Instance->Handle)->m_Finalized = true;

		std::size_t allocationSize = job.Instance->MemoryUsage + (*job.Instance->Handle)->Size();
		m_AllocatedMemory += allocationSize;
		if (m_AllocatedMemory > m_MemoryLimit) {
			LOG_WARNING("Asynchronous resource allocation of %i byte exceeded ResourceManager memory limit!", allocationSize);
		}
	}
}

void ResourceManager::asyncLoadThreadFunction()
{
	LOG_DEBUG("Started ResourceManager async load thread");
	while (true) {
		// Grab a job
        m_AsyncLoadQueueMutex.lock();
		if (m_AsyncLoadQueue.empty()) {
            m_AsyncLoadQueueMutex.unlock();
			continue;
		}
        auto job = m_AsyncLoadQueue.front();
		m_AsyncLoadQueue.pop();
		m_AsyncLoadQueueMutex.unlock();

		// Run the factory function
		LOG_DEBUG("ResourceLoadThread: Calling factory function");
		job.FactoryFunction();

		// Insert into finalization queue
		m_AsyncFinalizationQueueMutex.lock();
		m_AsyncFinalizationQueue.push(std::move(job));
		m_AsyncFinalizationQueueMutex.unlock();
	}
}