#include "Renderer.h"
#include "ErrorChecking.h"

void Renderer::Clear(const float& r, const float& g, const float& b) const {
	GLCall(glClearColor(r, g, b, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const unsigned int& type) const {
	va.Bind();
	ib.Bind();
	shader.Bind();
	GLCall(glDrawElements(type, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawTriangle(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	Draw(va, ib, shader, GL_TRIANGLES);
}

void Renderer::DrawTriangleStrip(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	Draw(va, ib, shader, GL_TRIANGLE_STRIP);
}

void Renderer::DrawTriangleFan(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	Draw(va, ib, shader, GL_TRIANGLE_FAN);
}