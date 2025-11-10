#pragma once
#include <string>
#include "glm/glm.hpp"

class Shader
{
public:
	unsigned int m_shaderProgramID;
	
	Shader(const char* p_vertexPath, const char* p_fragmentPath);
	void Use();

    // utility uniform functions
    // ------------------------------------------------------------------------
    void SetBool(const std::string& name, bool value) const;
    // ------------------------------------------------------------------------
    void SetInt(const std::string& name, int value) const;
    // ------------------------------------------------------------------------
    void SetFloat(const std::string& name, float value) const;
    // ------------------------------------------------------------------------
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec2(const std::string& name, float x, float y) const;
    // ------------------------------------------------------------------------
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;
    // ------------------------------------------------------------------------
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetVec4(const std::string& name, float x, float y, float z, float w) const;
    // ------------------------------------------------------------------------
    void SetMat2(const std::string& name, const glm::mat2& mat) const;
    // ------------------------------------------------------------------------
    void SetMat3(const std::string& name, const glm::mat3& mat) const;
    // ------------------------------------------------------------------------
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
	// _loadShader() function should only be called after "m_shaderProgramID = glCreateProgram();"
	void _loadShader(const char* p_path, unsigned int p_shaderClass);
	void _checkCompileErrors(unsigned int p_shader, bool p_errorType);
};