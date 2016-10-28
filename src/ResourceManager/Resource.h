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
	virtual ~Resource() = default;

    // Called by ResourceManager from main thread after constructor is finished 
	// (Useful for e.g. OpenGL calls that needs to be run in a specific context)
	// NEEDS to return the total number of bytes of any internal allocations of the resource!
	virtual std::size_t Finalize() = 0;
	// Called by ResourceManager from main thread before destroying the resource
	virtual void Destroy() { }

	// Utility functions to track used memory
	void LogMemoryUsed(std::size_t size) { m_UsedMemory += size; }
	void LogMemoryFreed(std::size_t size) { m_UsedMemory -= size; }
	std::size_t m_UsedMemory = 0;

public:
    virtual bool Valid() const { return m_Finalized; }

private:
	bool m_Finalized = false;
};

#endif
