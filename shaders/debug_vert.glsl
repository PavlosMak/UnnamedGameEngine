#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 fragTexCoord;

void main()
{
    gl_Position = vec4(position, 1);
    fragTexCoord = texCoord;
}
