#ifndef SCENEGRAPH_NODE_HXX__
#define SCENEGRAPH_NODE_HXX__

#include <memory>
#include <string>
#include <list>

#include <glm/glm.hpp>

#include "model.hxx"
#include "shaderprog.hxx"

namespace cubes {

class Transform
{
public:
    Transform() = default;

    void setPos(const glm::vec3& pos);
    void setRot(const glm::vec3& rot);
    void setScale(const glm::vec3& scale);

    const glm::vec3& pos() const { return pos_; }
    const glm::vec3& rot() const { return rot_; }
    const glm::vec3& scale() const { return scale_; }
    const glm::mat4& modelMat() const { return modelMat_; }
    const bool dirty() const { return dirty_; }

    void computeModelMat();
    void computeModelMat(const glm::mat4& parentModelMat);

protected:
    glm::vec3 pos_{0.0f, 0.0f, 0.0f};
    glm::vec3 rot_{0.0f, 0.0f, 0.0f};
    glm::vec3 scale_{1.0f, 1.0f, 1.0f};

    glm::mat4 modelMat_ = glm::mat4(1.0f);

    bool dirty_{true};
};

class Node
{
public:
    Node() = default;
    Node(std::shared_ptr<Model>& model);

    void addChild(std::unique_ptr<Node>& node);
    template <typename T>
    void traverseChildren(T func)
    {
        for (auto&& child : children_) {
            func(child);
        }
    }

    Transform& transform() { return transform_; }

    void draw(ShaderProgram& shaderProg);
    void draw(ShaderProgram& shaderProg, const glm::mat4& parentModelMat);
    // void update(const glm::mat4& parentModelMat, bool force = false);
    // void update(bool force = false);

protected:
    void drawImpl(ShaderProgram& shaderProg);
    void drawChildren(ShaderProgram& shaderProg);

    // The model instance can be null, it means this node is a grouping node
    std::shared_ptr<Model> model_;
    Transform transform_;

    std::list<std::unique_ptr<Node>> children_;
};

}  // namespace cubes

#endif  // #ifndef SCENEGRAPH_NODE_HXX__
