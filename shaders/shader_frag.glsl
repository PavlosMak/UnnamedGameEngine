#version 450
layout(location = 3) uniform sampler2D colorMap;
layout(location = 4) uniform bool hasTexCoords;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
    const vec3 normal = normalize(fragNormal);

    if (hasTexCoords) {
        fragColor = vec4(texture(colorMap, fragTexCoord).rgb, 1);
    } else {
        // Output color value, change from (1, 0, 0) to something else
        fragColor = vec4(normal, 1);
    }
}
