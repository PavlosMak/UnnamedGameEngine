#version 450

//Constants
const float PI = 3.14159;
const float epsilon = 0.000001; //small epsilon to avoid div by 0
const int NUM_OF_LIGHTS = 1;

//Inputs
layout(location = 3) uniform vec3 cameraPos;

layout(location = 4) uniform float ambient;
layout(location = 5) uniform sampler2D normalMap;
layout(location = 6) uniform sampler2D albedoMap;
layout(location = 7) uniform sampler2D roughnessMap;
layout(location = 8) uniform sampler2D metallicMap;
layout(location = 9) uniform sampler2D ambientMap;

layout(location = 10) uniform vec3 lightPos[NUM_OF_LIGHTS];
layout(location = 10 + NUM_OF_LIGHTS) uniform vec3 lightColor[NUM_OF_LIGHTS];

layout(location = 10 + NUM_OF_LIGHTS + 1) uniform sampler2D shadowMap;
layout(location = 10 + NUM_OF_LIGHTS + 2) uniform mat4 lightMVP;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

//Outputs
layout(location = 0) out vec4 fragColor;


//Normal space to tangeant space
//as seen in LearnOpenGL's normal mapping tutorial:
//https://learnopengl.com/Advanced-Lighting/Normal-Mapping
vec3 getNormalFromMap() {
    vec3 tangentNormal = texture(normalMap, fragTexCoord).xyz * 2.0 - 1.0;
    vec3 Q1  = dFdx(fragPosition);
    vec3 Q2  = dFdy(fragPosition);
    vec2 st1 = dFdx(fragTexCoord);
    vec2 st2 = dFdy(fragTexCoord);
    vec3 N = normalize(fragNormal);
    vec3 T = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    return normalize(TBN * tangentNormal);
}

//Performs local Tone Mapping with Reinhard's formula
vec3 reinhardToneMap(vec3 color) {
    return color / (color + vec3(1.0));
}

//Trowbridge-Reitz GGX:
//approximates alignment of microfacets
float distributionGGX(vec3 normal, vec3 h, float roughness) {
    float a = roughness*roughness;
    float a2 = a*a;
    float phi = max(dot(normal, h), 0.0);
    float phi2 = phi*phi;

    float denom = (phi2 * (a2 - 1.0) + 1.0);

    return a2 / (denom * denom * PI);
}

//Geometry functions: approximate how much microfacets occlude each other
float schlickGGX(float phi, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    return phi / (phi * (1.0 - k) + k);
}

float geometrySmith(vec3 normal, vec3 camVec, vec3 lightVec, float roughness) {
    float phi = max(dot(normal, camVec), 0.0);
    float theta = max(dot(normal, lightVec), 0.0);
    return schlickGGX(theta, roughness)*schlickGGX(phi, roughness);
}

//Calculates the ratio of surface reflection at different angles (Fresnel)
//using the Schlick approximation
vec3 fresnelSchlick(float cos, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cos, 0.0, 1.0), 5.0);
}

//Calculates how much the point is in shadow
float shadowContribution(vec3 fragPos, vec3 normal, sampler2D texShadow, vec3 lightPosition) {
    vec3 lightDir = normalize(lightPosition - fragPos);

    //Change the bias based on the slope of the surface
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.03);
    vec3 biasedFragPos = fragPos + lightDir*bias;

    vec4 fragLightCoord = lightMVP * vec4(biasedFragPos, 1.0);
    fragLightCoord.xyz /= fragLightCoord.w;
    fragLightCoord.xyz = fragLightCoord.xyz * 0.5 + 0.5;

    float fragLightDepth = fragLightCoord.z;
    vec2 shadowMapCoord = fragLightCoord.xy;

    float result = 0.0;

    vec2 texelSize = 1.0 / textureSize(texShadow, 0);
    for(int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec2 filteredCoord = shadowMapCoord + vec2(x, y)*texelSize;
            float shadowMapDepth = texture(texShadow, filteredCoord).x;
            if ((shadowMapDepth < fragLightDepth)) {
                result += 1.0f;
            }
        }
    }
    return result / 9.0f;
}

void main() {
    vec3 normal = getNormalFromMap();//normalize(vec3(texture(normalMap, fragTexCoord)));
    vec3 albedo = pow(texture(albedoMap, fragTexCoord).rgb, vec3(2.2));
    float roughness = texture(roughnessMap, fragTexCoord).r;
    float metallic = texture(metallicMap, fragTexCoord).r;
    float ambientOcclusion = texture(ambientMap, fragTexCoord).r;

    vec3 camVec = normalize(cameraPos - fragPosition);

    //Precalculate base reflectivity so we can use Schlick for
    //both dielectrics and conductors
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 result = vec3(0.0);
    float shadowScale = 0;
    for (int i = 0; i < NUM_OF_LIGHTS; i++) {
        vec3 lightVec = normalize(lightPos[i] - fragPosition);
        vec3 H = normalize(lightVec + camVec);

        float cosTheta = max(dot(lightVec, normal), 0.0);
        float cosPhi = max(dot(normal, camVec), 0.0);

        //Scale contribution of light based on distance
        float distanceToLight = length(lightPos[i] - fragPosition);
        float attenuation = 1.0 / (distanceToLight * distanceToLight);
        vec3 radiance = lightColor[i]*attenuation;

        //Cook-Torrance BRDF
        float normalDistribution = distributionGGX(normal, H, roughness);
        vec3 F = fresnelSchlick(clamp(dot(camVec, H), 0.0, 1.0), F0);
        float G = geometrySmith(normal, camVec, lightVec, roughness);

        vec3 numerator = normalDistribution*F*G;
        float denominator = (4.0 * cosPhi * cosTheta) + epsilon;

        //Specular part
        vec3 specular = numerator / denominator;
        vec3 ks = F;

        //Lambertian diffuse
        vec3 kd = vec3(1.0) - ks;
        kd = kd * (1.0 - metallic);
        vec3 diffuse = kd * (albedo / PI);

        result += (diffuse + specular) * radiance * cosTheta;
        shadowScale += shadowContribution(fragPosition, normal, shadowMap, lightPos[i]);
    }
    //Normalize the shadow scale based on number of lights
    shadowScale = shadowScale / NUM_OF_LIGHTS;

    //Add ambient
    result += albedo*ambient*ambientOcclusion;

    //Tone mapping and gamma correction
    result = pow(reinhardToneMap(result), vec3(1.1/2.2));

    fragColor = (1-shadowScale)*vec4(result, 1.0);
}
