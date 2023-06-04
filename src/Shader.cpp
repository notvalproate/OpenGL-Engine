#include "Shader.h"
#include "ErrorChecking.h"
#include <iostream>
#include <fstream>

Shader::Shader(const std::string& filepath) {
	std::ifstream stream(filepath);
	std::string buffer, line;

	while (getline(stream, line)) {
		if (line == "#shader fragment") {
			vertexShader = buffer;
			buffer.clear();
			continue;
		}
		buffer += line;
		buffer += "\n";
	}
	fragmentShader = buffer;

	stream.close();

	m_RendererID = glCreateProgram();
	std::cout << m_RendererID;
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(m_RendererID, vs));
	GLCall(glAttachShader(m_RendererID, fs));
	GLCall(glLinkProgram(m_RendererID));
	GLCall(glValidateProgram(m_RendererID));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	GLCall(glUseProgram(m_RendererID));
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, NULL));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Error: " << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}
	
	return id;
}

void Shader::SetUniform1i(const std::string& name, const int& v1) {
	GLCall(glUniform1i(GetUniformLocation(name), v1));
}

void Shader::SetUniform1f(const std::string& name, const float& v1) {
	GLCall(glUniform1f(GetUniformLocation(name), v1));
}

void Shader::SetUniform1iv(const std::string& name, const int& size, const int* v1) {
	GLCall(glUniform1iv(GetUniformLocation(name), size, v1));
}

void Shader::SetUniform2f(const std::string& name, const float& v1, const float& v2){
	GLCall(glUniform2f(GetUniformLocation(name), v1, v2));
}

void Shader::SetUniform3fv(const std::string& name, const int& size, const float* v1) {
	GLCall(glUniform3fv(GetUniformLocation(name), size, v1));
}

void Shader::SetUniform3f(const std::string& name, const float& v1, const float& v2, const float& v3) {
	GLCall(glUniform3f(GetUniformLocation(name), v1, v2, v3));
}

void Shader::SetUniform4f(const std::string& name, const float& v1, const float& v2, const float& v3, const float& v4) {
	GLCall(glUniform4f(GetUniformLocation(name), v1, v2, v3, v4));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0])); 
}

void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix) {
	GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetMVP(const std::string& m, const glm::mat4& model, const std::string& v, const glm::mat4& view, const std::string& p, const glm::mat4& projection) {
	SetUniformMat4f(m, model);
	SetUniformMat4f(v, view);
	SetUniformMat4f(p, projection);
}

void Shader::SetModel(const std::string& m, const std::string& n, const glm::mat4& model) {
	SetUniformMat4f(m, model);
	SetUniformMat3f(n, glm::mat3(glm::transpose(glm::inverse(model))));
}

unsigned int Shader::GetUniformLocation(const std::string& name) {
	Bind();
	GLCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1) {
		std::cout << "Warning: Uniform " << name << " doesn't exist!" << std::endl;
	}
	return location;
}

void Shader::EnableBlending() const {
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Shader::Bind() const {
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}