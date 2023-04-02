#version 450

const int NUM_OF_LIGHTS = 2;

layout(location = 3) uniform sampler2D toonTexture;
layout(location = 4) uniform float timeMoment;
layout(location = 5) uniform vec3 lightPos[NUM_OF_LIGHTS];
layout(location = 5 + NUM_OF_LIGHTS) uniform vec3 lightColor[NUM_OF_LIGHTS];

layout(location = 0) out vec4 fragColor;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

void main()
{
    vec3 texValue =  texture(toonTexture, vec2(timeMoment, fragTexCoord.y)).xyz;
    vec3 result = vec3(0.0);
    vec3 normal = normalize(fragNormal);
    for (int i = 0; i < NUM_OF_LIGHTS; i++) {
        vec3 lightVec = normalize(lightPos[i] - fragPosition);

        float cosTheta = max(dot(lightVec, normal), 0.0);

        //Scale contribution of light based on distance
        float distanceToLight = length(lightPos[i] - fragPosition);
        float attenuation = 1.0 / (distanceToLight * distanceToLight);
        vec3 radiance = lightColor[i]*attenuation;

//        shadowScale = shadowContribution(fragPosition, normal, shadowMaps[i], lightPos[i], lightMVPs[i]);
//        result += (1 - shadowScale) * (diffuse + specular) * radiance * cosTheta;
        result += cosTheta*texValue;
    }

    fragColor = vec4(result,1.0);
}

