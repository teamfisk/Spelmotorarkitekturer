#include "ResourceManager.h"

std::unordered_map<std::string, Resource**> ResourceManager::m_Instances;

void ResourceManager::Collect()
{
	for (auto& kv : m_Instances) {
		Resource* res = *kv.second;
		if (res == nullptr) {
			continue;
		}
		if (res->m_ReferenceCount == 0) {
			delete *kv.second;
			*kv.second = nullptr;
		} else if (res->m_ReferenceCount < 0) {
			LOG_ERROR("Ref count is less than 0, wtf have you done?");
		}
	}
}