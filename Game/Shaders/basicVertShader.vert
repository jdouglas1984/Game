#version 150

in vec2 vertexPosition;
in vec4 vertexColor;

out vec4 fragmentColor;

out vec2 fragmentPosition;

uniform float time;

void main()
{
	gl_Position.xy = vertexPosition;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	
	fragmentPosition = vertexPosition;
	
	fragmentColor = vertexColor;
}