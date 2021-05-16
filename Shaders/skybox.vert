#version 330 core

in vec3 vskyposition;
out vec3 TexCoords;

uniform mat4 uMViewMatrix;
uniform mat4 uProjMatrix;


void main()
{
	TexCoords = vskyposition;
	gl_Position = uProjMatrix * uMViewMatrix * vec4 (vskyposition, 1.0);
}