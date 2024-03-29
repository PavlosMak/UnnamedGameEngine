#version 450

//Constants
const float PI = 3.14159;
const float epsilon = 0.000001;//small epsilon to avoid div by 0
const int NUM_OF_LIGHTS = 1;

//Inputs
layout(location = 3) uniform vec3 cameraPos;

layout(location = 4) uniform vec4 alpha_albedo;
layout(location = 5) uniform float roughness;
layout(location = 6) uniform float metallic;
layout(location = 7) uniform float ambient;

layout(location = 8) uniform vec3 lightPos[NUM_OF_LIGHTS];
layout(location = 8 + NUM_OF_LIGHTS) uniform vec3 lightColor[NUM_OF_LIGHTS];

layout(location = 8 + 2*NUM_OF_LIGHTS) uniform sampler2D shadowMaps[NUM_OF_LIGHTS];
layout(location = 8 + 3*NUM_OF_LIGHTS) uniform mat4 lightMVPs[NUM_OF_LIGHTS];
//layout(location = 8 + 4*NUM_OF_LIGHTS) uniform sampler2D depthTexture;
layout(location = 8 + 4*NUM_OF_LIGHTS) uniform bool isConeLight[NUM_OF_LIGHTS];

in vec3 fragPosition;
in vec3 fragNormal;

//Outputs
layout(location = 0) out vec4 fragColor;

//Performs local Tone Mapping with Reinhard's formula
vec3 reinhardToneMap(vec3 color) {
    return color / (color + vec3(1.0));
}

//Trowbridge-Reitz GGX:
//approximates alignment of microfacets
float distributionGGX(vec3 normal, vec3 h) {
    float a = roughness*roughness;
    float a2 = a*a;
    float phi = max(dot(normal, h), 0.0);
    float phi2 = phi*phi;

    float denom = (phi2 * (a2 - 1.0) + 1.0);

    return a2 / (denom * denom * PI);
}

//Geometry functions: approximate how much microfacets occlude each other
float schlickGGX(float phi) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    return phi / (phi * (1.0 - k) + k);
}

float geometrySmith(vec3 normal, vec3 camVec, vec3 lightVec) {
    float phi = max(dot(normal, camVec), 0.0);
    float theta = max(dot(normal, lightVec), 0.0);
    return schlickGGX(theta)*schlickGGX(phi);
}

//Calculates the ratio of surface reflection at different angles (Fresnel)
//using the Schlick approximation
vec3 fresnelSchlick(float cos, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cos, 0.0, 1.0), 5.0);
}

float getFallOff(vec3 fragPosition, mat4 lightMVP, bool isConeLight) {
    if (!isConeLight) {
        return 1.0f;
    }
    vec4 fragLightCoord = lightMVP * vec4(fragPosition, 1.0);
    fragLightCoord.xyz /= fragLightCoord.w;
    fragLightCoord.xyz = fragLightCoord.xyz * 0.5 + 0.5;
    vec2 shadowMapCoord = fragLightCoord.xy;
    float dist = distance(shadowMapCoord, vec2(0.5, 0.5));
    return max(0.0, 1-dist);
}

//Calculates how much the point is in shadow
float shadowContribution(vec3 fragPos, vec3 normal, sampler2D texShadow, vec3 lightPosition, mat4 lightMVP, bool isConeLight) {
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

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec2 filteredCoord = shadowMapCoord + vec2(x, y)*texelSize;
            float shadowMapDepth = texture(texShadow, filteredCoord).x;
            if ((shadowMapDepth < fragLightDepth) && fragLightDepth < 1.0 && fragLightCoord.x < 1.0 && fragLightCoord.y < 1.0 && fragLightCoord.z < 1.0) {
                result += 0.111f;// (1 / 9)
            }
        }
    }
    return result;
}


void main() {
    vec3 normal = normalize(fragNormal);
    vec3 camVec = normalize(cameraPos - fragPosition);
    vec3 albedo = vec3(alpha_albedo);
    //Precalculate base reflectivity so we can use Schlick for
    //both dielectrics and conductors
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 result = vec3(0.0);
    float shadowScale = 0.0;
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
        float normalDistribution = distributionGGX(normal, H);
        vec3 F = fresnelSchlick(clamp(dot(camVec, H), 0.0, 1.0), F0);
        float G = geometrySmith(normal, camVec, lightVec);

        vec3 numerator = normalDistribution*F*G;
        float denominator = (4.0 * cosPhi * cosTheta) + epsilon;

        //Specular part
        vec3 specular = numerator / denominator;
        vec3 ks = F;

        //Lambertian diffuse
        vec3 kd = vec3(1.0) - ks;
        kd = kd * (1.0 - metallic);
        vec3 diffuse = kd * (albedo / PI);

        shadowScale = shadowContribution(fragPosition, normal, shadowMaps[i], lightPos[i], lightMVPs[i], isConeLight[i]);
        result += getFallOff(fragPosition, lightMVPs[i], isConeLight[i])*(1-shadowScale)*(diffuse + specular) * radiance * cosTheta;
    }

    //Add ambient
    result += albedo*ambient;

    //Tone mapping and gamma correction
    result = pow(reinhardToneMap(result), vec3(1.1/2.2));

    //
    //    vec2 fragDepthCoord = fragPosition.xy;
    //    float fragDepth = texture(depthTexture, fragDepthCoord).x;

    fragColor = vec4(result, alpha_albedo.w);
}
