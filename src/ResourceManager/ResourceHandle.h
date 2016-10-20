#ifndef ResourceHandle_h__
#define ResourceHandle_h__

#include "Resource.h"

class IResourceHandle
{
protected:
	IResourceHandle(Resource** resource);
	IResourceHandle(const IResourceHandle& handle)
	{
		LOG_DEBUG("IResourceHandle(const IResourceHandle& handle)");
		m_Instance = handle.m_Instance;
		incrementCount();
	}
	IResourceHandle& operator=(const IResourceHandle& other)
	{
		LOG_DEBUG("IResourceHandle& operator=(const IResourceHandle& other)");
		m_Instance = other.m_Instance;
		incrementCount();
		return *this;
	}

public:
	virtual ~IResourceHandle();

	//bool InUse() const { return (*m_Instance)->m_ReferenceCount > 0; }
	bool Valid() const { return ((*m_Instance) != nullptr) && (*m_Instance)->Valid(); }

protected:
	Resource** m_Instance;

private:
	void incrementCount();
	void decrementCount();
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
	ResourceHandle()
		: IResourceHandle(nullptr)
	{ }
	ResourceHandle(const ResourceHandle<T>& other)
		: IResourceHandle(other)
	{ 
		m_Instance = other.m_Instance;
	}
	ResourceHandle<T>& operator=(const ResourceHandle<T> other) 
	{ 
		m_Instance = other.m_Instance;
		return *this;
	}

	T* operator*() const;
};

template <typename T>
T* ResourceHandle<T>::operator*() const
{
    return static_cast<T*>(*m_Instance);
}

#endif