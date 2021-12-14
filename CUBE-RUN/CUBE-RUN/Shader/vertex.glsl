#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoord;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

out vec4 fragLightSpace;
uniform mat4 lightVP;

void main(void)
{
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(in_Position, 1.0);

	FragPos = vec3(modelTransform * vec4(in_Position, 1.0));
	Normal = mat3(transpose(inverse(modelTransform))) * in_Normal;
	TexCoord = in_TexCoord;
	fragLightSpace = lightVP * vec4(FragPos, 1.0);
}