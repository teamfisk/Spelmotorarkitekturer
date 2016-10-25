#include "ResourceHandle.h"

IResourceHandle::IResourceHandle(Resource** resource)
    : m_Instance(resource)
{
	incrementCount();
}

IResourceHandle::IResourceHandle(const IResourceHandle& handle)
{
	m_Instance = handle.m_Instance;
	incrementCount();
}

IResourceHandle& IResourceHandle::operator=(const IResourceHandle& other)
{
	m_Instance = other.m_Instance;
	incrementCount();
	return *this;
}

IResourceHandle::~IResourceHandle()
{
	decrementCount();
}

bool IResourceHandle::Valid() const
{
	return m_Instance != nullptr && *m_Instance != nullptr && (*m_Instance)->Valid();
}

void IResourceHandle::Release()
{
	invalidate();
	// TODO: Actually free the affected resource
}

void IResourceHandle::incrementCount()
{
	if (m_Instance != nullptr && *m_Instance != nullptr) {
		(*m_Instance)->m_ReferenceCount++;
	}
}

void IResourceHandle::decrementCount()
{
	if (m_Instance != nullptr && *m_Instance != nullptr) {
		(*m_Instance)->m_ReferenceCount--;
	}
}

void IResourceHandle::invalidate()
{
	decrementCount();
	m_Instance = nullptr;
}

