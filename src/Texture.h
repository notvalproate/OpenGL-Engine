#pragma once
#include <string>
#include <GL/glew.h>

class Texture {
private:
	unsigned int m_RendererID;
	std::string m_Path;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	std::string type;

	Texture(const std::string& t, const std::string& path, const int& param, const int& wrapping);

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline std::string GetPath() const { return m_Path; }
};