#ifndef Resource_h__
#define Resource_h__

#include <list>
#include "Logging.h"
#include "ResourceBundle.h"

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
	Resource(std::shared_ptr<ResourceBundle::Block> block) { }
	virtual ~Resource()
	{
		if (m_ReferenceCount != 0) {
			LOG_ERROR("Resource was freed while still carrying references!");
		}
	}

public:
    virtual bool Valid() const { return true; }

private:
	unsigned int m_ReferenceCount = 0;
    std::list<Resource**>::iterator m_ActivityIterator;
};

class ThreadUnsafeResource : public Resource
{
	friend class ResourceManager;
};

#endif
