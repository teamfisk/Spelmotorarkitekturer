#ifndef ResourceNode_h__
#define ResourceNode_h__

#include <vector>
#include <string>

struct ResourceTreeNode
{
	std::string Name;
	std::size_t Size = 0;
    std::size_t Offset = 0;

	ResourceTreeNode* Parent = nullptr;
	std::vector<ResourceTreeNode*> Children;

    virtual void Insert(const std::string& path, ResourceTreeNode* node, ResourceTreeNode* parent = nullptr);
    virtual ResourceTreeNode* Search(const std::string& path, ResourceTreeNode* parent = nullptr);
};

#endif
