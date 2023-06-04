#include "Mesh.h"
#include <iostream>

Mesh::Mesh(const std::vector<float>& data, const std::vector<unsigned int> ind, const std::vector<Texture> tex) : indices(ind), textures(tex), shininess(32.0f) {
	VertexArray va;
	VertexBuffer vb(&data[0], sizeof(float) * data.size());

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);
	layout.Push<float>(1);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(&ind[0], ind.size());

	VAO = va.getID();
	IBO = ib.getID();
}

Mesh::Mesh(const std::vector<float>& data, const std::vector<unsigned int> ind, const std::vector<Texture> tex, const float& shiny) : indices(ind), textures(tex), shininess(shiny) {
	VertexArray va;
	VertexBuffer vb(&data[0], sizeof(float) * 12 * 24);

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);
	layout.Push<float>(1);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(&ind[0], ind.size());

	VAO = va.getID();
	IBO = ib.getID();
}

Mesh::Mesh(const std::vector<float>& data, const std::vector<unsigned int> ind, const std::vector<Texture> tex, const VertexBufferLayout& layout) : indices(ind), textures(tex), shininess(0) {
	VertexArray va;
	VertexBuffer vb(&data[0], sizeof(float) * 12 * 24);

	va.AddBuffer(vb, layout);

	IndexBuffer ib(&ind[0], ind.size());

	VAO = va.getID();
	IBO = ib.getID();
}

void Mesh::Draw(Shader& shader) {
	std::vector<int> diffSamplers;
	std::vector<int> specSamplers;
	std::vector<int> hudSamplers;
	int ishud = -1;

	for (unsigned int i = 0; i < textures.size(); i++) {
		if (textures[i].type == "diffuse") {
			diffSamplers.push_back(i);
			ishud = 0;
		}
		else if (textures[i].type == "specular") {
			specSamplers.push_back(i);
			ishud = 0;
		}
		else if (textures[i].type == "hud") {
			hudSamplers.push_back(i);
			ishud = 1;
		}
		textures[i].Bind(i);
	}

	if (ishud == 1) {
		shader.SetUniform1iv("u_Texture", hudSamplers.size(), &hudSamplers[0]);
	}
	else if(ishud == 0) {
		if (specSamplers.size() != 0) {
			shader.SetUniform1iv("u_Material.specular", specSamplers.size(), &specSamplers[0]);
		}
		if (shininess != 0.0f) {
			shader.SetUniform1f("u_Material.shininess", shininess);
		}
		shader.SetUniform1iv("u_Material.diffusion", diffSamplers.size(), &diffSamplers[0]);
	}

	GLCall(glBindVertexArray(VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, IBO));
	shader.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr));
}