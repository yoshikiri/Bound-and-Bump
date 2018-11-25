#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 inTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 projection;

void main() {
  vertexColor = vec4(color, 1.0);
  TexCoord = inTexCoord;
  gl_Position = projection * view * model * vec4(position, 1.0);
}