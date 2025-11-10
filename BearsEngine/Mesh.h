#pragma once

#include <vector>
#include "Renderable.h"

class MeshGrid : public Renderable
{
public:
	MeshGrid(const char* p_vertexPath, const char* p_trianglePath);
	~MeshGrid();
	void Render(Shader& shader) override;

private:
	void _readVerticesAndIndices(std::istream& p_vertexFile, std::istream& p_triangleFile, std::vector<float>& vertices, std::vector<unsigned int>& triangles);

	unsigned int m_noOfVertices;
	unsigned int m_noOfIndices;
	unsigned int m_VAO, m_VBO, m_EBO;
};