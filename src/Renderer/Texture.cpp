#include "Texture.h"

Texture::Texture(std::shared_ptr<ResourceBundle::Block> block)
{
	m_PNG = ResourceManager::Load<PNG>(block->Path());

	switch (m_PNG->Format) {
		case Image::ImageFormat::RGBA:
			m_Format = GL_RGBA;
			break;
		case Image::ImageFormat::RGB:
			m_Format = GL_RGB;
            break;
		default:
			throw std::runtime_error("Unsupported image format.");
	}
}

void Texture::Finalize()
{
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_PNG->Width, m_PNG->Height, 0, m_Format, GL_UNSIGNED_BYTE, m_PNG->Data);
	glBindTexture(GL_TEXTURE_2D, 0);

	Resource::Finalize();
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_Texture);
}

void Texture::Bind(GLenum textureUnit /*= GL_TEXTURE0*/, GLenum access /*= GL_READ_ONLY*/)
{		
	glBindImageTexture(textureUnit, m_Texture, 0, GL_FALSE, 0, access, m_Format);
}

