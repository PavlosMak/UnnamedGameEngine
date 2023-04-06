#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "../components/RobotArmComponents.h"
#include "Scene.h"
#include "../materials/MaterialManager.h"
#include "Entity.h"
#include "Components.h"
#include "../components/AnimComponents.h"

#include "iostream"

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

    int centralRoomId = 0;
    int animationRoomId = 1;
    int spotLightRoomId = 2;

    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/solid_color_frag.glsl", SHADER_TYPE::SOLID_COLOR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/shader_frag.glsl", SHADER_TYPE::NORMAL_AS_COLOR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/phong_frag.glsl", SHADER_TYPE::PHONG);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/pbr_frag.glsl", SHADER_TYPE::PBR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/pbr_textured_frag.glsl", SHADER_TYPE::TEXTURED_PBR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/xtoon_frag.glsl", SHADER_TYPE::TOON);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/pbr_oscilating_textured_frag.glsl",
                               SHADER_TYPE::OSCILLATING_PBR);
    m_shaderManager.loadShader("shaders/shader_height_vert.glsl", "shaders/pbr_heightmapped_frag.glsl",
                               SHADER_TYPE::HEIGHT_MAPPED);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/sdf_frag.glsl", SHADER_TYPE::SDF);

    MaterialManager *materialManager = MaterialManager::getInstance();

    Material *armadilloMat = materialManager->createPBRMaterial(m_shaderManager.getShader(SHADER_TYPE::PBR),
                                                                glm::vec4(204.f / 255.f, 84.f / 255.f, 0, 1), 1.0, 0.0,
                                                                0.2);


    Material *gold = materialManager->createPBRMaterial(m_shaderManager.getShader(SHADER_TYPE::PBR),
                                                        glm::vec4(0.9, 0.82, 0.03, 1), 0.620, 0.793, 0.2);
    Material *blueMesh = materialManager->createPBRMaterial(m_shaderManager.getShader(SHADER_TYPE::PBR),
                                                            glm::vec4(20.f / 255.f, 149.f / 255.f, 133.f / 255.f, 1),
                                                            0.717, 0.1, 0.2);


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

    Material *blue = materialManager->createPBRMaterial(m_shaderManager.getShader(SHADER_TYPE::PBR),
                                                        glm::vec4(0.0, 0.0, 0.8, 0.2), 1.0, 0.0, 0.2);

    Material *green = materialManager->createPBRMaterial(m_shaderManager.getShader(SHADER_TYPE::PBR),
                                                         glm::vec4(0.0, 0.8, 0.0, 0.2), 1.0, 0.0, 0.2);

    Material *red = materialManager->createPBRMaterial(m_shaderManager.getShader(SHADER_TYPE::PBR),
                                                       glm::vec4(0.8, 0.0, 0.0, 0.2), 1.0, 0.0, 0.2);

    Material *toon = materialManager->createXToonMaterial(m_shaderManager.getShader(SHADER_TYPE::TOON),
                                                          "resources/toon_map.png");
    Material *sdf = materialManager->createSDFMaterial(m_shaderManager.getShader(SHADER_TYPE::SDF),
                                                       "resources/sdf.png");

    Material *johnMaterial = materialManager->createTexturedPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::TEXTURED_PBR),
            "resources/ones_texture.png",
            "resources/ones_texture.png",
            "resources/zero_texture.png",
            "resources/lordAndSaviour/img.png",
            "resources/zero_texture.png",
            "resources/zero_texture.png"
    );


    Material *matRobot = materialManager->createPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::PBR), glm::vec4(1, 0, 0, 1), 0.4, 0.4, 0.01);

    Material *matGround = materialManager->createPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::PBR), glm::vec4(0.92, 0.807, 0.46, 1), 0.4, 0.0, 0.01);

    Material *matWalls = materialManager->createPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::PBR), glm::vec4(0.16, 0.16, 0.16, 1), 0.3, 0.0, 0.01);

    Material *matArches = materialManager->createPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::PBR), glm::vec4(1, 1, 1, 1), 0.4, 0.01, 0.01);

    Material *matPedestal = materialManager->createPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::PBR), glm::vec4(1, 1, 1, 1), 0.4, 0.01, 0.01);

    // load museum
    auto mainHall = loadScene(matGround, matWalls, matArches);

    // robotarm
    loadRobotArm(mainHall, Transform(glm::vec3(7.370, 0.160, -1.430), glm::vec3(0, 0, 0), glm::vec3(0.2, 0.2, 0.2)),
                 matRobot);


    // load the pedestals
    auto pedestalsController = createEntityParented("PedestalsController", mainHall,
                                                    Transform(glm::vec3(0, -1.7, 1.7), glm::vec3(0), glm::vec3(1)));
    pedestalsController.addComponent<MeshRendererComponent>("resources/cube.obj");
    pedestalsController.addComponent<MaterialComponent>(matPedestal);

    loadPedestal(pedestalsController, Transform(glm::vec3(0, 2, 0)), matPedestal, blueMesh, matArches, gold, 2,
                 "resources/TeapotSmall.obj");
    loadPedestal(pedestalsController, Transform(glm::vec3(0.3, 2, 0)), matPedestal, matArches, blueMesh, oscillating,
                 2, "resources/DragonSmall.obj");
    loadPedestal(pedestalsController, Transform(glm::vec3(0.6, 2, 0)), matPedestal, matArches, gold, red, 2,
                 "resources/ApeSmall.obj");
    loadPedestal(pedestalsController, Transform(glm::vec3(0.9, 2, 0)), matPedestal, gold, matArches, blueMesh, 2,
                 "resources/House.obj");
    loadPedestal(pedestalsController, Transform(glm::vec3(1.2, 2, 0)), matPedestal, gold, red, armadilloMat, 2,
                 "resources/SuzanneSmall.obj");

    // Create transparent planes
    Entity quadR = this->createEntityParented("QuadR", mainHall,
                                              Transform(glm::vec3(-3.360, 0.660, 0.5), glm::vec3(0, -171, 87),
                                                        glm::vec3(0.3)));
    quadR.addComponent<MeshRendererComponent>("resources/quad.obj");
    quadR.addComponent<MaterialComponent>(red);
    quadR.addComponent<FindMe>();
    Entity quadG = this->createEntityParented("QuadG", quadR, Transform(glm::vec3(0, 1, 1)));
    quadG.addComponent<MeshRendererComponent>("resources/quad.obj");
    quadG.addComponent<MaterialComponent>(green);


    Material *matAdam = materialManager->createPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::PBR), glm::vec4(1, 1, 1, 1), 0.3, 1.0, 0.01);

    Material *matLens = materialManager->createPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::PBR), glm::vec4(0.1, 0.1, 0.1, 1), 0.05, 0.0, 0.01);

    Material *matEyes = materialManager->createPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::PBR), glm::vec4(0.01, 0.01, 0.01, 1), 1, 0, 0.01);

    Material *matRed = materialManager->createPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::PBR), glm::vec4(1, 0, 0, 1), 0.0, 0.0, 0.01);

    auto adam = createEntity("Player");
    adam.addComponent<TransformComponent>(glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0), glm::vec3(0.7));
    adam.addComponent<MeshRendererComponent>("resources/adam/Body.obj");
    adam.addComponent<MaterialComponent>(matAdam);
    adam.addComponent<WasdComponent>(0.02f);
    adam.addComponent<PlayerComponent>(matAdam, toon);

    auto head = createEntityParented("AdamHead", adam, Transform(glm::vec3(0, 0.5, 0)));
    head.addComponent<MeshRendererComponent>("resources/adam/Head.obj");
    head.addComponent<MaterialComponent>(matAdam);

    auto eye = createEntityParented("AdamEye", head, Transform());
    eye.addComponent<MeshRendererComponent>("resources/adam/Eye.obj");
    eye.addComponent<MaterialComponent>(matEyes);

    auto lens = createEntityParented("AdamLens", head, Transform());
    lens.addComponent<MeshRendererComponent>("resources/adam/Lens.obj");
    lens.addComponent<MaterialComponent>(matLens);
    lens.addComponent<FindMe>();

    auto antena = createEntityParented("AdamAntena", head, Transform());
    antena.addComponent<MeshRendererComponent>("resources/adam/Antena.obj");
    antena.addComponent<MaterialComponent>(matEyes);

    auto antenaCap = createEntityParented("AdamAntenaCap", head, Transform());
    antenaCap.addComponent<MeshRendererComponent>("resources/adam/antenaCap.obj");
    antenaCap.addComponent<MaterialComponent>(matRed);

    auto smallEye = createEntityParented("AdamSmallEye", head, Transform());
    smallEye.addComponent<MeshRendererComponent>("resources/adam/smallEye.obj");
    smallEye.addComponent<MaterialComponent>(matEyes);

    auto smallRed = createEntityParented("AdamSmallRed", head, Transform());
    smallRed.addComponent<MeshRendererComponent>("resources/adam/smallRed.obj");
    smallRed.addComponent<MaterialComponent>(matRed);

    auto armL = createEntityParented("AdamArmL", adam, Transform(glm::vec3(0.4, 0.25, 0)));
    armL.addComponent<MeshRendererComponent>("resources/adam/Arms.obj");
    armL.addComponent<MaterialComponent>(matAdam);

    auto armR = createEntityParented("AdamArmR", adam, Transform(glm::vec3(-0.4, 0.25, 0)));
    armR.addComponent<MeshRendererComponent>("resources/adam/Arms.obj");
    armR.addComponent<MaterialComponent>(matAdam);

    auto bunnyController = createEntityParented("BunnyCtrl", mainHall,
                                                Transform(glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(1)));
    bunnyController.addComponent<MeshRendererComponent>("resources/Controller.obj");
    bunnyController.addComponent<MaterialComponent>(blue);
    bunnyController.addComponent<FindMe>();

    auto bunny = createEntityParented("Bunny", bunnyController,
                                      Transform(glm::vec3(1), glm::vec3(0, 90, 0), glm::vec3(1)));
    bunny.addComponent<MeshRendererComponent>("resources/Bunny.obj");
    bunny.addComponent<MaterialComponent>(matPedestal);

    auto bezierAnim = Anim(true, 4);

    std::vector points = {
            glm::vec3(0, 0, 0),
            glm::vec3(0.1, 1, 0),
            glm::vec3(0.9, 1, 0),

            glm::vec3(1, 0, 0),

            glm::vec3(1.1, 1, 0),
            glm::vec3(1.9, 1, 0),
            glm::vec3(2, 0, 0),
    };

    auto curve = CompositeBezier(points);

    bunny.addComponent<BezierAnimation>(bezierAnim, curve);


