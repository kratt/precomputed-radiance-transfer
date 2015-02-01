//Author: Sören Pirk
//Date: 22.01.2013

#version 400 core

uniform sampler2D tex; 

uniform vec3 diffuseColor;
uniform vec3 ambientColor;
uniform vec3 specularColor;

uniform vec3 camPos;
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform float shadowIntensity;
uniform int renderMode;
uniform float alpha;
uniform int applyShadow;

uniform sampler2D shadowMapBlurred;
uniform sampler2D shadowMap;

in vec4 VertPosition;
in vec4 VertColor;
in vec4 VertTexture;
in vec4 VertNormal;
in vec4 VertShadowCoord;

float attenuation(vec3 P, vec3 L, float kC, float kL, float kQ)
{
    float d = distance(P, lightPos);
    float r = 1 / (kC + kL * d + kQ * d * d);
    return r;
}

float dualConeSpotlight(vec3 P, vec3 L, float outerCone, float innerCone)
{
    vec3 V = normalize(L-P);
    float d = dot(V, normalize(-lightDir));
    return smoothstep(outerCone, innerCone, d);
}

float VSM(vec4 smcoord)
{
	vec3 coords = smcoord.xyz / smcoord.w ;    

    if(smcoord.z < 1)
        return 1;

    float depth = 1;//coords.z;
    
    vec4 depthBlurrred = texture(shadowMapBlurred, coords.xy);

    float depthSM = depthBlurrred.x;
	float sigma2  = depthBlurrred.y;

    float realDepth = texture(shadowMap, coords.xy).x;

	sigma2 -= depthSM * depthSM;

	float bias = 0.0001;

	float dist = depth - depthSM;
	float P = sigma2 / ( sigma2 + dist * dist );
	float lit = max( P, ( depth - bias ) <= depthSM ? 1.0 : 0.0);
	lit = min(1.0, lit);   

    return mix(shadowIntensity, 1.0, lit);

    return lit;
}

void main()
{
    float shadow = 1.0;
    if(applyShadow == 1)    
    {
       shadow = VSM(VertShadowCoord);
    }

    vec4 texColor = texture(tex, VertTexture.st);

    vec3 N = normalize(VertNormal.xyz);
    vec3 L = normalize(lightPos-VertPosition.xyz);

    float d = max(0.0, dot(N, L));   
    float t = dualConeSpotlight(VertPosition.xyz, lightPos, 0.1, 1.0);
    float a = attenuation(VertPosition.xyz, lightPos-VertPosition.xyz, 1.01, 0.00, 0.0);

    vec3 ambient  = texColor.rgb * ambientColor;
    vec3 diffuse  = texColor.rgb * diffuseColor;

    vec4 color;
    if(renderMode == 0)
    {
        color.xyz = (ambient + diffuse) * shadow * d;
        color.a = alpha;
    }
    if(renderMode == 1)
    {
        color.xyz = vec3(0.2, 0.2, 0.2) * shadow;
        color.a = alpha;
    }
    if(renderMode == 2)
    {
        color.xyz = vec3(1.0, 1.0, 1.0) * shadow;
        color.a = alpha;
    }
    if(renderMode == 3)
    {
        color.xyz = vec3(1.0, 1.0, 1.0) * shadow * d;
        color.a = alpha;
    }

    gl_FragColor = vec4(color.xyz, alpha);	
}