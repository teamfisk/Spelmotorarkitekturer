#include "ResourceManager.h"

std::unordered_map<std::string, Resource**> ResourceManager::m_Instances;

void ResourceManager::Collect()
{
	for (auto& kv : m_Instances) {
		//Resource* res = *kv.second;
		//if (res->m_ReferenceCount == 0) {
		//	kv.second = nullptr;
		//}
	}
}