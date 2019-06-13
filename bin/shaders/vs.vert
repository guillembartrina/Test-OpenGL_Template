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

bool camFocus = false;;
vec3 posFocus = vec3(4.0, 0.0, 4.0);
vec3 colFocus = vec3(1.0, 1.0, 1.0);

vec3 Ambient()
{
	return (ka);
}

vec3 Diffuse()
{
	vec3 col = vec3(0.0, 0.0, 0.0);

	mat3 NM = inverse(transpose(mat3(VT * GT)));
	vec3 tmpNormal = NM * normal;

	if(!camFocus) posFocus = (VT * vec4(posFocus, 1.0)).xyz;

	vec3 tmpL = posFocus - (VT * GT * vec4(position, 1.0)).xyz;

	float tmpPhi = dot(normalize(tmpNormal), normalize(tmpL));

	if(tmpPhi > 0)
	{
		col = kd * colFocus * tmpPhi;
	}

	return (col);
}

void main()
{
	vec3 tmpColor = Ambient() + Diffuse();

	color = vec4(tmpColor, 1.0);
	gl_Position = PT * VT * GT * vec4(position, 1.0);
}