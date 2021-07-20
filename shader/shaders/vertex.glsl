#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;

out vec3 colour;

uniform float offset;

void main()
{
    gl_Position = vec4(aPos.x + offset, -aPos.y, aPos.z, 1.0);
    colour = vec3(gl_Position);
}