//    Material *mountainMaterial = materialManager->createHeightMappedTexturedPBRMaterial(
//            m_shaderManager.getShader(SHADER_TYPE::HEIGHT_MAPPED),
//            "resources/ones_texture.png",
//            "resources/ones_texture.png",
//            "resources/zero_texture.png",
//            "resources/ones_texture.png",
//            "resources/ones_texture.png",
//            "resources/dragon-scales/height.png"
//    );

    //Load the armadillo meshes
    std::vector<std::filesystem::path> paths;
    for (int i = 1; i <= 20; i++) {
        paths.emplace_back("resources/armadillo/armadillo" + std::to_string(i) + ".obj");
    }

    Entity armadillo = this->createEntity("Armadillo");
    armadillo.addComponent<MeshRendererComponent>("resources/armadillo/armadillo1.obj");
    armadillo.addComponent<TransformComponent>(glm::vec3(7.240, 1.550, 2.820), glm::vec3(-30, 180, 0),
                                               glm::vec3(0.2, 0.2, 0.2));
    armadillo.addComponent<MaterialComponent>(armadilloMat);
    armadillo.addComponent<SkinnedMeshAnimationComponent>(paths);

    animationRoomEntities.push_back(this->m_tagToEntity["Armadillo"]);

//    Entity puzzle = this->createEntity("Puzzle");
//    puzzle.addComponent<MeshRendererComponent>("resources/cube.obj");
//    puzzle.addComponent<TransformComponent>(glm::vec3(-0.380, 0.8, -0.5), glm::vec3(0), glm::vec3(0.05));
//    puzzle.addComponent<MaterialComponent>(red);
//    puzzle.addComponent<PuzzleObjectComponent>(red, green, blue, 2);

    Entity johny = this->createEntity("LordAndSavior");
    johny.addComponent<MeshRendererComponent>("resources/cube.obj");
    johny.addComponent<TransformComponent>(glm::vec3(2.520, 0.920, -4.060), glm::vec3(0, 180., -90),
                                           glm::vec3(0.700, 0.320, 0.190));
    johny.addComponent<MaterialComponent>(johnMaterial);
    johny.addComponent<FindMe>();


    Entity secretRoom = this->createEntity("SecretRoom");
    secretRoom.addComponent<MeshRendererComponent>("resources/cube.obj");
    secretRoom.addComponent<TransformComponent>(glm::vec3(2.490, 0.710, -2.910), glm::vec3(0), glm::vec3(1));
    secretRoom.addComponent<MaterialComponent>(matArches);

    Entity sdfQuad = this->createEntity("SDF");
    sdfQuad.addComponent<MeshRendererComponent>("resources/quad.obj");
    sdfQuad.addComponent<TransformComponent>(glm::vec3(2.970, 1.190, -1.900), glm::vec3(90, 0, 0),
                                             glm::vec3(1));
    sdfQuad.addComponent<MaterialComponent>(sdf);

    //Define some light
    Entity light1 = this->createEntity("MainLight");
    light1.addComponent<TransformComponent>(glm::vec3(0.360, 2.270, 1.593), glm::vec3(293.00, 5, 0), glm::vec3(1));
    light1.addComponent<LightComponent>(glm::vec3(10.0f));
    light1.addComponent<RoomComponent>(-1);

    Entity animationRoomLight = this->createEntity("Spotlight");
    animationRoomLight.addComponent<TransformComponent>(glm::vec3(-7.485, 0.684, 0.018), glm::vec3(264, 180, 0),
                                                        glm::vec3(1));
    animationRoomLight.addComponent<LightComponent>(glm::vec3(1.0f));
    animationRoomLight.addComponent<RoomComponent>(animationRoomId);

    Entity cameraEntity = this->createEntity("Camera");
    cameraEntity.addComponent<TransformComponent>(glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0), glm::vec3(1));
    cameraEntity.addComponent<CameraComponent>(&camera);
    cameraEntity.addComponent<CamControllerComp>(&adam.getComponent<TransformComponent>().transform);

    updateStatistics();
}

