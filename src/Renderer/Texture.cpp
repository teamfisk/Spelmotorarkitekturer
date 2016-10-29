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

	m_PNG.Release();
}

std::size_t Texture::Size()
{
	return 0;
}

void Texture::Destroy()
{
	glDeleteTextures(1, &m_Texture);
}

void Texture::Bind(GLenum textureUnit /*= GL_TEXTURE0*/, GLenum access /*= GL_READ_ONLY*/)
{		
	glActiveTexture(textureUnit);	
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	
	// Think this one is for binding to image2D, not sampler2D.
	//glBindImageTexture(0/*this refers to the "image unit", which is apparently different from "texture image unit" i.e. GL_TEXTURE0, GL_TEXTURE1, etc.*/, 
	//	m_Texture, 0, GL_FALSE, 0, access, GL_RGBA32F);	// GL_RGBA seems to not be in this table https://www.opengl.org/sdk/docs/man4/html/glBindImageTexture.xhtml	
}

