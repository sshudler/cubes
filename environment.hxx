#ifndef ENVIRONMENT_HXX__
#define ENVIRONMENT_HXX__

#include <string>

#include <GLFW/glfw3.h>

#include "camera.hxx"

namespace cubes {

class Environment
{
public:
    Environment(Camera& cam);

    bool init(uint32_t width, uint32_t height, const std::string& winName);
    void processInput();
    bool shouldClose();
    void endFrame();
    void close();

    float timestamp();
    uint32_t winWidth() const { return winWidth_; }
    uint32_t winHeight() const { return winHeight_; }

private:
    static Environment* envInstance_;
    static void fbSizeCB(GLFWwindow* window, int width, int height);
    static void scrollCB(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseButtonCB(GLFWwindow* window, int button, int action, int mods);
    static void mousePosCB(GLFWwindow* window, double xpos, double ypos);
    static void keyCB(GLFWwindow* window, int key, int scancode, int action, int mods);

    Camera& cam_;
    uint32_t winWidth_{1025};
    uint32_t winHeight_{820};
    GLFWwindow* window_{nullptr};
    float deltaTime_{0.0f};
    float lastFrameTime_{0.0f};
    bool firstMousePress_{true};
    bool mouseButtonPressed_{false};
};

}  // namespace cubes

#endif  // #ifndef ENVIRONMENT_HXX__
