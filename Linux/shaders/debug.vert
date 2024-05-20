#version 310 es

precision highp float;

layout (std140,binding = 0) uniform Matrices
{
	uniform mat4 projection;
	uniform mat4 view;
};

in vec3 position;

void main() 
{
    gl_Position = projection * view * vec4(position, 1.0);
}
