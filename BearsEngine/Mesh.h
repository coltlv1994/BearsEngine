#pragma once

#include <vector>
#include "Renderable.h"

class MeshGrid : public Renderable
{
public:
	MeshGrid(const char* p_vertexPath, const char* p_trianglePath, const char* p_texturePath);
	MeshGrid(const unsigned int p_noOfXSeg, const unsigned int p_noOfYSeg, const char* p_texturePath);
	~MeshGrid();
	void Render(Shader& shader) override;

private:
	void _readVerticesAndIndices(const char* p_vertexPath, const char* p_trianglePath, std::vector<float>& vertices, std::vector<unsigned int>& triangles);
	void _createSphere(const unsigned int p_noOfXSeg, const unsigned int p_noOfYSeg, std::vector<float>& vertices, std::vector<unsigned int>& triangles);
	void _generateBuffers(std::vector<float>& vertices, std::vector<unsigned int>& triangles);
	void _createTexture(const char* p_texturePath);
	unsigned int m_noOfVertices;
	unsigned int m_noOfIndices;
	unsigned int m_VAO, m_VBO, m_EBO;
	unsigned int m_texture;
};