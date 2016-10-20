#include "ResourceManager.h"

std::unordered_map<ResourceManager::ResourceTypeHash, ResourceManager::InstanceMap> ResourceManager::m_Instances;

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