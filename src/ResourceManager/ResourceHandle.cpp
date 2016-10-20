#include "ResourceHandle.h"

IResourceHandle::IResourceHandle(Resource** resource)
    : m_Instance(resource)
{
	incrementCount();
}

IResourceHandle::~IResourceHandle()
{
	decrementCount();
}

void IResourceHandle::incrementCount()
{
	if (m_Instance != nullptr) {
		(*m_Instance)->m_ReferenceCount++;
	}
}

void IResourceHandle::decrementCount()
{
	if (m_Instance != nullptr) {
		(*m_Instance)->m_ReferenceCount--;
	}
}

