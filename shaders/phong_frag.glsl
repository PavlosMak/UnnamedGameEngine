#version 450

#define LIGHTS 2

layout(location = 3) uniform vec3 lightPos;
layout(location = 4) uniform vec3 lightColor;
layout(location = 5) uniform vec3 cameraPos;
layout(location = 6) uniform vec3 kd;
layout(location = 7) uniform float shininess;

layout(location = 0) out vec4 fragColor;


in vec3 fragPosition;
in vec3 fragNormal;

vec3 lightContribution(vec3 lightVec, vec3 lightColor, vec3 cameraVec) {
    float theta = max(0, dot(lightVec, fragNormal));
    vec3 reflection = normalize(reflect(-lightVec, fragNormal));
    float phi = max(0.0, dot(reflection, cameraVec));
    vec3 specular = kd*pow(phi, shininess);
    return lightColor*(kd*theta + kd*phi);
}

void main() {
    vec3 result = vec3(0.0);
    for (int i = 0; i < LIGHTS; i++) {
        vec3 lightVec = normalize(lightPos - fragPosition);
        vec3 cameraVec = normalize(cameraPos - fragPosition);
        result += lightContribution(lightVec, lightColor, cameraVec);
    }
    fragColor = vec4(result, 1.0f);
}
