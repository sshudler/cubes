#include "model.hxx"

using namespace cubes;

Model::Model(const std::vector<Mesh>& meshes) : meshes_{meshes} {}

void Model::draw(ShaderProgram& shaderProg)
{
    for (auto& mesh : meshes_) {
        mesh.draw(shaderProg);
    }
}
