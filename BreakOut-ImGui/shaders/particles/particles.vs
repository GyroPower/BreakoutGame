#version 420 core

layout (location = 0) in vec4 vertex;

out vec2 TexCoords;
out vec4 partColor;

uniform mat4 projection;
uniform mat4 model;
uniform vec2 offset;
uniform vec4 color;



void main(){
	float scale = 10.0f;

	TexCoords = vertex.zw;
	partColor = color;

	gl_Position = projection * model * vec4((vertex.xy * scale),0.0f,1.0f); 
}