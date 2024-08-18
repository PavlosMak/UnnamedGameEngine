#pragma once

#include "framework/shader.h"
#include "../managers/ShaderManager.h"
#include "../scene/Entity.h"
#include "../lights/Light.h"
#include "Texture.h"


struct PBRTexture {
    int m_normalMapId;
    int m_roughnessMapId;
    int m_metallicMapId;
    int m_albedoMapId;
    int m_ambientOcclusionMapId;
    int m_heightMapId;
};

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

    Material(int id, const Shader &shader, int sdfId, bool isSDF) : ID(id), m_shader(shader), m_sdfTextureId(sdfId),
                                                                    TYPE(SDF) {};

    Material(int id, const Shader &shader, SHADER_TYPE shaderType, PBRTexture texture) : ID(id), m_shader(shader),
                                                           TYPE(shaderType), m_pbrTexture1(texture) {
    };


    Material(int id, const Shader &shader, PBRTexture startingTexture, PBRTexture endTexture) : ID(id),
                                                                                                m_shader(shader),
                                                                                                TYPE(SHADER_TYPE::OSCILLATING_PBR),
                                                                                                m_pbrTexture1(startingTexture),
                                                                                                m_pbrTexture2(endTexture) {};

    Material(int id, const Shader &shader, glm::vec4 albedo, float roughness, float metallic, float ambient) :
            ID(id), m_shader(shader), m_color(albedo), m_roughness(roughness), m_metallic(metallic),
            m_ambient(ambient), TYPE(SHADER_TYPE::PBR) {}

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

    //Textures - The second one is used in case of oscillating material
    PBRTexture m_pbrTexture1;
    PBRTexture m_pbrTexture2;

    int m_sdfTextureId;
};