#version 330 core

uniform float time;

in vec3 fragColor;
layout(location = 0) out vec4 diffuseColor;

void main() {
    diffuseColor = vec4(fragColor, 1);
}
