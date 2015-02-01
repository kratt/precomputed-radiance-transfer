#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;
in vec4 VertShadowCoord;

uniform vec3 lightPos;
uniform vec3 camPos;
uniform int applyShadow;
uniform float shadowIntensity;

uniform sampler2D shadowMapBlurred;
uniform sampler2D shadowMap;

uniform int isSelected = 0;

float VSM(vec4 smcoord)
{
	vec3 coords = smcoord.xyz / smcoord.w ;    

    if(smcoord.z < 1)
       return 1;

    float depth =  coords.z;
    
    vec4 depthBlurrred = texture(shadowMapBlurred, coords.xy);

    float depthSM = depthBlurrred.x;
	float sigma2  = depthBlurrred.y;

    float realDepth = texture(shadowMap, coords.xy).x;

	sigma2 -= depthSM * depthSM;

	float bias = 0.000001;

	float dist = depth - depthSM;
	float P = sigma2 / ( sigma2 + dist * dist);
 	float lit = max( P, ( depth - bias ) <= depthSM ? 1.0 : 0.0);
	lit = min(1.0, lit);   

    return mix(shadowIntensity, 1.0, lit);
}

void main()
{
    vec4 color = VertColor;

    float shadow = 1.0;
    if(applyShadow == 1)    
    {
       shadow = VSM(VertShadowCoord);
    }


    vec3 P = VertPosition.xyz;
    vec3 N = normalize(VertNormal.xyz);
    vec3 L = normalize(lightPos.xyz - P);
    vec3 V = normalize(camPos.xyz-P);
    vec3 H = normalize(L+V);

    float d = max(dot(N, L), 0);
    float s = pow(max(dot(N, H), 0), 128);

    if(d <= 0)
        s = 0;
	
    vec3 lColor      = vec3(1, 1, 1);
    vec3 matAmbient  = vec3(0.0, 0.0, 0.0);
    vec3 matDiffuse  = vec3(1, 1, 1);
    vec3 matSpecular = vec3(1, 1, 1);

    if(isSelected == 1)
        matDiffuse = vec3(1, 0, 0);

    vec3 ambient  = matAmbient  * lColor;
    vec3 diffuse  = matDiffuse  * lColor * d * 1.0;
    vec3 specular = matSpecular * lColor * s * 0.0;

    vec4 finalColor = vec4(diffuse + ambient + specular, 1);
    finalColor.xyz *= shadow;

    gl_FragColor = vec4(finalColor.xyz, 1.0);	
}
