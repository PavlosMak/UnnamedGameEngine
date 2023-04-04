#version 450

layout(location = 0) uniform mat4 mvpMatrix;
layout(location = 1) uniform mat4 modelMatrix;
// Normals should be transformed differently than positions:
// https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
layout(location = 2) uniform mat3 normalModelMatrix;
layout(location = 3) uniform sampler2D heightMap;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main()
{
    gl_Position = mvpMatrix * vec4(position, 1);

    fragPosition = (modelMatrix * vec4(position, 1)).xyz;
    fragNormal = normalize(normalModelMatrix * normal);
    fragTexCoord = texCoord;

    //offset by height map
    float offset = texture(heightMap, fragTexCoord).r;
    fragPosition = fragPosition + fragNormal*offset;
}
