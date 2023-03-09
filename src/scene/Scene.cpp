#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

Scene::Scene() : m_shaderManager() {};

SceneStats Scene::getSceneStats() {
    return m_stats;
}

Entity Scene::getEntityByTag(std::basic_string<char> tag) {
    return Entity{m_tagToEntity[tag], this};
}

void Scene::setup(Camera &camera) {
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/solid_color_frag.glsl", SHADER_TYPE::SOLID_COLOR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/shader_frag.glsl", SHADER_TYPE::NORMAL_AS_COLOR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/phong_frag.glsl", SHADER_TYPE::PHONG);
    //TODO: We probably want a material pool to access them by name
    Material material = Material(m_shaderManager.getShader(SHADER_TYPE::SOLID_COLOR), glm::vec3(1.0, 0.0, 0.0),
                                 SHADER_TYPE::SOLID_COLOR);
    Material materialBlue = Material(m_shaderManager.getShader(SHADER_TYPE::SOLID_COLOR), glm::vec3(0.0, 1.0, 0.0),
                                     SHADER_TYPE::SOLID_COLOR);
    Material normalMaterial = Material(m_shaderManager.getShader(SHADER_TYPE::NORMAL_AS_COLOR),
                                       SHADER_TYPE::NORMAL_AS_COLOR);
    Material phongMaterial = Material(m_shaderManager.getShader(SHADER_TYPE::PHONG), glm::vec3(1.0, 0.0, 0.0),
                                      SHADER_TYPE::PHONG);


    Entity cameraEntity = this->createEntity("Camera");
    cameraEntity.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(-1, 3, 0)));
    cameraEntity.addComponent<CameraComponent>(camera, glm::vec3(5, 3, -1));

//    Entity secondDragon = this->createEntity("Syrax");
//    secondDragon.addComponent<MeshRendererComponent>("resources/dragon.obj");
//    secondDragon.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(0.0, 0.0, 1.0)));
//    secondDragon.addComponent<MaterialComponent>(normalMaterial);
//
//    Entity thirdDragon = this->createEntity("Mushu");
//    thirdDragon.addComponent<MeshRendererComponent>("resources/dragon.obj");
//    thirdDragon.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(0.0, 0.0, -1.0)));
//    thirdDragon.addComponent<MaterialComponent>(normalMaterial);

//    Entity ape = this->createEntity("Ape");
//    ape.addComponent<MeshRendererComponent>("resources/ape.obj");
//    ape.addComponent<TransformComponent>(glm::scale(glm::rotate(glm::mat4{1.0f},
//                                                                glm::radians(-45.f),
//                                                                glm::vec3(0.0, 1.0, 0.0)),
//                                                    glm::vec3(0.1, 0.1, 0.1))
//    );
//    ape.addComponent<MaterialComponent>(phongMaterial);

    Entity defaultCube = this->createEntity("TodoDelete");
    defaultCube.addComponent<MeshRendererComponent>("resources/defaultCube.obj");
    defaultCube.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(5, 3, -1)));
    defaultCube.addComponent<MaterialComponent>(normalMaterial);

    Entity sponza = this->createEntity("Sponza");
    //TODO: The app crashes if we use nested path for the resource
    sponza.addComponent<MeshRendererComponent>("resources/sponza.obj");
    sponza.addComponent<TransformComponent>(glm::scale(glm::mat4{1.0f}, glm::vec3(0.02, 0.02, 0.02)));
    sponza.addComponent<MaterialComponent>(normalMaterial);

    m_stats.triangleCount += defaultCube.getComponent<MeshRendererComponent>().mesh.getTriangleCount();
    m_stats.triangleCount += sponza.getComponent<MeshRendererComponent>().mesh.getTriangleCount();
}

void Scene::update(const long long &timeStep) {
    glm::mat4 vpMatrix;

    Entity camera = this->getEntityByTag("Camera");
    auto &mainCameraComponent = camera.getComponent<CameraComponent>();
    auto &mainCameraTransform = camera.getComponent<TransformComponent>();
    mainCameraComponent.getViewProjectionMatrix(vpMatrix, mainCameraTransform.transform);

    auto group = m_Registy.group<MeshRendererComponent, TransformComponent>();

    glEnable(GL_DEPTH_TEST);
    for (auto entity: group) {
        TransformComponent &transform = group.get<TransformComponent>(entity);
        MeshRendererComponent &meshRenderer = group.get<MeshRendererComponent>(entity);
        MaterialComponent &materialComponent = m_Registy.get<MaterialComponent>(entity);

        const glm::mat4 mvpMatrix = vpMatrix * transform.transform;
        // Normals should be transformed differently than positions (ignoring translations + dealing with scaling):
        // https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
        const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(transform.transform));

        materialComponent.material.bindMaterial();

        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(transform.transform));
        glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));

        //TODO: THE FOLLOWING SHOULD BE MOVED SOMEWHERE ELSE
//        glUniform3fv(3, 1, glm::value_ptr(glm::vec3(0, 3, 2))); //3 -> light pos
//        glUniform3fv(4, 1, glm::value_ptr(glm::vec3(0.4)));//4 -> light color

        meshRenderer.mesh.draw();
        //        glUniform1i(4, GL_FALSE); This was used for the texture coordinates
    }
    mainCameraTransform.transform = glm::translate(mainCameraTransform.transform, glm::vec3(0.003));
}


Entity Scene::createEntity(const std::string &name) {
    auto handle = m_Registy.create();
    Entity entity = {handle, this};
    entity.addComponent<TagComponent>(name);
    m_tagToEntity.emplace(name, handle);
    return entity;
}