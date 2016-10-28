#ifndef Texture_h__
#define Texture_h__

//#include <boost/filesystem.hpp>
#include "OpenGL.h"
#include "PNG.h"

class Texture : public Resource
{
	friend class ResourceManager;

protected:
	Texture(std::shared_ptr<ResourceBundle::Block> block);
	void Finalize() override;

public:
	~Texture();

	void Bind(GLenum textureUnit = GL_TEXTURE0, GLenum access = GL_READ_ONLY);

private:
	GLuint m_Texture = 0;
	GLenum m_Format = 0;
	enum TextureType : GLint { cInvalid = 0, cDDS = 1, cPNG = 2};
	TextureType m_Type;

	ResourceHandle<PNG> m_PNG;
private:
	//TODO: Make sure that all memory used is logged.
	std::size_t Size() { return m_UsedMemory; }
};

#endif 