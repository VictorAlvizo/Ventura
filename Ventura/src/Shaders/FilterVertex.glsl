#version 330 core
layout(location = 0) in vec2 vs_Pos;
layout(location = 1) in vec2 vs_TexCoords;

uniform int u_DistortionType;
uniform float u_Time;
uniform float u_Strength;

out vec2 g_TexCoords;

void main() {
	gl_Position = vec4(vs_Pos.x, vs_Pos.y, 0.0f, 1.0f);

	if(u_DistortionType == 1) { //Upside down
		g_TexCoords = vec2(1.0f - vs_TexCoords.x, 1.0f - vs_TexCoords.y);
	}else if(u_DistortionType == 2) { //Mirage
		g_TexCoords = vec2(vs_TexCoords.x + sin(u_Time) * u_Strength, vs_TexCoords.y + cos(u_Time) * u_Strength);
	}else if(u_DistortionType == 3) { //Shake
		gl_Position.x += cos(u_Time * 10) * u_Strength;
		gl_Position.y += cos(u_Time * 15) * u_Strength;
		g_TexCoords = vs_TexCoords;
	} else {
		gl_Position = vec4(vs_Pos.x, vs_Pos.y, 0.0f, 1.0f);
		g_TexCoords = vs_TexCoords;
	}
}