#include <iostream>

#include <glad/glad.h>

#include "mesh.hxx"

using namespace cubes;

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
           const std::vector<Texture>& textures)
    : vertices_{vertices}, indices_{indices}, textures_{textures}
{
    initMesh();
}

void Mesh::draw(ShaderProgram& shaderProg)
{
    for (uint32_t i = 0; i < textures_.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        shaderProg.setInt(textures_[i].type() + std::to_string(i), i);
        glBindTexture(GL_TEXTURE_2D, textures_[i].id());
    }

    // std::cout << "rendering mesh " << VAO_ << std::endl;

    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);          // reset binding
    glActiveTexture(GL_TEXTURE0);  // reset to default
}

void Mesh::initMesh()
{
    // Set up a VAO
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    // Set up VBO and specify buffer
    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

    // Set up EBO and specify indices
    glGenBuffers(1, &EBO_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices_.size(), indices_.data(), GL_STATIC_DRAW);

    // Set up the vertex attribute pointers
    // - position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    // - tex attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoords));

    // Reset vertex array binding for the next meshes in the model(s)
    glBindVertexArray(0);
}
