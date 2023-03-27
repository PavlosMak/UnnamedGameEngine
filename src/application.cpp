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
#include <imgui/imgui.h>

DISABLE_WARNINGS_POP()

#include <framework/entt_imgui/imgui_entt_entity_editor.hpp>

#include <framework/shader.h>
#include <framework/window.h>
#include <functional>
#include <iostream>
#include "camera.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/Components.h"
#include "systems/WasdControllerSystem.h"
#include "systems/RenderSystem.h"
#include "systems/DebugSystem.h"


class Application {
public:
    Application()
            : m_window("Final Project", glm::ivec2(1024, 1024), OpenGLVersion::GL45), m_scene(m_registry) {

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

        // register inspector components
        m_debugSystem.register_component<WasdComponent>("WASD");
        m_debugSystem.register_component<TransformComponent>("Transform");
        m_debugSystem.register_component<SRTTransformComponent>("SRTTransform");
        m_debugSystem.register_component<TagComponent>("Tag");
        // m_debugSystem.register_component<CameraComponent>("Camera");
    }

    void update() {
        //Create our camera
        Camera camera = Camera();
        m_scene.setup(camera);
        long long timeStep = 0l;
        while (!m_window.shouldClose()) {

            // update the window state
            m_window.updateInput();


            m_debugSystem.run(m_registry);

            // handle input
            m_wasdSystem.update(m_registry);

            // update scene
            m_scene.update(timeStep);

            // m_renderSystem.renderMeshes(m_registry, m_scene.getEntityByTag("Camera"), m_window.getWindowSize(), m_window.getAspectRatio());
            m_renderSystem.renderMeshesSRT(m_registry, m_scene.getEntityByTag("Camera"), m_window.getWindowSize(), m_window.getAspectRatio());

            // Processes input and swaps the window buffer
            m_window.swapBuffers();
            timeStep += 1l;
        }
    }

    // Event based input handlers
    // callbacks called when key pressed

    // In here you can handle key presses
    // key - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__keys.html
    // mods - Any modifier keys pressed, like shift or control
    void onKeyPressed(int key, int mods) {
        m_wasdSystem.onKeyPressed(key);
    }

    // In here you can handle key releases
    // key - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__keys.html
    // mods - Any modifier keys pressed, like shift or control
    void onKeyReleased(int key, int mods) {
        m_wasdSystem.onKeyReleased(key);
    }

    // If the mouse is moved this function will be called with the x, y screen-coordinates of the mouse
    void onMouseMove(const glm::dvec2 &cursorPos) {
        // std::cout << "Mouse at position: " << cursorPos.x << " " << cursorPos.y << std::endl;
    }

    // If one of the mouse buttons is pressed this function will be called
    // button - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__buttons.html
    // mods - Any modifier buttons pressed
    void onMouseClicked(int button, int mods) {
        // std::cout << "Pressed mouse button: " << button << std::endl;
    }

    // If one of the mouse buttons is released this function will be called
    // button - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__buttons.html
    // mods - Any modifier buttons pressed
    void onMouseReleased(int button, int mods) {
        // std::cout << "Released mouse button: " << button << std::endl;
    }

private:
    Window m_window;

    // Shader for default rendering and for depth rendering
    Shader m_shadowShader;

    DebugSystem m_debugSystem;


    Scene m_scene;
    entt::registry m_registry;

    // systems
    WasdControllerSystem m_wasdSystem;
    RenderSystem m_renderSystem;
};

int main() {
    Application app;

    app.update();

    return 0;
}
