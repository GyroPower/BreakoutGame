#version 330 core 

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureImg;
uniform vec3 color; 


void main(){

    FragColor = vec4(color, 1.0) * texture(textureImg, TexCoords);

}