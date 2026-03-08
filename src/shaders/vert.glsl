#version 330 core

// AAA: figure out how to have a dynamic number of model matricies

layout(location=0) in uint in_MeshIndex;
layout(location=1) in vec3 in_Pos;
layout(location=2) in vec3 in_Color;

out vec3 vert_Color;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrices[2];

void main() {
    mat4 transform = projMatrix * viewMatrix * modelMatrices[in_MeshIndex];
    vec4 pos = transform * vec4(in_Pos, 1.0);

    vert_Color = in_Color;
    gl_Position = pos;
}