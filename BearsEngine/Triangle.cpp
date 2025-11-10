#include "Triangle.h"

#include <cstring>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

Triangle::Triangle(const float* p_vertices, const float* p_colors)
{
	memcpy(m_vertices, p_vertices, sizeof(float) * 9);
	memcpy(m_colors, p_colors, sizeof(float) * 9);
	_determineVerticesOrder();
	_generateBuffers();
}

Triangle::~Triangle()
{
	if (m_VBO)
	{
		glDeleteBuffers(1, &m_VBO);
	}

	if (m_VAO)
	{
		glDeleteVertexArrays(1, &m_VAO);
	}
}

void Triangle::Render(Shader& p_shader)
{
	p_shader.Use();

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Triangle::_determineVerticesOrder()
{
	// firstly, check if these three vertices can form a triangle

	// secondly, check if vertices are arranged counterclockwise

}

void Triangle::_generateBuffers()
{
	// aggregate vertices and their colors
	// total of 18 floats are needed
	float buffer[18] = { 0 };

	for (int i = 0; i < 3; i++)
	{
		// vertices
		memcpy(&buffer[6 * i], &m_vertices[3 * i], sizeof(float) * 3);

		// colors
		memcpy(&buffer[6 * i + 3], &m_colors[3 * i], sizeof(float) * 3);
	}


	glGenBuffers(1, &m_VBO);
	glGenVertexArrays(1, &m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindVertexArray(m_VAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

	// vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}