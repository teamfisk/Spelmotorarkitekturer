#ifndef Resource_h__
#define Resource_h__

#include <list>
#include "Logging.h"

class Resource
{
	friend class IResourceHandle;
	friend class ResourceManager;

protected:
	Resource() = default;
	virtual ~Resource()
	{
		if (m_ReferenceCount != 0) {
			LOG_ERROR("Resource was freed while still carrying references!");
		}
	}

private:
	unsigned int m_ReferenceCount = 0;
};

class ThreadUnsafeResource : public Resource
{
	friend class ResourceManager;
};

#endif
