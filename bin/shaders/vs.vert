#version 450

in vec3 position;
in vec3 normal;
in vec3 ka;
in vec3 kd;
in vec3 ks;
in float n;

out vec4 color;

uniform mat4 GT;
uniform mat4 VT;
uniform mat4 PT;

vec3 posFocus = vec3(0.0, 0.0, 0.0);
vec3 colFocus = vec3(1.0, 1.0, 1.0);

void main()
{
	vec3 tmpColor = kd;
	
	color = vec4(tmpColor, 1.0);
	gl_Position = PT * VT * GT * vec4(position, 1.0);
}