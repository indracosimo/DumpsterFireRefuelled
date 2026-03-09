#version 330 core
layout (location = 0) in vec3 in_Position;

uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = lightSpaceMatrix * modelMatrix * vec4(in_Position, 1.0);
}