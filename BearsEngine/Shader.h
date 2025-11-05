#pragma once
#include <string>

class Shader
{
public:
	unsigned int m_shaderProgram;
	void Initialize(const char* p_vertexPath, const char* p_fragmentPath);
	void Use();

private:
	std::string _loadShader(const char* p_path);
	unsigned int _loadVertexShader(const char* p_path);
	unsigned int _loadFragmentShader(const char* p_path);
};