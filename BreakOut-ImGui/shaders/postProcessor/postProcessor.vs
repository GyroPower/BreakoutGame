#version 420 core

layout (location = 0) in vec4 vertex;

out vec2 TexCoords;


void main(){
	
	gl_Position = vec4(vertex.xy, 0.0f, 1.0f);

	vec2 texture = vertex.zw;

	TexCoords = texture;

}