
#include <iostream>
#include <fstream>

#include <glad/glad.h>

#include "shaderprog.hxx"

using namespace cubes;

ShaderProgram::ShaderProgram(const char* vertFilename, const char* fragFilename)
{
    auto vertexShader = ShaderProgram::compileShader(GL_VERTEX_SHADER, vertFilename);
    auto fragmentShader = ShaderProgram::compileShader(GL_FRAGMENT_SHADER, fragFilename);

    progID_ = glCreateProgram();
    glAttachShader(progID_, vertexShader);
    glAttachShader(progID_, fragmentShader);
    glLinkProgram(progID_);

    int success;
    glGetProgramiv(progID_, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(progID_, 512, nullptr, infoLog);
        std::cout << "Shader prog link failed\n" << infoLog << std::endl;

        exit(-2);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderProgram::useProg() const
{
    glUseProgram(progID_);
}

void ShaderProgram::setFloat(const std::string& name, const float val) const
{
    glUniform1f(glGetUniformLocation(progID_, name.c_str()), val);
}

void ShaderProgram::setInt(const std::string& name, const int val) const
{
    glUniform1i(glGetUniformLocation(progID_, name.c_str()), val);
}

void ShaderProgram::setFloatVec4(const std::string& name, const std::array<float, 4>& vals) const
{
    glUniform4f(glGetUniformLocation(progID_, name.c_str()), vals[0], vals[1], vals[2], vals[3]);
}

// void ShaderProgram::setFloatVec3(const std::string& name, const std::array<float, 3>& vals) const
void ShaderProgram::setFloatVec3(const std::string& name, const glm::vec3& v) const
{
    // glUniform3f(uniform_loc, vals[0], vals[1], vals[2]);
    glUniform3f(glGetUniformLocation(progID_, name.c_str()), v[0], v[1], v[2]);
}

void ShaderProgram::setFloatMatrix4(const std::string& name, void* mat) const
{
    auto uniform_loc = glGetUniformLocation(progID_, name.c_str());
    glUniformMatrix4fv(glGetUniformLocation(progID_, name.c_str()), 1, GL_FALSE, (const GLfloat*) mat);
}

unsigned int ShaderProgram::compileShader(int shaderType, const char* shaderFilename)
{
    // Read and compile shader
    std::ifstream input{shaderFilename};
    if (input.fail()) {
        std::cerr << "Failed to read shader: " << shaderFilename << std::endl;

        exit(-2);
    }

    const std::string shaderStr((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    auto shaderStrPtr = shaderStr.c_str();

    unsigned int shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &shaderStrPtr, nullptr);
    glCompileShader(shaderHandle);

    int success;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderHandle, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed\n" << infoLog << std::endl;

        exit(-2);
    }

    return shaderHandle;
}
