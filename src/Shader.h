#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader {
private:
	unsigned int m_RendererID;
	std::string filepath, vertexShader, fragmentShader;
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int GetUniformLocation(const std::string& name);
public:
	Shader(const std::string& filepath);

	void Bind() const;
	void Unbind() const; 

	void SetUniform1i(const std::string& name, const int& v1);
	void SetUniform1f(const std::string& name, const float& v1);
	void SetUniform1iv(const std::string& name, const int& size, const int* v1);
	void SetUniform2f(const std::string& name, const float& v1, const float& v2);
	void SetUniform3fv(const std::string& name, const int& size, const float* v1);
	void SetUniform3f(const std::string& name, const float& v1, const float& v2, const float& v3);
	void SetUniform4f(const std::string& name, const float& v1, const float& v2, const float& v3, const float& v4);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);

	void SetMVP(const std::string& m, const glm::mat4& model, const std::string& v, const glm::mat4& view, const std::string& p, const glm::mat4& projection);
	void SetModel(const std::string& m, const std::string& n, const glm::mat4& model);
	unsigned int GetProgramId() const { return m_RendererID; }
	void EnableBlending() const;
};