#include "ResourceHandle.h"

IResourceHandle::IResourceHandle(Resource** resource)
    : m_Instance(resource)
{
    (*m_Instance)->m_ReferenceCount++;
}

IResourceHandle::~IResourceHandle()
{
    (*m_Instance)->m_ReferenceCount--;
}

