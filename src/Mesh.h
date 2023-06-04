#include "glm/glm.hpp"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "ErrorChecking.h"
#include <vector>

class Mesh {
private:
	unsigned int VAO, IBO;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;
	float shininess;
public:
	Mesh(const std::vector<float>& data, const std::vector<unsigned int> ind, const std::vector<Texture> tex);
	Mesh(const std::vector<float>& data, const std::vector<unsigned int> ind, const std::vector<Texture> tex, const float& shininess);
	Mesh(const std::vector<float>& data, const std::vector<unsigned int> ind, const std::vector<Texture> tex, const VertexBufferLayout& layout);

	void Draw(Shader& shader);
};
