#ifndef CAMERA_HXX__
#define CAMERA_HXX__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace cubes {

class Camera
{
public:
    enum CameraMovement
    {
        FORWARD_MOVE,
        BACKWARD_MOVE,
        LEFT_MOVE,
        RIGHT_MOVE
    };

    glm::vec3 position_{0.0f, 0.0f, 0.5f};
    glm::vec3 eye_{0.0f, 0.0f, 0.0f};
    glm::vec3 front_{0.0f, 0.0f, -1.0f};
    glm::vec3 up_{0.0f, 1.0f, 0.0f};
    glm::vec3 right_;
    glm::vec3 worldUp_{0.0f, 1.0f, 0.0f};

    // float yaw_{-90.0f};
    // float pitch_{0.0f};

    float yaw_{0.0f};
    float pitch_{0.0f};

    float moveSpeed_{2.5f};
    float mouseSensitivity_{0.1f};
    float zoom_{45.0f};

    Camera() { updateCameraVectors(); }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : position_(posX, posY, posZ), worldUp_{upX, upY, upZ}, yaw_{yaw}, pitch_{pitch}
    {
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix()
    {
        auto view_mat = glm::lookAt(position_, position_ + front_, up_);
        view_mat = glm::rotate(view_mat, glm::radians(pitch_), glm::vec3(-1.0f, 0.0f, 0.0f));
        view_mat = glm::rotate(view_mat, glm::radians(yaw_), glm::vec3(0.0f, 1.0f, 0.0f));
        return view_mat;
    }

    void processKeyboard(CameraMovement dir, float delta_time)
    {
        float velocity = moveSpeed_ * delta_time;
        switch (dir) {
            case FORWARD_MOVE: position_ += front_ * velocity; break;
            case BACKWARD_MOVE: position_ -= front_ * velocity; break;
            case LEFT_MOVE: position_ -= right_ * velocity; break;
            case RIGHT_MOVE: position_ += right_ * velocity; break;
        }
    }

    void processMouseMovement(float xoffset, float yoffset, bool constrain_pitch = true)
    {
        xoffset *= mouseSensitivity_;
        yoffset *= mouseSensitivity_;

        yaw_ += xoffset;
        pitch_ += yoffset;

        if (constrain_pitch) {
            pitch_ = std::min(pitch_, 89.0f);
            pitch_ = std::max(pitch_, -89.0f);
        }

        updateCameraVectors();
    }

    void processMouseScroll(float yoffset)
    {
        zoom_ -= yoffset;
        zoom_ = std::min(zoom_, 60.0f);
        zoom_ = std::max(zoom_, 1.0f);
    }

private:
    void updateCameraVectors()
    {
        // glm::vec3 front{
        //     cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)),
        //     sin(glm::radians(pitch_)),
        //     sin(glm::radians(yaw_)) * cos(glm::radians(pitch_))
        // };
        // front_ = glm::normalize(front);
        right_ = glm::normalize(glm::cross(front_, worldUp_));
        up_ = glm::normalize(glm::cross(right_, front_));
    }
};

}  // end namespace cubes

#endif  // #ifndef CAMERA_HXX__
