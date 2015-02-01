#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

uniform vec3 lightPos;

void main()
{
   gl_FragColor = vec4(0, 0, 0, 1);	
}
