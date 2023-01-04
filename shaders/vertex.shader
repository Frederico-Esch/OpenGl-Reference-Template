#version 330 core
#define M_PI 3.1415926535897932384626433832795

uniform float time;

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 color;

out vec3 fragColor;

void main() {
    gl_Position = vec4(aPos, 1.0, 1.0);
    //float Hdeg = time + color.y*120 + color.z*240 ;
    //Hdeg = mod(Hdeg, 2*M_PI);
    //Hdeg = 360*Hdeg/(2*M_PI);
    //float H = mod(Hdeg/60, 2);

    //float C = 1;
    //float X = (1 - abs(H - 1));

    //if(Hdeg < 60) {
    //    fragColor = vec3(C, X, 0);
    //} else if (Hdeg < 120) {
    //    fragColor = vec3(X, C, 0);
    //} else if (Hdeg < 180) {
    //    fragColor = vec3(0, C, X);
    //} else if (Hdeg < 240) {
    //    fragColor = vec3(0, X, C);
    //} else if (Hdeg < 300) {
    //    fragColor = vec3(X, 0, C);
    //} else {
    //    fragColor = vec3(C, 0, X);
    //}

    float x = aPos.x/10;
    float y = aPos.y/10;
    float b = x + y;
    fragColor = color + b; // vec3(color.x + b, color.y + b, color.z + b);
}