void Scene::update(const long long &timeStep) {

    //Get the player
    entt::entity player = m_tagToEntity["Player"];
    PlayerComponent &playerComponent = m_registry.get<PlayerComponent>(player);
//    Transform &playerTransform = m_registry.get<TransformComponent>(player).transform;
    glm::vec3 playerPos = glm::vec3(m_registry.get<TransformComponent>(player).transform.worldTransform()[3]);

    //Determine if the player is under the spotlight by doing
    //the shadowmapping calculation
    bool underSpotlight = false;
    entt::entity spotlight = m_tagToEntity["Spotlight"];
    Transform &spotlightTrans = m_registry.get<TransformComponent>(spotlight).transform;

    Camera spotCamera = Camera();
    glm::mat4 spotVP;
    spotCamera.getViewProjectionMatrix(spotVP, spotlightTrans);
    glm::vec4 playerLightPosHom = spotVP * glm::vec4(playerPos, 1);
    glm::vec3 playerLightPos = glm::vec3(playerLightPosHom) / playerLightPosHom.w;
    auto shadowMapPos = glm::vec2(playerLightPos);

    auto dist = glm::distance(shadowMapPos, glm::vec2(0.5, 0.5));
    underSpotlight = dist < 3.5;

    std::cout << dist << " " << underSpotlight << std::endl;

    if (underSpotlight) {
        playerComponent.isToon = true;
        m_registry.get<MaterialComponent>(m_tagToEntity["AdamArmL"]).material = playerComponent.toonMaterial;
        m_registry.get<TransformComponent>(m_tagToEntity["AdamArmL"]).transform.rotation.x = 90;
    }

    //Determine distance to Carmack and update in the player
    entt::entity carmack = m_tagToEntity["LordAndSavior"];
    glm::vec3 carmackPos = m_registry.get<TransformComponent>(carmack).transform.pos;
    float distanceToCarmack = glm::distance(carmackPos, playerPos);
    playerComponent.distanceToCarmack = distanceToCarmack;
    if (distanceToCarmack < 1) {
        playerComponent.isToon = false;
        m_registry.get<MaterialComponent>(m_tagToEntity["AdamArmL"]).material = playerComponent.basicMaterial;
        m_registry.get<TransformComponent>(m_tagToEntity["AdamArmL"]).transform.rotation.x = 00;
    }
}

