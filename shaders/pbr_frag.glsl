#version 450


//Constants
const float PI = 3.14159;
const float epsilon = 0.000001; //small epsilon to avoid div by 0

//Inputs
layout(location = 3) uniform vec3 lightPos;
layout(location = 4) uniform vec3 lightColor;
layout(location = 5) uniform vec3 cameraPos;

layout(location = 6) uniform vec3 albedo;
layout(location = 7) uniform float roughness;
layout(location = 8) uniform float metallic;

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

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 camVec = normalize(cameraPos - fragPosition);

    //Precalculate base reflectivity so we can use Schlick for
    //both dielectrics and conductors
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 result = vec3(0.0);
    for (int i = 0; i < 1; i++) {
        vec3 lightVec = normalize(lightPos - fragPosition);
        vec3 H = normalize(lightVec + camVec);

        float cosTheta = max(dot(lightVec, normal), 0.0);
        float cosPhi = max(dot(normal, camVec), 0.0);

        //Scale contribution of light based on distance
        float distanceToLight = length(lightPos - fragPosition);
        float attenuation = 1.0 / (distanceToLight * distanceToLight);
        vec3 radiance = lightColor*attenuation;

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

        result += (diffuse + specular) * radiance * cosTheta;
    }

    //Tone mapping and gamma correction
    result = pow(reinhardToneMap(result), vec3(1.1/2.2));

    fragColor = vec4(result, 1.0);
}
