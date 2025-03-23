#version 330 core

layout(location=0) in vec3 in_Pos;
layout(location=1) in vec3 in_Color;
out vec3 vColor;
void main() {
  vColor = in_Color;
  gl_Position = vec4(in_Pos.x, in_Pos.y, in_Pos.z, 1.0);
}