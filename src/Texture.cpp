#include "Texture.h"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); //4 channels, recommended for PNG

	/* glGenTextures() generates a texture name in [m_RendererID] */
	GLCall(glGenTextures(1, &m_RendererID));
	/* glBindTexture() binds a texture named [m_RendererID]
	   to the texturing target [GL_TEXTURE_2D] */
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	/* glTexParameteri() sets the texture parameters */
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	/* glTexImage2D() specifies a two-dimensional texture image */
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer ));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	/* glDeleteTextures() deletes texture named [m_RendererID] */
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	/* glActiveTexture() selects the active texture unit 
	   subsequent texture state calls will affect */
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	/* glBindTexture() binds a texture named [m_RendererID] 
	   to the texturing target [GL_TEXTURE_2D] */
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind()
{
	/* glBindTexture(0) binds the Default Texture
	   to the texturing target [GL_TEXTURE_2D] */
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
