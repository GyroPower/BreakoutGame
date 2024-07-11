#version 330 core 

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureImg;
uniform vec4 color; 


void main(){

    FragColor = (texture(textureImg, TexCoords) * color);

}