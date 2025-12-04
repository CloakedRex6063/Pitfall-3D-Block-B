#version 310 es

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;
layout(location = 7) in mat4 model;

precision highp float;

layout (std140,binding = 0) uniform Matrices
{
	uniform mat4 projection;
	uniform mat4 view;
};

out vec2 TexCoords;

void main()
{
    TexCoords = tex;    
    gl_Position = projection * view * model * vec4(pos, 1.0);
}