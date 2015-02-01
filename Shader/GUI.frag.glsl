//Author: Sören Pirk
//Date: 22.01.2013

#version 400 core

in vec4 VertPosition;
in vec3 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

void main()
{
   vec4 color = VertColor;
   gl_FragColor = VertColor;	
}
