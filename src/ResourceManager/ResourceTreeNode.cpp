#include "ResourceTreeNode.h"

void ResourceTreeNode::Insert(const std::string& path, ResourceTreeNode* node, ResourceTreeNode* parent /* = nullptr */)
{
    if (parent == nullptr) {
        parent = this;
    }

    if (path.empty()) {
        // Insert
        parent->Children.push_back(node);
        node->Parent = parent;
    } else {
        // Traverse
        unsigned long firstDelim = path.find('/');
        std::string segment = path.substr(0, firstDelim);
        for (auto& child : parent->Children) {
            if (child->Name == segment) {
                std::string nextSegment;
                if (firstDelim != std::string::npos) {
                    nextSegment = path.substr(firstDelim + 1);
                }
                return Insert(nextSegment, node, child);
            }
        }
    }
}

ResourceTreeNode* ResourceTreeNode::Search(const std::string& path, ResourceTreeNode* parent /* = nullptr */)
{
    if (parent == nullptr) {
        parent = this;
    }

    unsigned long firstDelim = path.find('/');
    std::string segment = path.substr(0, firstDelim);
    for (auto& child : parent->Children) {
        if (child->Name == segment) {
            std::string nextSegment = path.substr(firstDelim + 1);
            return Search(nextSegment, child);
        }
    }

    return parent;
}
