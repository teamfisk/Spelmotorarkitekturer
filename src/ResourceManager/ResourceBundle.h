#ifndef ResourceBundle_h__
#define ResourceBundle_h__

#include <string>
#include "ResourceTreeNode.h"

class ResourceBundle
{
public:
	ResourceBundle(const std::string& path)
		: m_BundlePath(path)
	{ }

protected:
	std::string m_BundlePath;
    ResourceTreeNode m_RootNode;
};

#endif
