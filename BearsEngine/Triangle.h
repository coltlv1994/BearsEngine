#pragma once
#include "Renderable.h"
class Triangle : public Renderable
{
public:
	Triangle(const float* p_vertices, const float* p_colors);
	~Triangle();
	void Render(Shader& shader) override;

private:
	float m_vertices[9] = { 0 };
	float m_colors[9] = { 0 };
	unsigned int m_VAO, m_VBO;

	void _determineVerticesOrder();
	void _generateBuffers();
};