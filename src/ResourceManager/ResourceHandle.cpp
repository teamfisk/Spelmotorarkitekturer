#include "ResourceHandle.h"

IResourceHandle::IResourceHandle(Resource** resource, unsigned int* refCount)
    : m_Instance(resource)
	, m_RefCount(refCount)
{
	incrementCount();
}

IResourceHandle::IResourceHandle(const IResourceHandle& handle)
{
	m_Instance = handle.m_Instance;
	m_RefCount = handle.m_RefCount;
	incrementCount();
}

IResourceHandle& IResourceHandle::operator=(const IResourceHandle& other)
{
	m_Instance = other.m_Instance;
	m_RefCount = other.m_RefCount;
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
}

void IResourceHandle::incrementCount()
{
	if (m_Instance != nullptr) {
		(*m_RefCount)++;
	}
}

void IResourceHandle::decrementCount()
{
	if (m_Instance != nullptr) {
		(*m_RefCount)--;
	}
}

void IResourceHandle::invalidate()
{
	decrementCount();
	m_Instance = nullptr;
}

