#include "Lighting.h"

//Base Lighting

void BaseLight::setBaseAttribs(const glm::vec3& coloring, const float& amb, const float& diff, const float& bright) {
	ambientStrength = amb;
	diffusionStrength = diff;
	brightness = bright;

	color = coloring;
	ambient = ambientStrength * color;
	diffuse = diffusionStrength * color;
	needUpdate = true;
}

void BaseLight::setAmbience(const float& amb) {
	ambientStrength = amb;
	ambient = ambientStrength * color;
	needUpdate = true;
}

void BaseLight::setDiffuse(const float& diff) {
	diffusionStrength = diff;
	diffuse = diffusionStrength * color;
	needUpdate = true;
};

void BaseLight::setBrightness(const float& bright) {
	brightness = bright;
	needUpdate = true;
};

void BaseLight::setColor(const glm::vec3& coloring) {
	color = coloring;
	ambient = ambientStrength * color;
	diffuse = diffusionStrength * color;
	needUpdate = true;
};

//Directional Attributes

DirLight::DirLight() {
	direction = glm::vec3(1.0f);
	setBaseAttribs(glm::vec3(1.0f), 0.5f, 1.0f, 1.0f);
}

DirLight::DirLight(const glm::vec3& dir, const glm::vec3& coloring, const float& brightness) {
	direction = dir;
	setBaseAttribs(coloring, 0.5f, 1.0f, brightness);
}

void DirLight::setDirection(const glm::vec3& dir) {
	direction = dir;
	needUpdate = true;
}

void DirLight::updateUniforms(Shader& shader) {
	if (needUpdate) {
		shader.Bind();

		shader.SetUniform3fv("u_dLight.direction", 1, &direction[0]);
		shader.SetUniform3fv("u_dLight.ambient", 1, &ambient[0]);
		shader.SetUniform3fv("u_dLight.diffusion", 1, &diffuse[0]);
		shader.SetUniform1f("u_dLight.brightness", brightness);

		needUpdate = false;
	}
}

//Point Attributes

PointLight::PointLight(const glm::vec3& pos, const glm::vec3& coloring, const float& brightness, const std::string& ind) {
	index = ind;
	position = pos;
	lampModel = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(0.2));
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;
	setBaseAttribs(coloring, 0.0f, 1.0f, brightness);
}

void PointLight::updateUniforms(Shader& shader) {
	if (needUpdate) {
		shader.Bind();

		shader.SetUniform3fv(index + ".position", 1, &position[0]);
		shader.SetUniform3fv(index + ".ambient", 1, &ambient[0]);
		shader.SetUniform3fv(index + ".diffusion", 1, &diffuse[0]);
		shader.SetUniform1f(index + ".brightness", brightness);

		shader.SetUniform1f(index + ".Kc", constant);
		shader.SetUniform1f(index + ".Kl", linear);
		shader.SetUniform1f(index + ".Kq", quadratic);

		needUpdate = false;
	}
}

//Point Light List

void PointLightList::Push(const glm::vec3& pos, const glm::vec3& coloring, const float& brightness) {
	std::string index = "u_pLight[" + std::to_string(list.size()) + "]";
	list.push_back(PointLight(pos, coloring, brightness, index));
}

void PointLightList::PushList(const std::vector<glm::vec3>& attribs) {
	for (int i = 0; i < attribs.size() / 3; i++) {
		std::string index = "u_pLight[" + std::to_string(list.size()) + "]";
		list.push_back(PointLight(attribs[3*i], attribs[3*i +1], attribs[3*i + 2].x, index));
	}
}

void PointLightList::UpdateList(const std::vector<glm::vec3>& attribs, const std::vector<float>& bright) {
	for (unsigned int i = 0; i < list.size() || i < attribs.size() / 2; i++) {
		list[i].setPosition(attribs[2 * i]);
		list[i].setColor(attribs[2 * i + 1]);
		list[i].setBrightness(bright[i]);
	}
}

void PointLightList::Delete(const int& i) {
	list.erase(list.begin() + i);
	std::string index;
	for (unsigned int j = i; j < list.size(); j++) {
		index = "u_pLight[" + std::to_string(j) + "]";
		list[j].setIndex(index);
	}
}

void PointLightList::Draw(Mesh& lightmesh, Shader& shader, const glm::mat4& view, const glm::mat4& projection) {
	for (unsigned int i = 0; i < list.size(); i++) {
		shader.Bind();
		shader.SetUniform3fv("u_LampColor", 1, &list[i].getColor()[0]);
		shader.SetUniform1f("u_Material.shininess", list[i].getBrightness());
		shader.SetMVP("ul_Model", list[i].getLampModel(), "ul_View", view, "ul_Projection", projection);
		lightmesh.Draw(shader);
	}
}

void PointLightList::updateUniforms(Shader& shader) {
	for (unsigned int i = 0; i < list.size(); i++) {
		list[i].updateUniforms(shader);
	}
}

//SPOTLIGHT

SpotLight::SpotLight(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& coloring, const float& brightness, const std::string& ind) {
	position = pos;
	direction = dir; 

	constant = 1.0f;
	linear = 0.027f;
	quadratic = 0.0028f;

	cutOff = 10.0f;
	outerCutOff = 25.0f;

	lampModel = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(0.2));
	lightSpace = glm::lookAt(pos, pos + dir, glm::vec3(0.0, 1.0, 0.0));

	index = ind;

	setBaseAttribs(coloring, 0.0f, 1.0f, brightness);
}

void SpotLight::updateUniforms(Shader& shader) {
	if (needUpdate) {
		shader.SetUniform3fv(index + ".position", 1, &position[0]);
		shader.SetUniform3fv(index + ".direction", 1, &direction[0]);

		shader.SetUniform3fv(index + ".ambient", 1, &ambient[0]);
		shader.SetUniform3fv(index + ".diffusion", 1, &diffuse[0]);

		shader.SetUniform1f(index + ".Kc", constant);
		shader.SetUniform1f(index + ".Kl", linear);
		shader.SetUniform1f(index + ".Kq", quadratic);

		shader.SetUniform1f(index + ".cutOff", glm::cos(glm::radians(cutOff)));
		shader.SetUniform1f(index + ".outerCutOff", glm::cos(glm::radians(outerCutOff)));

		shader.SetUniform1i("u_Flashlight", 7);
		std::string lightspaceind = "u_Lightspace[" + index[9];
		lightspaceind += "]";
		shader.SetUniformMat4f(lightspaceind, lightSpace);

		shader.SetUniform1f(index + ".brightness", brightness);
	}
}

//SPOTLIGHT LIST

void SpotLightList::Push(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& coloring, const float& brightness) {
	std::string index = "u_sLight[" + std::to_string(list.size()) + "]";
	list.push_back(SpotLight(pos, dir, coloring, brightness, index));
}

void SpotLightList::PushList(const int& total, const std::vector<glm::vec3>& attribs, const std::vector<float>& bright) {
	for (int i = 0; i < total; i++) {
		std::string index = "u_pLight[" + std::to_string(list.size()) + "]";
		list.push_back(SpotLight(attribs[3 * i], attribs[3 * i + 1], attribs[3 * i + 2], bright[i], index));
	}
}

void SpotLightList::updateUniforms(Shader& shader) {
	for (unsigned int i = 0; i < list.size(); i++) {
		list[i].updateUniforms(shader);
	}
}