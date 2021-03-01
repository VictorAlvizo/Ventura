#version 330 core

in vec2 g_TexCoords;

uniform sampler2D u_Text;
uniform vec4 u_TextColor;

out vec4 fragColor;

void main() {
	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(u_Text, g_TexCoords).r);
	fragColor = u_TextColor * sampled;
}