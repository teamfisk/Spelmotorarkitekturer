#ifndef BaseTexture_h__
#define BaseTexture_h__

#include "ResourceManager/Resource.h"

class BaseTexture : public ThreadUnsafeResource
{
	friend class ResourceManager;

public:
	unsigned int Width = 0;
	unsigned int Height = 0;
};

#endif