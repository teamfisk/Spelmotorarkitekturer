#include "ResourceManager.h"

std::vector<std::function<ResourceBundle*(const std::string&)>> ResourceManager::m_BundleFormatFactoryFunctions;
std::list<ResourceBundle*> ResourceManager::m_Bundles;
std::unordered_map<ResourceManager::ResourceTypeHash, ResourceManager::InstanceMap> ResourceManager::m_Instances;

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
			Resource* res = *kv2.second;
			if (res == nullptr) {
				continue;
			}
			if (res->m_ReferenceCount == 0) {
				delete *kv2.second;
				*kv2.second = nullptr;
			}
			else if (res->m_ReferenceCount < 0) {
				LOG_ERROR("Ref count is less than 0, wtf have you done?");
			}
		}
	}
}