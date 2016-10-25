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
	virtual ~Resource()
	{
		if (m_ReferenceCount != 0) {
			LOG_ERROR("Resource was freed while still carrying references!");
		}
	}

    // Guaranteed to be called on main thread after constructor is finished
	virtual void Finalize() { m_Finalized = true; }

public:
    virtual bool Valid() const { return m_Finalized; }

private:
	unsigned int m_ReferenceCount = 0;
	bool m_Finalized = false;
};

#endif
