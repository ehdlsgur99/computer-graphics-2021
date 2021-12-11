#version 330 core

layout (location = 0) in vec3 in_Pos;

out vec3 TexCoords;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;

void main()
{
    TexCoords = in_Pos * 2;
    vec4 pos = projectionTransform * viewTransform * vec4(in_Pos * 2, 1.0);
    gl_Position = pos.xyww;
}  
