#version 330 core

layout(location=0) out vec4 fragmentColor;

in vec3 vert_Color;

void main() {
    fragmentColor = vec4(vert_Color, 1.0);
}