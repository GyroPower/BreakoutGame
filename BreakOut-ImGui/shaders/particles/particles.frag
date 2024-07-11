#version 420 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 partColor;

uniform sampler2D imageTex;




void main(){

	FragColor = (texture(imageTex, TexCoords) * partColor);
}