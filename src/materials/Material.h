#pragma once

#include "framework/shader.h"
#include "ShaderManager.h"
#include "../scene/Entity.h"
#include "../lights/Light.h"
#include "../texture.h"

class Material {
public:

    Material(int id, const Shader &shader, glm::vec4 color, SHADER_TYPE shaderType) :
            ID(id), m_shader(shader), m_color(color), TYPE(shaderType) {};

    Material(int id, const Shader &shader, SHADER_TYPE shaderType) : ID(id), m_shader(shader),
                                                                     TYPE(shaderType) {};

    Material(int id, const Shader &shader, glm::vec4 color, float shininess, SHADER_TYPE shaderType) : ID(id),
                                                                                                       m_shader(shader),
                                                                                                       m_color(color),
                                                                                                       m_shininess(
                                                                                                               shininess),
                                                                                                       TYPE(
                                                                                                               shaderType) {};

    Material(int id, const Shader &shader, int sdfId, bool isSDF) : ID(id), m_shader(shader), m_sdfTextureId(sdfId), TYPE(SDF) {};

    Material(int id, const Shader &shader, SHADER_TYPE shaderType, int normalMapId, int roughMapId, int metalMapId,
             int albedoMapId,
             int ambientOcclusionMapId, int heightMapId) : ID(id), m_shader(shader),
                                                           TYPE(shaderType),
                                                           m_normalMapId(normalMapId),
                                                           m_roughnessMapId(roughMapId), m_metallicMapId(metalMapId),
                                                           m_albedoMapId(albedoMapId),
                                                           m_heightMapId(heightMapId),
                                                           m_ambientOcclusionMapId(ambientOcclusionMapId) {
    };


    Material(int id, const Shader &shader, int normalMapId, int roughMapId, int metalMapId, int albedoMapId,
             int ambientOcclusionMapId, int heightMapId,
             int normalMapId2, int roughMapId2, int metalMapId2, int albedoMapId2,
             int ambientOcclusionMapId2, int heightMapId2) : ID(id), m_shader(shader),
                                                             TYPE(SHADER_TYPE::OSCILLATING_PBR),
                                                             m_normalMapId(normalMapId),
                                                             m_roughnessMapId(roughMapId), m_metallicMapId(metalMapId),
                                                             m_albedoMapId(albedoMapId),
                                                             m_heightMapId(heightMapId),
                                                             m_ambientOcclusionMapId(ambientOcclusionMapId),
                                                             m_normalMapId2(normalMapId2),
                                                             m_roughnessMapId2(roughMapId2),
                                                             m_metallicMapId2(metalMapId2),
                                                             m_albedoMapId2(albedoMapId2),
                                                             m_heightMapId2(heightMapId2),
                                                             m_ambientOcclusionMapId2(ambientOcclusionMapId2) {};

    Material(int id, const Shader &shader, glm::vec4 albedo, float roughness, float metallic, float ambient) :
            ID(id), m_shader(shader), m_color(albedo), m_roughness(roughness), m_metallic(metallic),
            m_ambient(ambient), TYPE(SHADER_TYPE::PBR) {}

    Material(int id, const Shader &shader, int toonTextureId) : ID(id), m_shader(shader),
                                                                m_toonTextureId(toonTextureId),
                                                                TYPE(SHADER_TYPE::TOON) {}

    [[nodiscard]] const Shader &
    getShader() const;

    [[nodiscard]] glm::vec4 getColor() const;

    void setColor(glm::vec4 &color);

    [[nodiscard]] float getShininess() const;

    void setShininess(float shininess);

    [[nodiscard]] float getRoughness() const;

    void setRoughness(float roughness);

    [[nodiscard]] float getMetallic() const;

    void setMetallic(float metallic);

    [[nodiscard]] float getAmbient() const;

    void setAmbient(float ambient);

    /**
     * Binds the material
     * @param cameraPosition
     * @param lights
     * @param lightPositions
     * @return the number of texture slots occupied
     */
    void bindMaterial(glm::vec3 &cameraPosition, std::vector<Light> &lights, std::vector<glm::vec3> &lightPositions);

    const int ID;


    //TODO: properly encapsulate these
    int textureSlotOccupied = 0;
    int lightOffset = 0;

    const SHADER_TYPE TYPE;
private:
    const Shader &m_shader;
    glm::vec4 m_color{1.0f}; //m_color acts as the diffuse for phong and albedo for pbr
    float m_shininess{0.0f};

    //PBR
    float m_roughness{1.0f};
    float m_metallic{1.0f};
    float m_ambient{0.0f};

    //Textures (not the nicest way to do it :/)
    int m_normalMapId;
    int m_roughnessMapId;
    int m_metallicMapId;
    int m_albedoMapId;
    int m_ambientOcclusionMapId;
    int m_heightMapId;

    //The second pair is used in case of oscillating material
    //honestly we should just wrap those in a PBR texture class
    int m_normalMapId2;
    int m_roughnessMapId2;
    int m_metallicMapId2;
    int m_albedoMapId2;
    int m_ambientOcclusionMapId2;
    int m_heightMapId2;

    int m_toonTextureId;

    int m_sdfTextureId;
};