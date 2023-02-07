#version 450 core
#define M_PI 3.1415926535897932384626433832795

uniform float time;

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoords;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec2 fragTexCoords;

void main() {
    gl_Position = vec4(aPos, 1.0, 1.0);
    fragTexCoords = vec2(texCoords.x, 1.0 - texCoords.y);
    float x = aPos.x/10;
    float y = aPos.y/10;
    float b = x + y;
    fragColor = color + b; // vec3(color.x + b, color.y + b, color.z + b);
}
