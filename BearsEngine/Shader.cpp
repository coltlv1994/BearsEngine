#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader.h"

Shader::Shader(const char* p_vertexPath, const char* p_fragmentPath)
{
	m_shaderProgramID = glCreateProgram();
	//_loadShader2(p_vertexPath, p_fragmentPath);

	_loadShader(p_vertexPath, GL_VERTEX_SHADER);
	_loadShader(p_fragmentPath, GL_FRAGMENT_SHADER);
}

void Shader::Use()
{
	glUseProgram(m_shaderProgramID);
}

// _loadShader() function should only be called after "m_shaderProgramID = glCreateProgram();"
void Shader::_loadShader(const char* p_path, unsigned int p_shaderClass)
{
	Use();

	std::string shaderText;
	std::ifstream shaderFile;

	try
	{
		shaderFile.open(p_path);

		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();

		shaderText = shaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR: Shader read fail. " << std::endl << e.what() << std::endl;
	}

	const char* shaderCode = shaderText.c_str();

	unsigned int shader;

	shader = glCreateShader(p_shaderClass);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	_checkCompileErrors(shader, false);

	glAttachShader(m_shaderProgramID, shader);
	glLinkProgram(m_shaderProgramID);
	_checkCompileErrors(m_shaderProgramID, true);
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(shader);
}

// utility uniform functions
// ------------------------------------------------------------------------
void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, &value[0]);
}
void Shader::SetVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(m_shaderProgramID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, &value[0]);
}
void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, &value[0]);
}
void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(m_shaderProgramID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::_checkCompileErrors(unsigned int p_shader, bool p_errorType)
{
	int success;
	char log[1024];

	if (p_errorType == false)
	{
		// Shader compiling errors
		glGetShaderiv(p_shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(p_shader, 1024, NULL, log);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR" << std::endl << log << std::endl;
		}
	}
	else
	{
		// Shader linking errors
		glGetProgramiv(p_shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(p_shader, 1024, NULL, log);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type" << std::endl << log << std::endl;
		}
	}
}