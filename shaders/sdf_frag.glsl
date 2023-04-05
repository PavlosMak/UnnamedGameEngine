#version 450
layout(location = 3) uniform sampler2D sdfTexture;
layout(location = 4) uniform float alphaClip;

layout(location = 0) out vec4 fragColor;

in vec2 fragTexCoord;

void main()
{
    float alpha = texture(sdfTexture, fragTexCoord).r;
    if(alpha < alphaClip) {
        discard;
    }
    float value = 1.0f;
    fragColor = vec4(value);
}

