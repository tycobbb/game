#version 330 core

layout(location=0) out vec4 fragmentColor;

in vec3 vert_Color;
in vec3 vert_Normal;

void main() {
    vec3 color = normalize(vert_Normal) * 0.5 + 0.5;
    fragmentColor = vec4(color, 1.0);
}