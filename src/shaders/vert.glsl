#version 330 core

layout(location=0) in uint in_MeshIndex;
layout(location=1) in vec3 in_Pos;
layout(location=2) in vec3 in_Normal;
layout(location=3) in vec3 in_Color;

out vec3 vert_Color;
out vec3 vert_Normal;

// AAA: figure out how to have a dynamic number of model matricies
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrices[2];

void main() {
    mat4 modelMatrix = modelMatrices[in_MeshIndex];
    mat4 transform = projMatrix * viewMatrix * modelMatrix;

    vec4 pos = transform * vec4(in_Pos, 1.0);
    vec4 normal = modelMatrix * vec4(in_Normal, 0.0);

    vert_Color = in_Color;
    vert_Normal = in_Normal.xyz;
    gl_Position = pos;
}