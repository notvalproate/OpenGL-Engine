#include "Texture.h"
#include "stb_image/stb_image.h"
#include "ErrorChecking.h"

Texture::Texture(const std::string& t, const std::string& path, const int& param, const int& wrapping) : m_RendererID(0), m_Path(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), type(t) {

	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	Bind();

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	Unbind();

	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer);
	}
}

void Texture::Bind(unsigned int slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}