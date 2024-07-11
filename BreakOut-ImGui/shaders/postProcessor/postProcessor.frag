#version 420 core

out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D scene;


void main(){

	fragColor = vec4(0.0f);

	fragColor = texture(scene,TexCoords);
}