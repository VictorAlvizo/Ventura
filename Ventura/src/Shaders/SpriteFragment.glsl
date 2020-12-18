#version 330 core

in vec2 g_TexCoords;

uniform sampler2D u_Sprite;
uniform vec4 u_Color;

out vec4 fragColor;

void main() {
	fragColor = texture(u_Sprite, g_TexCoords) * u_Color;
}