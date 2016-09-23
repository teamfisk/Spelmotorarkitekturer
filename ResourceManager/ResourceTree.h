#ifndef ResourceTree_h__
#define ResourceTree_h__

#include <vector>
#include <string>

class ResourceTree
{
public:
	class Node
	{
		std::string Name;
		void* Data = nullptr;
		std::size_t Size = 0;

		Node* Parent = nullptr;
		std::vector<Node*> Children;
	};

	void Insert(const std::string& path, void* data = nullptr)
	{
		
	}

	const Node* Find(const std::string& path)
	{
		
	}

private:
	Node* m_Root = nullptr;
};

#endif
