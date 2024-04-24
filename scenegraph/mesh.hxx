#ifndef SCENEGRAPH_MESH_HXX__
#define SCENEGRAPH_MESH_HXX__

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "shaderprog.hxx"
#include "texture.hxx"

namespace cubes {

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoords;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
         const std::vector<Texture>& textures);

    void draw(ShaderProgram& shaderProg);

private:
    void initMesh();

    std::vector<Vertex> vertices_;
    std::vector<uint32_t> indices_;
    std::vector<Texture> textures_;
    uint32_t VAO_, VBO_, EBO_;
};

}  // namespace cubes

#endif  // #ifndef SCENEGRAPH_MESH_HXX__
