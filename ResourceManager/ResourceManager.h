#ifndef ResourceManager_h__
#define ResourceManager_h__

class ResourceManager
{
private:
	ResourceManager() = default;

public:
	template <typename T, bool async = true>
	static T* Load(const char* path, 
};

#endif
