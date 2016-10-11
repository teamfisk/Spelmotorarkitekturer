#ifndef Resource_h__
#define Resource_h__

#include <list>
#include "Logging.h"

class Resource
{
	friend class IResourceHandle;
	friend class ResourceManager;
public:
	struct StillLoadingException : public std::runtime_error
    {
        virtual const char* what() const throw()
        {
            return "Resource is still loading.";
        }
    };
    struct FailedLoadingException : public std::runtime_error
    {
        FailedLoadingException(char const* const message)
            : std::runtime_error(message)
        { }
        FailedLoadingException() : std::runtime_error("Resource failed to load.") { };
    };

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
