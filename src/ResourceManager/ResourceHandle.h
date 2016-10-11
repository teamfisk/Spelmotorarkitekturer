#ifndef ResourceHandle_h__
#define ResourceHandle_h__

#include "Resource.h"

class IResourceHandle
{
protected:
	IResourceHandle(Resource** resource)
		: m_Instance(resource)
	{
		(*m_Instance)->m_ReferenceCount++;
	}

public:
	virtual ~IResourceHandle()
	{
		(*m_Instance)->m_ReferenceCount--;
	}

	//bool InUse() const { return (*m_Instance)->m_ReferenceCount > 0; }
	//bool Valid() const { return ((*m_Instance) != nullptr) && (*m_Instance)->Valid(); }

protected:
	Resource** m_Instance;
};

template <typename T>
class ResourceHandle : public IResourceHandle
{
	friend class ResourceManager;

private:
	ResourceHandle(Resource** resource)
		: IResourceHandle(resource)
	{ }

public:
	T* operator*() const { return static_cast<T*>(*m_Instance); }
};

#endif