#version 450

out vec4 FragColor;

in vec2 fragTexCoord;

uniform sampler2D depthMap;

//// required when using a perspective projection matrix
//float LinearizeDepth(float depth)
//{
//    float z = depth * 2.0 - 1.0; // Back to NDC
//    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
//}
//

void main()
{
    float near_plane = 1.0f;
    float far_plane = 2.0f;
    float depthValue = texture(depthMap, fragTexCoord).r;
    // FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    float z = depthValue * 2.0 - 1.0;
    float val = (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}

