#version 310 es
layout (location = 0) in vec3 aPos;

precision highp float;

out vec3 TexCoords;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

void main()
{
    TexCoords = aPos;
    gl_Position = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
}  