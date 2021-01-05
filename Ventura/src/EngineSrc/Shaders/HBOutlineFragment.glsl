#version 330 core

uniform vec3 u_Color;

out vec4 fragColor;

void main() {
	fragColor = vec4(u_Color, 1.0f);
}