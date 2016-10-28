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

	//Run this function when you use memory from any resource.
	void LogMemoryUsed(size_t size) { m_UsedMemory += size; }
	//Run this function when you delete memory in any resource.
	void LogMemoryFreed(size_t size) { m_UsedMemory -= size; }

	size_t m_UsedMemory = 0;

public:
    virtual bool Valid() const { return true; }
	virtual size_t Size() = 0;

private:

	unsigned int m_ReferenceCount = 0;
    std::list<Resource**>::iterator m_ActivityIterator;
};

class ThreadUnsafeResource : public Resource
{
	friend class ResourceManager;
};

#endif
