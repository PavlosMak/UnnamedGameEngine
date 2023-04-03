#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "../components/RobotArmComponents.h"
#include "Scene.h"
#include "../materials/MaterialManager.h"
#include "Entity.h"
#include "Components.h"
#include "../components/AnimComponents.h"

Scene::Scene(entt::registry &registry) : m_registry(registry), m_shaderManager() {}

SceneStats Scene::getSceneStats() {
    return m_stats;
}

Entity Scene::getEntityByTag(std::basic_string<char> tag) {
    return Entity{m_tagToEntity[tag], m_registry};
}

void Scene::updateStatistics() {
    auto group = m_registry.group<MeshRendererComponent, TransformComponent>();
    for (auto entity: group) {
        MeshRendererComponent &meshRenderer = group.get<MeshRendererComponent>(entity);
        m_stats.triangleCount += meshRenderer.mesh.getTriangleCount();
    }
}

void Scene::setup(Camera &camera) {
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/solid_color_frag.glsl", SHADER_TYPE::SOLID_COLOR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/shader_frag.glsl", SHADER_TYPE::NORMAL_AS_COLOR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/phong_frag.glsl", SHADER_TYPE::PHONG);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/pbr_frag.glsl", SHADER_TYPE::PBR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/pbr_textured_frag.glsl", SHADER_TYPE::TEXTURED_PBR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/xtoon_frag.glsl", SHADER_TYPE::TOON);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/pbr_oscilating_textured_frag.glsl",
                               SHADER_TYPE::OSCILLATING_PBR);

    MaterialManager *materialManager = MaterialManager::getInstance();

    Material *groundColor = materialManager->createPBRMaterial(m_shaderManager.getShader(SHADER_TYPE::PBR),
                                                               glm::vec4(0.8, 0.8, 0.8, 1.0), 1.0, 0.0, 0.2);
    Material *blue = materialManager->createPBRMaterial(m_shaderManager.getShader(SHADER_TYPE::PBR),
                                                        glm::vec4(0.0, 0.0, 0.8, 1), 1.0, 0.0, 0.2);

    Material *texturedMaterial = materialManager->createTexturedPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::TEXTURED_PBR),
            "resources/dragon-scales/normal.png",
            "resources/dragon-scales/roughness.png",
            "resources/dragon-scales/metallic.png",
            "resources/dragon-scales/albedo.png",
            "resources/dragon-scales/ao.png",
            "resources/dragon-scales/height.png");

    Material *oscillating = materialManager->createTexturedOscillatingPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::OSCILLATING_PBR),
            "resources/dragon-scales/normal.png",
            "resources/dragon-scales/roughness.png",
            "resources/dragon-scales/metallic.png",
            "resources/dragon-scales/albedo.png",
            "resources/dragon-scales/ao.png",
            "resources/dragon-scales/height.png",
            "resources/rustedIron/normal.png",
            "resources/rustedIron/roughness.png",
            "resources/rustedIron/metallic.png",
            "resources/rustedIron/albedo.png",
            "resources/ones_texture.png",
            "resources/zero_texture.png"
    );


    auto light = Light(glm::vec3(10.0f));

    //Define some lights
    Entity light1 = this->createEntity("Light1");
    light1.addComponent<TransformComponent>(glm::vec3(2.115, 2.500, -0.787), glm::vec3(274.00, 0, 0), glm::vec3(1));
    light1.addComponent<LightComponent>(light);

    Entity mushu = this->createEntity("Mushu");
    mushu.addComponent<MeshRendererComponent>("resources/dragon.obj");
    mushu.addComponent<TransformComponent>(glm::vec3(2.103, -0.030, -0.350), glm::vec3(0, -20, 0), glm::vec3(1));
    mushu.addComponent<MaterialComponent>(oscillating);

    Entity ground = this->createEntity("Ground");
    ground.addComponent<MeshRendererComponent>("resources/cube.obj");
    ground.addComponent<TransformComponent>(glm::vec3(1.573, -1.270, 0), glm::vec3(0), glm::vec3(5.900, 0.995, 4.840));
    ground.addComponent<MaterialComponent>(groundColor);

    Entity cameraEntity = this->createEntity("Camera");
    cameraEntity.addComponent<TransformComponent>(glm::vec3(0, 0, -1), glm::vec3(0, 240, 0), glm::vec3(1));
    cameraEntity.addComponent<CameraComponent>(&camera);
    cameraEntity.addComponent<WasdComponent>(0.02f);

    updateStatistics();
}

void Scene::update(const long long &timeStep) {}

Entity Scene::createEntity(const std::string &name) {
    auto handle = m_registry.create();
    Entity entity = {handle, m_registry};
    entity.addComponent<TagComponent>(name);
    m_tagToEntity.emplace(name, handle);
    return entity;
}

