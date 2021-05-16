#version 330 core

in vec3 TexCoords;

uniform samplerCube uTextureSkybox;

void main()
{
	gl_FragColor = texture(uTextureSkybox, TexCoords);
}