#version 450

in vec2 ex_Position;

uniform vec4 uColor1;
uniform vec4 uColor2;

uniform float uFill;

out vec4 out_Color;

void main()
{
    out_Color = uColor1;

    float dist = sqrt(ex_Position.x*ex_Position.x + ex_Position.y*ex_Position.y);
    if(ex_Position.x < 0)
        dist *= -1;

    if( ex_Position.x > uFill-0.5 )
        out_Color = uColor2;
}
