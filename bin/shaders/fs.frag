#version 450

in vec3 positionF;
in vec3 normalF;
in vec3 kaF;
in vec3 kdF;
in vec3 ksF;
in float nF;

in vec3 focusF;

vec3 colFocus = vec3(1.0, 1.0, 1.0);

vec3 Ambient()
{
	return (kaF);
}

vec3 Diffuse()
{
	vec3 col = vec3(0.0, 0.0, 0.0);

	vec3 L = normalize(focusF - positionF);

	float alpha = dot(normalF, L);

	if(alpha > 0)
	{
		col = kdF * colFocus * alpha;
	}

	return (col);
}

vec3 Specular()
{
	vec3 col = vec3(0.0, 0.0, 0.0);

	vec3 L = normalize(focusF - positionF);

	float alpha = dot(normalF, L);

	if(alpha > 0)
	{
		vec3 R = reflect(-L, normalF);
		vec3 V = normalize(-focusF);

		float beta = dot(R, V);

		if(beta > 0 && nF > 0.0)
		{
			float shine = pow(max(0.0, beta), nF);

			col = ksF * colFocus * shine;
		}

	}

	return (col);
}

void main()
{
    vec3 color = Ambient() + Diffuse() + Specular();
    gl_FragColor = vec4(color, 1.0);
}