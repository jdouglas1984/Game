#version 150

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;
out vec4 fragColor;

//uniform float time;
uniform sampler2D playerTexture;

void main()
{
	vec4 textureColor = texture(playerTexture, fragmentUV);
	fragColor = textureColor * fragmentColor; 
	//fragColor = fragmentColor * vec4((sin(time + fragmentPosition.y) + 1) * 0.5,(cos(time + fragmentPosition.y) + 1) * 0.5, (cos(time + fragmentPosition.y) + 1) * 0.5, 0);
}