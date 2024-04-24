#ifndef SHADER_PROG_HXX__
#define SHADER_PROG_HXX__

#include <array>
#include <string>
#include <glm/glm.hpp>

namespace cubes {

class ShaderProgram
{
public:
    ShaderProgram(const char* vertFilename, const char* fragFilename);

    void useProg() const;
    unsigned int getProgID() const { return progID_; }

    // Set uniforms
    void setFloat(const std::string& name, const float val) const;
    void setInt(const std::string& name, const int val) const;
    void setFloatVec4(const std::string& name, const std::array<float, 4>& vals) const;
    // void setFloatVec3(const std::string& name, const std::array<float, 3>& vals) const;
    void setFloatVec3(const std::string& name, const glm::vec3& v) const;
    void setFloatMatrix4(const std::string& name, void* mat) const;

private:
    unsigned int compileShader(int shaderType, const char* shaderFilename);

    unsigned int progID_;
};

}  // namespace cubes

#endif  // #ifndef SHADER_PROG_HXX__
