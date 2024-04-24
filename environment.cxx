#include <iostream>
#include <fstream>

#include <glad/glad.h>

#include "environment.hxx"

using namespace cubes;

Environment* Environment::envInstance_{nullptr};

Environment::Environment(Camera& cam) : cam_{cam}
{
    if (envInstance_)  // Allow only one instance of environment
        exit(-1);

    envInstance_ = this;
}

bool Environment::init(uint32_t width, uint32_t height, const std::string& winName)
{
    winWidth_ = width;
    winHeight_ = height;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(winWidth_, winHeight_, winName.c_str(), nullptr, nullptr);

    if (window_ == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window_);

    glfwSetFramebufferSizeCallback(window_, fbSizeCB);
    glfwSetKeyCallback(window_, keyCB);
    glfwSetMouseButtonCallback(window_, mouseButtonCB);
    glfwSetCursorPosCallback(window_, mousePosCB);
    glfwSetScrollCallback(window_, scrollCB);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    return true;
}

void Environment::processInput()
{
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
        cam_.processKeyboard(Camera::FORWARD_MOVE, deltaTime_);
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
        cam_.processKeyboard(Camera::BACKWARD_MOVE, deltaTime_);
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
        cam_.processKeyboard(Camera::LEFT_MOVE, deltaTime_);
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
        cam_.processKeyboard(Camera::RIGHT_MOVE, deltaTime_);
}

bool Environment::shouldClose()
{
    return glfwWindowShouldClose(window_);
}

float Environment::timestamp()
{
    float time_val = glfwGetTime();

    deltaTime_ = time_val - lastFrameTime_;
    lastFrameTime_ = time_val;

    return time_val;
}

void Environment::endFrame()
{
    glfwPollEvents();
    glfwSwapBuffers(window_);
}

void Environment::close()
{
    glfwTerminate();
}

void Environment::fbSizeCB(GLFWwindow* window, int width, int height)
{
    envInstance_->winWidth_ = width;
    envInstance_->winHeight_ = height;
    glViewport(0, 0, width, height);
}

void Environment::scrollCB(GLFWwindow* window, double xoffset, double yoffset)
{
    envInstance_->cam_.processMouseScroll((float) yoffset);
}

void Environment::mouseButtonCB(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            envInstance_->mouseButtonPressed_ = true;
            envInstance_->firstMousePress_ = true;
        }
        if (action == GLFW_RELEASE) {
            envInstance_->mouseButtonPressed_ = false;
        }
    }
}

void Environment::mousePosCB(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = 400, lastY = 300;
    if (!envInstance_->mouseButtonPressed_)
        return;

    if (envInstance_->firstMousePress_) {
        lastX = xpos;
        lastY = ypos;
        envInstance_->firstMousePress_ = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    // true in 3rd argument - constrain pitch / yaw
    envInstance_->cam_.processMouseMovement(xoffset, yoffset, true);
}

void Environment::keyCB(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        envInstance_->cam_.processKeyboard(Camera::FORWARD_MOVE, envInstance_->deltaTime_);
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        envInstance_->cam_.processKeyboard(Camera::BACKWARD_MOVE, envInstance_->deltaTime_);
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        envInstance_->cam_.processKeyboard(Camera::LEFT_MOVE, envInstance_->deltaTime_);
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        envInstance_->cam_.processKeyboard(Camera::RIGHT_MOVE, envInstance_->deltaTime_);
}
