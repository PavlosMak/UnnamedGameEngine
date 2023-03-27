#version 450
layout(location = 3) uniform vec3 color;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(color, 1);
}

