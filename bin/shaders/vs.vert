#version 330

attribute vec3 position;

uniform mat4 GT;
uniform mat4 VT;
uniform mat4 PT;

void main()
{
    gl_Position = PT * VT * GT * vec4(position, 1.0);
}