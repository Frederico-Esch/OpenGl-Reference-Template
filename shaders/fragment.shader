#version 450 core

uniform float time;

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec2 fragTexCoords;

layout (location = 0) out vec4 diffuseColor;

uniform sampler2D fabricTexture;
uniform int whichTexture;

void main() {
    if(whichTexture >= 0){
        diffuseColor = vec4(fragColor, 1)*texture(fabricTexture, fragTexCoords);
    } else {
        diffuseColor = vec4(fragColor, 1);
    }
}
