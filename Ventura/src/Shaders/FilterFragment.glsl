#version 330 core

in vec2 g_TexCoords;

uniform sampler2D u_ScreenTexture;

out vec4 fragColor;

void main() {
	fragColor = vec4(vec3(1.0f - texture(u_ScreenTexture, g_TexCoords)), 1.0f);
}