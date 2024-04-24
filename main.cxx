#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <memory>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.hxx"
#include "environment.hxx"
#include "scenegraph/mesh.hxx"
#include "scenegraph/model.hxx"
#include "scenegraph/node.hxx"
#include "shaderprog.hxx"
#include "texture.hxx"

#define WIN_WIDTH 1025
#define WIN_HEIGHT 820

int main(int argc, char** argv)
{
    cubes::Camera cameraObj;
    cubes::Environment env{cameraObj};

    bool ret = true;
    ret = env.init(WIN_WIDTH, WIN_HEIGHT, std::string{"cubes"});
    if (!ret) {
        std::cerr << "Failed to initialize environment\n";
        return -1;
    }
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, env.winWidth(), env.winHeight());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    // glDepthFunc(GL_ALWAYS);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    cubes::ShaderProgram shaderProg{"../shaders/shader_vert.glsl", "../shaders/shader_frag.glsl"};

    std::vector<cubes::Vertex> vertices{{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},   {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
                                        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}}, {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},

                                        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},    {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
                                        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},  {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},

                                        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},   {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
                                        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},   {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},

                                        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},  {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
                                        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},  {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},

                                        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},  {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
                                        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},  {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},

                                        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},    {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}},
                                        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}},  {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}}};
    std::vector<uint32_t> indices{0,  1,  3,  1,  2,  3,  4,  5,  7,  5,  6,  7,  8,  9,  11, 9,  10, 11,
                                  12, 13, 15, 13, 14, 15, 16, 17, 19, 17, 18, 19, 20, 21, 23, 21, 22, 23};

    std::vector<cubes::Texture> textures{{std::string{"../resources/container2.png"}, std::string{"texture"}}};

    cubes::Mesh mesh{vertices, indices, textures};
    auto model = std::make_shared<cubes::Model>(std::vector<cubes::Mesh>({mesh}));

    const uint32_t numCubes{10};
    glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, -0.5f),   glm::vec3(2.0f, 5.0f, -15.0f),
                                 glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                 glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                 glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                 glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    cubes::Node rootNode;

    for (uint32_t i = 0; i < numCubes; ++i) {
        auto node = std::make_unique<cubes::Node>(model);
        node->transform().setPos(cubePositions[i]);
        rootNode.addChild(node);
    }

    using Clock = std::chrono::high_resolution_clock;

    static uint32_t framesCnt = 0;
    static auto start = Clock::now();

    // Render loop
    while (!env.shouldClose()) {
        env.processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        float timeVal = env.timestamp();

        shaderProg.useProg();

        glm::mat4 view = cameraObj.getViewMatrix();
        glm::mat4 proj =
            glm::perspective(glm::radians(cameraObj.zoom_), float(env.winWidth()) / env.winHeight(), 0.1f, 100.0f);

        shaderProg.setFloatMatrix4("view", glm::value_ptr(view));
        shaderProg.setFloatMatrix4("proj", glm::value_ptr(proj));

        uint32_t i = 0;
        rootNode.traverseChildren([&i, timeVal](auto&& child) {
            float angle = 20.0f * (i + 1) * (timeVal / 4.0f);
            child->transform().setRot(glm::vec3(angle, angle, angle));
            ++i;
        });
        rootNode.draw(shaderProg);

        env.endFrame();

        ++framesCnt;
        auto end = Clock::now();
        const std::chrono::duration<double, std::milli> fp_ms = end - start;
        const auto duration = fp_ms.count();
        if (duration >= 10 * 1000) {  // 10 seconds interval
            std::cout << "FPS: " << (framesCnt * 1000.0) / duration << std::endl;

            framesCnt = 0;
            start = Clock::now();
        }
    }

    env.close();

    return 0;
}
