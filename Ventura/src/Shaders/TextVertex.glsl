#version 330 core
layout(location = 0) in vec4 vs_Vertex; //(vec2 | Pos, vec2 | Texturee Coordinates)

out vec2 g_TexCoords;

uniform mat4 u_Projection;

void main() {
	gl_Position = u_Projection * vec4(vs_Vertex.xy, 0.0f, 1.0f);
	g_TexCoords = vs_Vertex.zw;
}