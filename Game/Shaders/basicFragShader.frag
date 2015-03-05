#version 150

in vec2 fragmentPosition;
in vec4 fragmentColor;
out vec4 fragColor;

uniform float time;

void main()
{
	fragColor = fragmentColor * vec4((sin(time + fragmentPosition.y) + 1) * 0.5,(cos(time + fragmentPosition.y) + 1) * 0.5, 0, 0);
}