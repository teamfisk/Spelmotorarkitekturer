#ifndef ResourceBundle_h__
#define ResourceBundle_h__

#include <string>

class ResourceBundle
{
public:
	ResourceBundle(const std::string& path)
		: m_BundlePath(path)
	{ }

	virtual std::size_t Read(const std::string& resourcePath, void* destination) = 0;

protected:
	std::string m_BundlePath;
};
#endif
