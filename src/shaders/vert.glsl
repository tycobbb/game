#version 330 core

layout(location=0) in vec3 in_Pos;
layout(location=1) in vec3 in_Color;

out vec3 vert_Color;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

void main() {
    mat4 transform = projMatrix * viewMatrix;
    vec4 pos = transform * vec4(in_Pos, 1.0);

    vert_Color = in_Color;
    gl_Position = pos;
}