Entity Scene::createEntity(const std::string &name) {
    auto handle = m_registry.create();
    Entity entity = {handle, m_registry};
    entity.addComponent<TagComponent>(name);
    m_tagToEntity.emplace(name, handle);
    return entity;
}

Entity Scene::createEntityParented(const std::string &name, Entity &parent, Transform t) {
    auto handle = m_registry.create();
    Entity entity = {handle, m_registry};
    entity.addComponent<TagComponent>(name);
    m_tagToEntity.emplace(name, handle);
    auto parentTransform = &parent.getComponent<TransformComponent>().transform;
    entity.addComponent<TransformComponent>(t.pos, t.rotation, t.scale, parentTransform);
    return entity;
}

Entity Scene::loadRobotArm(Entity &parent, Transform t, Material *mat) {

    // Robot arm
    Entity base = this->createEntityParented("Base", parent, t);
    base.addComponent<MeshRendererComponent>("resources/robotarm/Base.obj");
    base.addComponent<MaterialComponent>(mat);

    animationRoomEntities.push_back(this->m_tagToEntity["Base"]);


    Entity rBase = this->createEntity("rBase");
    rBase.addComponent<TransformComponent>(glm::vec3(0, 0.89, 0), glm::vec3(0), glm::vec3(1),
                                           &base.getComponent<TransformComponent>().transform);
    rBase.addComponent<MeshRendererComponent>("resources/robotarm/RotatingBase.obj");
    rBase.addComponent<MaterialComponent>(mat);

    animationRoomEntities.push_back(this->m_tagToEntity["rBase"]);


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

    animationRoomEntities.push_back(this->m_tagToEntity["Bicep"]);


    Entity forearm = this->createEntity("Forearm");
    forearm.addComponent<TransformComponent>(glm::vec3(0, 2.8, 0), glm::vec3(0), glm::vec3(1),
                                             &bicep.getComponent<TransformComponent>().transform);
    forearm.addComponent<MeshRendererComponent>("resources/robotarm/Forearm.obj");
    forearm.addComponent<MaterialComponent>(mat);
    forearm.addComponent<TransformAnimation>(baseAnim, backForthX);

    animationRoomEntities.push_back(this->m_tagToEntity["Forearm"]);


    Entity wrist = this->createEntity("Wrist");
    wrist.addComponent<TransformComponent>(glm::vec3(0, 3, 0), glm::vec3(0), glm::vec3(1),
                                           &forearm.getComponent<TransformComponent>().transform);
    wrist.addComponent<MeshRendererComponent>("resources/robotarm/Wrist.obj");
    wrist.addComponent<MaterialComponent>(mat);
    wrist.addComponent<TransformAnimation>(baseAnim, backForthY);

    animationRoomEntities.push_back(this->m_tagToEntity["Wrist"]);


    Entity hand = this->createEntity("Hand");
    hand.addComponent<TransformComponent>(glm::vec3(0, 1.2, 0), glm::vec3(0), glm::vec3(1),
                                          &wrist.getComponent<TransformComponent>().transform);
    hand.addComponent<MeshRendererComponent>("resources/robotarm/Hand.obj");
    hand.addComponent<MaterialComponent>(mat);

    animationRoomEntities.push_back(this->m_tagToEntity["Hand"]);


    Entity c1 = this->createEntity("Claw1");
    c1.addComponent<TransformComponent>(glm::vec3(0, 0.8, 0.3), glm::vec3(0), glm::vec3(1),
                                        &hand.getComponent<TransformComponent>().transform);
    c1.addComponent<MeshRendererComponent>("resources/robotarm/Claw1.obj");
    c1.addComponent<MaterialComponent>(mat);

    animationRoomEntities.push_back(this->m_tagToEntity["Claw1"]);

    Entity c2 = this->createEntity("Claw2");
    c2.addComponent<TransformComponent>(glm::vec3(0, 0.8, -0.3), glm::vec3(0), glm::vec3(1),
                                        &hand.getComponent<TransformComponent>().transform);
    c2.addComponent<MeshRendererComponent>("resources/robotarm/Claw2.obj");
    c2.addComponent<MaterialComponent>(mat);
    animationRoomEntities.push_back(this->m_tagToEntity["Claw2"]);


    return base;
}

