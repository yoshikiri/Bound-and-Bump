#version 450 core

in vec4 vertexColor;
in vec2 TexCoord;

out vec4 fragment;

uniform vec4 myColor;

uniform sampler2D myTexture1;
//uniform sampler2D myTexture2;

void main(){
  fragment = texture(myTexture1, TexCoord) + vec4(vertexColor);
}