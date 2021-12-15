#version 450

layout(location=0) in vec3 in_Position;
layout(location=1) in vec4 in_Color;
layout(location=2) in vec2 in_TexCoords;

layout(location=4) uniform mat4 uTransform;

out vec4 ex_Color;
out vec2 ex_TexCoords;

void main()
{
    ex_Color = in_Color;
    gl_Position = uTransform * vec4(in_Position, 1.0);
    ex_TexCoords = in_TexCoords;
}
