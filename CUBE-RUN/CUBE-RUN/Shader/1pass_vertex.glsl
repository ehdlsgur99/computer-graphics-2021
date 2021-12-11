#version 330

layout (location = 0) in vec3 inPos;

uniform mat4 lightSpace;
uniform mat4 modelTransform;

void main(void)
{
	gl_Position = lightSpace * modelTransform * vec4(inPos, 1.0);
}