Entity Scene::loadRobotArm(Transform baseTransform, Material *mat) {

    // Robot arm
    Entity base = this->createEntity("Base");
    base.addComponent<TransformComponent>(baseTransform.pos, baseTransform.rotation, baseTransform.scale);
    base.addComponent<MeshRendererComponent>("resources/robotarm/Base.obj");
    base.addComponent<MaterialComponent>(mat);

//    auto bezierAnim = Anim(true, 4);
//    auto curve = CubicBezier(
//            glm::vec3(0, 0, 0),
//            glm::vec3(1, 1, 0),
//            glm::vec3(3, 0, 0),
//            glm::vec3(2, 1, 0)
//    );
//
//    base.addComponent<BezierAnimation>(bezierAnim, curve);
//    base.addComponent<RotationAnimation>(bezierAnim, [](float t) {
//        return glm::vec3(500*t*t, 500*t*t, 500*t*t);
//    });

    Entity rBase = this->createEntity("rBase");
    rBase.addComponent<TransformComponent>(glm::vec3(0, 0.89, 0), glm::vec3(0), glm::vec3(1),
                                           &base.getComponent<TransformComponent>().transform);
    rBase.addComponent<MeshRendererComponent>("resources/robotarm/RotatingBase.obj");
    rBase.addComponent<MaterialComponent>(mat);

    auto baseAnim = Anim(true, 3);
    auto backForthY = [](Transform trans, float t) {
        auto newTransform(trans);

        if (t > 0.5) {
            newTransform.rotation = glm::vec3(0, 360 * t, 0);
        } else {
            newTransform.rotation = glm::vec3(0, -360 * t, 0);
        }

        return newTransform;
    };

    auto backForthX = [](Transform trans, float t) {
        auto newTransform(trans);

        if (t > 0.5) {
            newTransform.rotation = glm::vec3(360 * t, 0, 0);
        } else {
            newTransform.rotation = glm::vec3(-360 * t, 0, 0);
        }

        return newTransform;
    };

    Entity bicep = this->createEntity("Bicep");
    bicep.addComponent<TransformComponent>(glm::vec3(0, 0.9951, 0), glm::vec3(0), glm::vec3(1),
                                           &rBase.getComponent<TransformComponent>().transform);
    bicep.addComponent<MeshRendererComponent>("resources/robotarm/Bicep.obj");
    bicep.addComponent<MaterialComponent>(mat);
    rBase.addComponent<TransformAnimation>(baseAnim, backForthY);

    Entity forearm = this->createEntity("Forearm");
    forearm.addComponent<TransformComponent>(glm::vec3(0, 2.8, 0), glm::vec3(0), glm::vec3(1),
                                             &bicep.getComponent<TransformComponent>().transform);
    forearm.addComponent<MeshRendererComponent>("resources/robotarm/Forearm.obj");
    forearm.addComponent<MaterialComponent>(mat);
    forearm.addComponent<TransformAnimation>(baseAnim, backForthX);

    Entity wrist = this->createEntity("Wrist");
    wrist.addComponent<TransformComponent>(glm::vec3(0, 3, 0), glm::vec3(0), glm::vec3(1),
                                           &forearm.getComponent<TransformComponent>().transform);
    wrist.addComponent<MeshRendererComponent>("resources/robotarm/Wrist.obj");
    wrist.addComponent<MaterialComponent>(mat);
    wrist.addComponent<TransformAnimation>(baseAnim, backForthY);

    Entity hand = this->createEntity("Hand");
    hand.addComponent<TransformComponent>(glm::vec3(0, 1.2, 0), glm::vec3(0), glm::vec3(1),
                                          &wrist.getComponent<TransformComponent>().transform);
    hand.addComponent<MeshRendererComponent>("resources/robotarm/Hand.obj");
    hand.addComponent<MaterialComponent>(mat);
//    hand.addComponent<RotationAnimation>(baseAnim, backForthY);

    Entity c1 = this->createEntity("Claw1");
    c1.addComponent<TransformComponent>(glm::vec3(0, 0.8, 0.3), glm::vec3(0), glm::vec3(1),
                                        &hand.getComponent<TransformComponent>().transform);
    c1.addComponent<MeshRendererComponent>("resources/robotarm/Claw1.obj");
    c1.addComponent<MaterialComponent>(mat);

    Entity c2 = this->createEntity("Claw2");
    c2.addComponent<TransformComponent>(glm::vec3(0, 0.8, -0.3), glm::vec3(0), glm::vec3(1),
                                        &hand.getComponent<TransformComponent>().transform);
    c2.addComponent<MeshRendererComponent>("resources/robotarm/Claw2.obj");
    c2.addComponent<MaterialComponent>(mat);

    return base;
}

Entity Scene::loadPedestal(Transform baseTransform, Material *pedestalMat, Material *meshMat, std::string mesh) {

    Entity pedestal = this->createEntity("Pedestal");
    pedestal.addComponent<TransformComponent>(baseTransform.pos, baseTransform.rotation, baseTransform.scale);
    pedestal.addComponent<MeshRendererComponent>("resources/Pedestal.obj");
    pedestal.addComponent<MaterialComponent>(pedestalMat);

    Entity suzanne = this->createEntity("PedestalMesh");
    suzanne.addComponent<TransformComponent>(glm::vec3(0, 0.5, 0), glm::vec3(0), glm::vec3(1),
                                             &pedestal.getComponent<TransformComponent>().transform);
    suzanne.addComponent<MeshRendererComponent>(mesh);
    suzanne.addComponent<MaterialComponent>(meshMat);

    return suzanne;
}