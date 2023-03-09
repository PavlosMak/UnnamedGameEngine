#version 450


layout(location = 3) uniform vec3 lightPos;
layout(location = 4) uniform vec3 lightColor;
layout(location = 5) uniform vec3 kd;

layout(location = 0) out vec4 fragColor;


in vec3 fragPosition;
in vec3 fragNormal;

void main() {

    vec3 lightVec = normalize(lightPos-fragPosition);
    float theta = max(0, dot(lightVec, fragNormal));

    fragColor = vec4(kd*lightColor*theta, 1.0f);

}
