#ifndef SCENEGRAPH_MODEL_HXX__
#define SCENEGRAPH_MODEL_HXX__

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "mesh.hxx"
#include "shaderprog.hxx"

namespace cubes {

class Model
{
public:
    Model(const std::vector<Mesh>& meshes);

    void draw(ShaderProgram& shaderProg);

private:
    std::vector<Mesh> meshes_;
};

}  // namespace cubes

#endif  // #ifndef SCENEGRAPH_MODEL_HXX__
