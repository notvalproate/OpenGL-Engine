#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


class Renderer {
private:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const unsigned int& type) const;
public:
	void Clear(const float& r, const float& g, const float& b) const;
	void DrawTriangle(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawTriangleStrip(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawTriangleFan(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};