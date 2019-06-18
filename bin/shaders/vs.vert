#version 450

in vec3 position;
in vec3 normal;
in vec3 ka;
in vec3 kd;
in vec3 ks;
in float n;

out vec3 positionF;
out vec3 normalF;
out vec3 kaF;
out vec3 kdF;
out vec3 ksF;
out float nF;

out vec3 focusF;

uniform mat4 GT;
uniform mat4 VT;
uniform mat4 PT;

uniform bool SCOfocus;
uniform vec3 focus;

void main()
{
	positionF = (VT * GT * vec4(position, 1.0)).xyz;

	mat3 NM = inverse(transpose(mat3(VT * GT)));
	normalF = normalize(NM * normal);

	kaF = ka;
	kdF = kd;
	ksF = ks;
	nF = n;

	if(SCOfocus) focusF = focus;
	else focusF = (VT * vec4(focus, 1.0)).xyz;

	gl_Position = PT * VT * GT * vec4(position, 1.0);
}