Entity Scene::loadPedestal(Entity &parent, Transform transform, Material *pedestalMat,
                           Material *meshMat,
                           Material *meshMat2,
                           Material *meshMat3,
                           int key,
                           std::string mesh) {

    Entity pedestal = this->createEntityParented("Pedestal" + mesh, parent, transform);
    pedestal.addComponent<MeshRendererComponent>("resources/Pedestal.obj");
    pedestal.addComponent<MaterialComponent>(pedestalMat);

    Entity suzanne = this->createEntityParented("PedestalMesh", pedestal, Transform(glm::vec3(0, 0.45, 0)));
    suzanne.addComponent<MeshRendererComponent>(mesh);
    suzanne.addComponent<MaterialComponent>(meshMat);
    suzanne.addComponent<PuzzleObjectComponent>(meshMat, meshMat2, meshMat3, key);
    suzanne.addComponent<FindMe>();

    return suzanne;
}

Entity Scene::loadScene(Material *matGround, Material *matWalls, Material *matArches) {

    Entity mainHall = this->createEntity("MainHall");
    mainHall.addComponent<MeshRendererComponent>("resources/env/MainHall.obj");
    mainHall.addComponent<TransformComponent>(glm::vec3(0), glm::vec3(0), glm::vec3(1));
    mainHall.addComponent<MaterialComponent>(matWalls);

    Entity arch = this->createEntityParented("Arch", mainHall, Transform());
    arch.addComponent<MeshRendererComponent>("resources/env/Arch.obj");
    arch.addComponent<MaterialComponent>(matArches);

    Entity hallway = this->createEntityParented("Hallway", mainHall, Transform());
    hallway.addComponent<MeshRendererComponent>("resources/env/Hallway.obj");
    hallway.addComponent<MaterialComponent>(matArches);

    Entity room = this->createEntityParented("Room", mainHall, Transform());
    room.addComponent<MeshRendererComponent>("resources/env/Room.obj");
    room.addComponent<MaterialComponent>(matWalls);

    Entity ground = this->createEntityParented("Ground", mainHall, Transform());
    ground.addComponent<MeshRendererComponent>("resources/env/Ground.obj");
    ground.addComponent<MaterialComponent>(matGround);

    return mainHall;
}