#version 330 core
layout(location = 0) in vec2 vs_Pos;
layout(location = 1) in vec2 vs_TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_Model;

layout(std140) uniform Vision {
	mat4 u_View;
};

uniform bool u_Flipped;

out vec2 g_TexCoords;

void main() {
	gl_Position = u_Projection * u_View * u_Model * vec4(vs_Pos, 0.0f, 1.0f);
	g_TexCoords = (u_Flipped) ? vec2(1.0f - vs_TexCoords.x, vs_TexCoords.y) : vs_TexCoords;
}