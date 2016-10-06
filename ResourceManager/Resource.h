#ifndef Resource_h__
#define Resource_h__

class Resource
{
	friend class ResourceManager;

protected:
	Resource() = default;
	virtual Resource() = default;

public:
	struct StillLoadingException : std::exception { };
	struct FailedLoadingException : std::exception
	{
		FailedLoadingException(const char* const message)
			: std::exception(message)
		{ }
	};
};

class ThreadUnsafeResource : public Resource
{
	friend class ResourceManager;
};

#endif
