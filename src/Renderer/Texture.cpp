#include "Texture.h"

Texture::Texture(std::string path)
{
	ResourceHandle<PNG> handle = ResourceManager::Load<PNG>(path, 0);
	auto png = *handle;

	switch (png->Format)
	{
	case Image::ImageFormat::RGBA:
		m_Format = GL_RGBA;
		break;
	case Image::ImageFormat::RGB:
		m_Format = GL_RGB;
	default:
		throw std::runtime_error("Unsupported image format.");
		break;
	}

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, png->Width, png->Height, 0, m_Format, GL_UNSIGNED_BYTE, png->Data);	
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_Texture);
}

void Texture::Bind(GLenum textureUnit /*= GL_TEXTURE0*/, GLenum access /*= GL_READ_ONLY*/)
{		
	glBindImageTexture(textureUnit, m_Texture, 0, GL_FALSE, 0, access, m_Format);
}

