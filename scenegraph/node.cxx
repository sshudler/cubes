#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "node.hxx"

using namespace cubes;

void Transform::setPos(const glm::vec3& pos)
{
    pos_ = pos;
    dirty_ = true;
}

void Transform::setRot(const glm::vec3& rot)
{
    rot_ = rot;
    dirty_ = true;
}

void Transform::setScale(const glm::vec3& scale)
{
    scale_ = scale;
    dirty_ = true;
}

void Transform::computeModelMat()
{
    if (!dirty_)
        return;

    const glm::mat4 tformX = glm::rotate(glm::mat4(1.0f), glm::radians(rot_.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 tformY = glm::rotate(glm::mat4(1.0f), glm::radians(rot_.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 tformZ = glm::rotate(glm::mat4(1.0f), glm::radians(rot_.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotMat = tformY * tformX * tformZ;

    // translation * rotation * scale (also know as TRS matrix)
    modelMat_ = glm::translate(glm::mat4(1.0f), pos_) * rotMat * glm::scale(glm::mat4(1.0f), scale_);

    dirty_ = false;
}

void Transform::computeModelMat(const glm::mat4& parentModelMat)
{
    computeModelMat();
    modelMat_ = parentModelMat * modelMat_;
    dirty_ = false;
}

Node::Node(std::shared_ptr<Model>& model) : model_{model} {}

void Node::addChild(std::unique_ptr<Node>& node)
{
    children_.push_back(std::move(node));
}

void Node::draw(ShaderProgram& shaderProg)
{
    transform_.computeModelMat();
    drawImpl(shaderProg);
}

void Node::draw(ShaderProgram& shaderProg, const glm::mat4& parentModelMat)
{
    transform_.computeModelMat(parentModelMat);
    drawImpl(shaderProg);
}

void Node::drawImpl(ShaderProgram& shaderProg)
{
    if (model_) {
        // std::cout << "drawing model, this " << (uint64_t) this << std::endl;
        shaderProg.setFloatMatrix4(std::string{"model"}, (void*) glm::value_ptr(transform_.modelMat()));
        model_->draw(shaderProg);
    }

    drawChildren(shaderProg);
}

void Node::drawChildren(ShaderProgram& shaderProg)
{
    // std::cout << "render cycle, this " << (uint64_t) this << " num children " << children_.size() << std::endl;

    for (auto&& child : children_) {
        child->draw(shaderProg, transform_.modelMat());
    }
}
