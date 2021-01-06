#version 330 core

in vec2 g_TexCoords;

uniform sampler2D u_ScreenTexture;
uniform vec3 u_ColorOverlay;
uniform vec2 u_Offsets[9];
uniform float u_Kernel[9];
uniform bool u_KernelEnabled;

out vec4 fragColor;

void main() {
    if(u_KernelEnabled) {
        vec3 sampleTex[9];

        for(int i = 0; i < 9; i++){
            sampleTex[i] = vec3(texture(u_ScreenTexture, g_TexCoords.st + u_Offsets[i]));
        }

        vec3 col = vec3(0.0f);
        for(int i = 0; i < 9; i++){
            col += sampleTex[i] * u_Kernel[i];
        }

        fragColor = vec4(col, 1.0f) * vec4(u_ColorOverlay, 1.0f);
    }else {
        fragColor = texture(u_ScreenTexture, g_TexCoords) * vec4(u_ColorOverlay, 1.0f);
    }
}