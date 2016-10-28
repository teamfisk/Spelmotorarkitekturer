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

    // Guaranteed to be called on main thread after constructor is finished
	virtual void Finalize() { m_Finalized = true; }

	//Run this function when you use memory from any resource.
	void LogMemoryUsed(size_t size) { m_UsedMemory += size; }
	//Run this function when you delete memory in any resource.
	void LogMemoryFreed(size_t size) { m_UsedMemory -= size; }

	std::size_t m_UsedMemory = 0;

public:
    virtual bool Valid() const { return m_Finalized; }
	// This needs to be implemented to return the allocated size of the resource
	virtual std::size_t Size() = 0;

private:
	bool m_Finalized = false;
};

#endif
