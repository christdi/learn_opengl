#version 330 core
out vec4 FragColor;

in vec3 Colour;
in vec2 TexCoord;

uniform float Mix;
uniform sampler2D Texture1;
uniform sampler2D Texture2;

void main() {
    FragColor = mix(texture(Texture1, TexCoord), texture(Texture2, vec2(-TexCoord.x, TexCoord.y)), Mix);
} 