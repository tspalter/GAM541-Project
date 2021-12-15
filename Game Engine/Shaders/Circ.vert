#version 450

layout(location=0) in vec3 in_Position;
layout(location=4) uniform mat4 uTransform;

uniform vec2 uScreenSize, uSpriteSize;

out vec4 ex_Color1;
out vec4 ex_Color2;
out vec2 ex_Position;

void main()
{
    gl_Position = uTransform * vec4(in_Position, 1.0);
    ex_Position = in_Position.xy;
}
