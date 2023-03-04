//#include "Image.h"
#include "mesh.h"
#include "texture.h"
// Always include window first (because it includes glfw, which includes GL which needs to be included AFTER glew).
// Can't wait for modules to fix this stuff...
#include <framework/disable_all_warnings.h>

DISABLE_WARNINGS_PUSH()

#include <glad/glad.h>
// Include glad before glfw3
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <imgui/imgui.h>

DISABLE_WARNINGS_POP()

#include <framework/shader.h>
#include <framework/window.h>
#include <functional>
#include <iostream>
#include <vector>

#include "camera.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/Components.h"

class Application {
public:
    Application()
            : m_window("Final Project", glm::ivec2(1024, 1024), OpenGLVersion::GL45) {
        m_window.registerKeyCallback([this](int key, int scancode, int action, int mods) {
            if (action == GLFW_PRESS)
                onKeyPressed(key, mods);
            else if (action == GLFW_RELEASE)
                onKeyReleased(key, mods);
        });
        m_window.registerMouseMoveCallback(std::bind(&Application::onMouseMove, this, std::placeholders::_1));
        m_window.registerMouseButtonCallback([this](int button, int action, int mods) {
            if (action == GLFW_PRESS)
                onMouseClicked(button, mods);
            else if (action == GLFW_RELEASE)
                onMouseReleased(button, mods);
        });

        try {
            ShaderBuilder defaultBuilder;
            defaultBuilder.addStage(GL_VERTEX_SHADER, "shaders/shader_vert.glsl");
            defaultBuilder.addStage(GL_FRAGMENT_SHADER, "shaders/shader_frag.glsl");
            m_defaultShader = defaultBuilder.build();

            ShaderBuilder shadowBuilder;
            shadowBuilder.addStage(GL_VERTEX_SHADER, "shaders/shadow_vert.glsl");
            m_shadowShader = shadowBuilder.build();

            // Any new shaders can be added below in similar fashion.
            // ==> Don't forget to reconfigure CMake when you do!
            //     Visual Studio: PROJECT => Generate Cache for ComputerGraphics
            //     VS Code: ctrl + shift + p => CMake: Configure => enter
            // ....
        } catch (ShaderLoadingException e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void setup() {
        //TODO: The next lines are all a dirty hack to quickly demonstrate the ECS.

        Entity firstDragon = scene.createEntity("Smaug");
        firstDragon.addComponent<MeshComponent>("resources/dragon.obj");
        firstDragon.addComponent<TransformComponent>(glm::mat4{1.0f});

        Entity secondDragon = scene.createEntity("Syrax");
        secondDragon.addComponent<MeshComponent>("resources/dragon.obj");
        secondDragon.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(0.0, 0.0, 1.0)));

        Entity thirdDragon = scene.createEntity("Mushu");
        thirdDragon.addComponent<MeshComponent>("resources/dragon.obj");
        thirdDragon.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(0.0, 0.0, -1.0)));

        entities.push_back(firstDragon);
        entities.push_back(secondDragon);
        entities.push_back(thirdDragon);
    }

    void update() {
        //Save current time for profiling
        int dummyInteger = 0; // Initialized to 0

        //Create our camera
        Camera camera = Camera(glm::vec3(-1, 1, -1), glm::vec3(0), glm::vec3(0, 1, 0));

        //Init a viewProjectionMatrix that we are going to update
        glm::mat4 viewProjectionMatrix;


        while (!m_window.shouldClose()) {

            // This is your game loop
            // Put your real-time logic and rendering in here
            m_window.updateInput();
            // Use ImGui for easy input/output of ints, floats, strings, etc...
            ImGui::Begin("Debug Window");
            ImGui::InputInt("This is an integer input",
                            &dummyInteger); // Use ImGui::DragInt or ImGui::DragFloat for larger range of numbers.
            ImGui::Text("Value is: %i", dummyInteger); // Use C printf formatting rules (%i is a signed integer)
            ImGui::End();

            //Adjust size of window
            glViewport(0, 0, m_window.getWindowSize().x, m_window.getWindowSize().y);
            // Clear the screen
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // ...
            glEnable(GL_DEPTH_TEST);

            //Get new view projection matrix based on camera position
            camera.getViewProjectionMatrix(viewProjectionMatrix, m_window.getAspectRatio());

            m_defaultShader.bind();

            for (Entity entity: entities) {
                //Below needs
//                auto m_mesh =

                glm::mat4 &m_modelMatrix = entity.getComponent<TransformComponent>().transform;

                const glm::mat4 mvpMatrix = viewProjectionMatrix * m_modelMatrix;
                // Normals should be transformed differently than positions (ignoring translations + dealing with scaling):
                // https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
                const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix));

                glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
                glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
                glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
//                if (m_mesh.hasTextureCoords()) {
//                    m_texture.bind(GL_TEXTURE0);
//                    glUniform1i(3, 0);
//                    glUniform1i(4, GL_TRUE);
//                } else {
//                    glUniform1i(4, GL_FALSE);
//                }
                glUniform1i(4, GL_FALSE);
                entity.getComponent<MeshComponent>().mesh.draw();
//            m_mesh.draw();
            }
            // Processes input and swaps the window buffer
            m_window.swapBuffers();
        }
    }

    // In here you can handle key presses
    // key - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__keys.html
    // mods - Any modifier keys pressed, like shift or control
    void onKeyPressed(int key, int mods) {
        switch (key) {
            case GLFW_KEY_A:
                break;
            case GLFW_KEY_W:
                break;
            case GLFW_KEY_S:
                break;
            case GLFW_KEY_D:
                break;
        }
//        std::cout << "Key pressed: " << key << std::endl;
    }

    // In here you can handle key releases
    // key - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__keys.html
    // mods - Any modifier keys pressed, like shift or control
    void onKeyReleased(int key, int mods) {
        std::cout << "Key released: " << key << std::endl;
    }

    // If the mouse is moved this function will be called with the x, y screen-coordinates of the mouse
    void onMouseMove(const glm::dvec2 &cursorPos) {
        std::cout << "Mouse at position: " << cursorPos.x << " " << cursorPos.y << std::endl;
    }

    // If one of the mouse buttons is pressed this function will be called
    // button - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__buttons.html
    // mods - Any modifier buttons pressed
    void onMouseClicked(int button, int mods) {
        std::cout << "Pressed mouse button: " << button << std::endl;
    }

    // If one of the mouse buttons is released this function will be called
    // button - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__buttons.html
    // mods - Any modifier buttons pressed
    void onMouseReleased(int button, int mods) {
        std::cout << "Released mouse button: " << button << std::endl;
    }

private:
    Window m_window;

    // Shader for default rendering and for depth rendering
    Shader m_defaultShader;
    Shader m_shadowShader;

    //TODO: HACK FOR DEMO BE SURE TO REMOVE
    std::vector<Entity> entities;

    Scene scene;

//    GPUMesh m_mesh;
//    Texture m_texture;
};

int main() {
    Application app;

    app.setup();
    app.update();

    return 0;
}
