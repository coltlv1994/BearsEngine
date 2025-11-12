#include "Mesh.h"

#include <iostream>
#include <vector>
#include <fstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#ifndef IMAGES_H
#define IMAGES_H
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#endif // IMAGES_H

MeshGrid::MeshGrid(const char* p_vertexPath, const char* p_trianglePath, const char* p_texturePath)
{
	std::vector<float> vertices;
	std::vector<unsigned int> triangles;

	_readVerticesAndIndices(p_vertexPath, p_trianglePath, vertices, triangles);

	_generateBuffers(vertices, triangles);

	_createTexture(p_texturePath);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

MeshGrid::MeshGrid(const unsigned int p_noOfXSeg, const unsigned int p_noOfYSeg, const char* p_texturePath)
{
	std::vector<float> vertices;
	std::vector<unsigned int> triangles;

	_createSphere(p_noOfXSeg, p_noOfYSeg, vertices, triangles);

	_generateBuffers(vertices, triangles);

	_createTexture(p_texturePath);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



MeshGrid::~MeshGrid()
{
	if (m_VBO)
	{
		glDeleteBuffers(1, &m_VBO);
	}

	if (m_VAO)
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	if (m_EBO)
	{
		glDeleteBuffers(1, &m_EBO);
	}
}

void MeshGrid::Render(Shader& shader)
{
	// bind Texture
	glBindTexture(GL_TEXTURE_2D, m_texture);

	shader.Use();

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_noOfIndices, GL_UNSIGNED_INT, 0);
}

void MeshGrid::_readVerticesAndIndices(const char* p_vertexPath, const char* p_trianglePath, std::vector<float>& p_vertices, std::vector<unsigned int>& p_triangles)
{
	std::ifstream vertexFile(p_vertexPath);
	std::ifstream triangleFile(p_trianglePath);

	if (!vertexFile.is_open() || !triangleFile.is_open())
	{
		// open failed
		exit(1);
	}

	float x, y, z, nx, ny, nz;

	constexpr float _PI = glm::pi<float>();

	while (vertexFile >> x >> y >> z >> nx >> ny >> nz)
	{
		p_vertices.push_back(x);
		p_vertices.push_back(y);
		p_vertices.push_back(z);
		p_vertices.push_back(nx);
		p_vertices.push_back(ny);
		p_vertices.push_back(nz);

		// calculate texture coordinate
		if (y != -1.0f && y != 1.0f)
		{
			//regular dots
			p_vertices.push_back(1.0f - (glm::atan(z, x) / _PI + 1.0f) / 2.0f);
			p_vertices.push_back(1.0f - (y + 1.0f) / 2.0f);
		}
		else
		{
			if (y == -1.0f)
			{
				// special treatment
				p_vertices.push_back(0.0f);
				p_vertices.push_back(1.0f);
			}
			else
			{
				p_vertices.push_back(0.0f);
				p_vertices.push_back(0.0f);
			}
		}
	}

	unsigned int index0, index1, index2;

	while (triangleFile >> index0 >> index1 >> index2)
	{
		p_triangles.push_back(index0);
		p_triangles.push_back(index1);
		p_triangles.push_back(index2);
	}

	m_noOfVertices = p_vertices.size();
	m_noOfIndices = p_triangles.size();
}

void MeshGrid::_createSphere(const unsigned int p_noOfXSeg, const unsigned int p_noOfYSeg, std::vector<float>& p_vertices, std::vector<unsigned int>& p_triangles)
{

	for (unsigned int y = 0; y <= p_noOfYSeg; ++y)
	{
		for (unsigned int x = 0; x <= p_noOfXSeg; ++x)
		{
			// Create x & y segments
			float xSegments = static_cast<float>(x) / static_cast<float>(p_noOfXSeg);
			float ySegments = static_cast<float>(y) / static_cast<float>(p_noOfYSeg);

			float xPos = std::cos(xSegments * 2.0f * glm::pi<float>()) * std::sin(ySegments * glm::pi<float>());
			float yPos = std::cos(ySegments * glm::pi<float>());
			float zPos = std::sin(xSegments * 2.0f * glm::pi<float>()) * std::sin(ySegments * glm::pi<float>());

			// Set position vertices
			p_vertices.push_back(xPos);
			p_vertices.push_back(yPos);
			p_vertices.push_back(zPos);

			// Set Normal vertices (color)
			p_vertices.push_back(xPos);
			p_vertices.push_back(yPos);
			p_vertices.push_back(zPos);

			// Set TexCoords vertices
			p_vertices.push_back(1.0f - xSegments);
			p_vertices.push_back(ySegments);
		}
	}

	for (unsigned int y = 0; y < p_noOfYSeg; ++y)
	{
		for (unsigned int x = 0; x < p_noOfXSeg; ++x)
		{

			unsigned int topLeft = y * (p_noOfXSeg + 1) + x;
			unsigned int bottomLeft = (y + 1) * (p_noOfXSeg + 1) + x;

			p_triangles.push_back(topLeft);
			p_triangles.push_back(topLeft + 1);
			p_triangles.push_back(bottomLeft);

			p_triangles.push_back(topLeft + 1);
			p_triangles.push_back(bottomLeft + 1);
			p_triangles.push_back(bottomLeft);
		}
	}

	m_noOfVertices = p_vertices.size();
	m_noOfIndices = p_triangles.size();
}

void MeshGrid::_generateBuffers(std::vector<float>& p_vertices, std::vector<unsigned int>& p_triangles)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_noOfVertices * sizeof(float), p_vertices.data(), GL_STATIC_DRAW);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_noOfIndices * sizeof(unsigned int), p_triangles.data(), GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal vector, it is after vertex' position information
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void MeshGrid::_createTexture(const char* p_texturePath)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(p_texturePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}