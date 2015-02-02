#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

uniform vec3 lightPos;
uniform vec3 camPos;

uniform sampler2D texLightProbe;

const float PI = 3.14159265359f;

vec3 sampleLightProbe()
{
	vec3 dir = normalize(reflect(normalize(-camPos), normalize(VertNormal.xyz))); //normalize(VertPosition.xyz);
	
	float d = sqrt(dir.x * dir.x + dir.y * dir.y);
	float r = (d == 0) ? 0.0f : (1.0f/PI/2.0f) * acos(dir.z) / d;

	float u = 0.5f + dir.x * r;
	float v = 0.5f + dir.y * r;

	return texture(texLightProbe, vec2(u, v)).xyz;
}

vec3 tonMapping(vec3 color)
{
	float x = color.x / (1.0f + color.x);
	float y = color.y / (1.0f + color.y);
	float z = color.z / (1.0f + color.z);
	
	return vec3(x, y, z);

}

vec3 Uncharted2Tonemap(vec3 x)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;

	return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

void main()
{
   vec3 N = normalize(VertNormal.xyz);
   vec3 L = normalize(lightPos.xyz);

   float d = max(0.0, dot(N, L));
   
   float exposure = 10.0f;   
   
   vec3 color = Uncharted2Tonemap(sampleLightProbe()*exposure);
   vec3 prtColor = Uncharted2Tonemap(abs(VertColor.xyz*exposure));
   
   vec3 finalColor = mix(color, prtColor, 1.0f);

   gl_FragColor = vec4(finalColor.xyz, 1);	
}
