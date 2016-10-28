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
	std::size_t Finalize() override;
	void Destroy() override;

public:
	void Bind(GLenum textureUnit = GL_TEXTURE0, GLenum access = GL_READ_ONLY);

private:
	ResourceHandle<PNG> m_PNG;
	GLuint m_Texture = 0;
	GLenum m_Format = 0;
	enum TextureType : GLint { cInvalid = 0, cDDS = 1, cPNG = 2};
	TextureType m_Type;
};

#endif 