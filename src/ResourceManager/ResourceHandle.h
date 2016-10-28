#ifndef ResourceHandle_h__
#define ResourceHandle_h__

#include "Resource.h"

class IResourceHandle
{
    friend class ResourceManager;
protected:
	IResourceHandle(Resource** resource);
	IResourceHandle(const IResourceHandle& handle);
	IResourceHandle& operator=(const IResourceHandle& other);

public:
	virtual ~IResourceHandle();

	//bool InUse() const { return (*m_Instance)->m_ReferenceCount > 0; }
	bool Valid() const;
	void Release();

protected:
	Resource** m_Instance;

private:
	void incrementCount();
	void decrementCount();
	void invalidate();
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
	{ }
	ResourceHandle<T>& operator=(const ResourceHandle<T> other);

	T* operator*() const;
	T* operator->() const;
};

template <typename T>
ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle<T> other)
{
	IResourceHandle::operator=(other);
	return *this;
}

template <typename T>
T* ResourceHandle<T>::operator*() const
{
    return static_cast<T*>(*m_Instance);
}

template <typename T>
T* ResourceHandle<T>::operator->() const
{
	return this->operator*();
}

#endif