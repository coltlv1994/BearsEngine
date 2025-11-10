#include "Mesh.h"

#include <iostream>
#include <vector>
#include <fstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

MeshGrid::MeshGrid(const char* p_vertexPath, const char* p_trianglePath)
{
	std::vector<float> vertices;
	std::vector<unsigned int> triangles;

    std::ifstream vertexFile(p_vertexPath);
    std::ifstream triangleFile(p_trianglePath);

    if (vertexFile.is_open() && triangleFile.is_open())
    {
        // open is good
        _readVerticesAndIndices(vertexFile, triangleFile, vertices, triangles);
    }
    else
    {
        exit(1);
    }

    vertexFile.close();
    triangleFile.close();

    m_noOfVertices = vertices.size();
    m_noOfIndices = triangles.size();

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_noOfVertices * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_noOfIndices * sizeof(unsigned int), triangles.data(), GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal vector, it is after vertex' position information
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
    shader.Use();

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_noOfIndices, GL_UNSIGNED_INT, 0);
}

void MeshGrid::_readVerticesAndIndices(std::istream& p_vertexFile, std::istream& p_triangleFile, std::vector<float>& p_vertices, std::vector<unsigned int>& p_triangles)
{
    float x, y, z, nx, ny, nz;

    while (p_vertexFile >> x >> y >> z >> nx >> ny >> nz)
    {
        p_vertices.push_back(x);
        p_vertices.push_back(y);
        p_vertices.push_back(z);
        p_vertices.push_back(nx);
        p_vertices.push_back(ny);
        p_vertices.push_back(nz);
    }

    unsigned int index0, index1, index2;

    while (p_triangleFile >> index0 >> index1 >> index2)
    {
        p_triangles.push_back(index0);
        p_triangles.push_back(index1);
        p_triangles.push_back(index2);
    }
}