#version 330 core
layout(location = 0) in vec2 vs_Pos;

uniform mat4 u_Projection;
uniform mat4 u_Model;

layout(std140) uniform Vision {
	mat4 u_View;
};

void main() {
	gl_Position = u_Projection * u_View * u_Model * vec4(vs_Pos, 0.0f, 1.0f);
}