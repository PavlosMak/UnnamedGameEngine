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

#include <framework/shader.h>
#include <framework/window.h>
#include <functional>
#include <iostream>
#include "camera.h"
#include "scene/SceneManager.h"
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/Components.h"
#include "systems/WasdControllerSystem.h"
#include "systems/AnimationSystem.h"
#include "systems/RobotArmSystem.h"
#include "systems/RenderSystem.h"
#include "systems/DebugSystem.h"

//TODO: Move to command line argument
#define PATH_TO_GAME_FILES "/home/pavlos/Desktop/stuff/GameEngine/example_games/banner"

class Application {
public:
    Application()
            : m_window("Game", glm::ivec2(1024, 1024), OpenGLVersion::GL45), m_sceneManager(PATH_TO_GAME_FILES) {

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

            ShaderBuilder debugBuilder;
            debugBuilder.addStage(GL_VERTEX_SHADER, "shaders/debug_vert.glsl")
                    .addStage(GL_FRAGMENT_SHADER, "shaders/shadow_debug_frag.glsl");
            m_debugShader = debugBuilder.build();

        } catch (ShaderLoadingException e) {
            std::cerr << e.what() << std::endl;
        }

        // register inspector components
        m_debugSystem.register_component<WasdComponent>("WASD");
        m_debugSystem.register_component<TransformComponent>("Transform");
        m_debugSystem.register_component<TagComponent>("Tag");
        m_debugSystem.register_component<CameraComponent>("Camera");
        m_debugSystem.register_component<LightComponent>("Light");
        m_debugSystem.register_component<MaterialComponent>("Material");
        m_debugSystem.register_component<SetRotation>("SetRotation");
        m_debugSystem.register_component<BezierAnimation>("BezierAnim");
        m_debugSystem.register_component<FindMe>("Find Me!");
        m_debugSystem.register_component<PlayerComponent>("Player");
        m_debugSystem.register_component<PuzzleObjectComponent>("Puzzle");
        m_debugSystem.register_component<SkinnedMeshAnimationComponent>("Skinned Animation");
        m_debugSystem.register_component<CamControllerComp>("CamController");

    }

    void update() {
        Scene& currentScene = m_sceneManager.getCurrentScene();
        currentScene.setup();
        long long timeStep = 0l;

        m_renderSystem.init(m_shadowShader, currentScene.m_registry);

        auto lastTick = glfwGetTime();

        while (!m_window.shouldClose()) {

            // update the window state
            m_window.updateInput();

            m_debugSystem.run(currentScene.m_registry);

            auto curTick = glfwGetTime();
            auto dt = curTick - lastTick;
            m_animSystem.stepAnimations(currentScene.m_registry, dt);
            lastTick = curTick;

            // handle input
            m_wasdSystem.update(currentScene.m_registry);

            m_roboArmSystem.setRotations(currentScene.m_registry);

            // update currentScene
            currentScene.update(timeStep);

            m_renderSystem.renderMeshes(m_shadowShader, currentScene.m_registry, currentScene.getEntityByTag("Camera"),
                                        m_window.getWindowSize(),
                                        m_window.getAspectRatio());

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
        m_debugSystem.onKeyRelease(key);
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
    Shader m_debugShader;


    DebugSystem m_debugSystem;

    SceneManager m_sceneManager;

    // systems
    WasdControllerSystem m_wasdSystem;
    RobotArmSystem m_roboArmSystem;
    RenderSystem m_renderSystem;
    AnimationSystem m_animSystem;
};

int main() {
    Application app;

    app.update();

    return 0;
}
