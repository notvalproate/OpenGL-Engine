#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "Shader.h"
#include <vector>
#include <string>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Mesh.h"

class BaseLight {
protected:
	glm::vec3 color;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	
	float ambientStrength;
	float diffusionStrength;
	float brightness;

	bool needUpdate = true;
public:
	void setBaseAttribs(const glm::vec3& coloring, const float& amb, const float& diff, const float& bright);
	void setAmbience(const float& amb);
	void setDiffuse(const float& diff);
	void setBrightness(const float& bright);
	void setColor(const glm::vec3& coloring);

	glm::vec3 getColor() const { return color; }
};

class DirLight : public BaseLight {
private:
	glm::vec3 direction;
public:
	DirLight();
	DirLight(const glm::vec3& dir, const glm::vec3& coloring, const float& brightness);
	
	void setDirection(const glm::vec3& dir);
	void updateUniforms(Shader& shader);
};

class PointLight : public BaseLight {
protected:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	glm::mat4 lampModel;

	std::string index;
public:
	PointLight(const glm::vec3& pos, const glm::vec3& coloring, const float& brightness, const std::string& ind);

	void setPosition(const glm::vec3& pos) { position = pos; needUpdate = true; lampModel = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(0.2f)); }
	void setAttenuation(const float& c, const float& l, const float& q) { constant = c; linear = l; quadratic = q; needUpdate = true; }
	void setIndex(const std::string& ind) { index = ind; needUpdate = true; }
	void setModel(const glm::mat4& model) { lampModel = model; needUpdate = true; }
	void updateUniforms(Shader& shader);

	glm::mat4 getLampModel() const { return lampModel; }
	float getBrightness() const { return brightness; }
};

class PointLightList {
public:
	std::vector<PointLight> list;
	void Push(const glm::vec3& pos, const glm::vec3& coloring, const float& brightness);
	void PushList(const std::vector<glm::vec3>& attribs);
	void UpdateList(const std::vector<glm::vec3>& attribs, const std::vector<float>& bright);
	void Delete(const int& i);

	void Draw(Mesh& lightmesh, Shader& shader, const glm::mat4& view, const glm::mat4& projection);
	void updateUniforms(Shader& shader);
};

class SpotLight : private BaseLight {
private:
	glm::vec3 position;
	glm::vec3 direction;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;

	glm::mat4 lightSpace;
	glm::mat4 lampModel;

	std::string index;
public:
	SpotLight(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& coloring, const float& brightness, const std::string& ind);

	void setDirection(const glm::vec3& dir) { direction = dir; needUpdate = true; }
	void setCutOff(const float& inner, const float& outer) { cutOff = inner; outerCutOff = outer; needUpdate = true; }
	void setPosition(const glm::vec3& pos) { position = pos; needUpdate = true; lampModel = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(0.2f)); }
	void setAttenuation(const float& c, const float& l, const float& q) { constant = c; linear = l; quadratic = q; needUpdate = true; }
	void setIndex(const std::string& ind) { index = ind; needUpdate = true; }
	void setModel(const glm::mat4& model) { lampModel = model; needUpdate = true; }
	void updateUniforms(Shader& shader);
};

class SpotLightList {
public:
	std::vector<SpotLight> list;

	void Push(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& coloring, const float& brightness);
	void PushList(const int& total, const std::vector<glm::vec3>& attribs, const std::vector<float>& bright);
	void updateUniforms(Shader& shader);
};