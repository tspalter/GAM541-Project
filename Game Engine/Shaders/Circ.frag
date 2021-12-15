#version 450

in vec2 ex_Position;

uniform vec4 uColor1;
uniform vec4 uColor2;

uniform vec2 uAngs;

uniform float uFill, uIn;

out vec4 out_Color;

void main()
{
    float xx = ex_Position.x;
    float yy = ex_Position.y;
    float dist = sqrt(xx*xx+yy*yy);
    float ang = atan(yy, xx);
    
    out_Color = uColor1;

    if( dist > 0.5 * (uFill + (uIn * (1.0-uFill))) )
        out_Color = uColor2;
    
    if( dist > 0.5 || dist < 0.5 * uIn )
        discard;

    if ( !((ang > uAngs.x && ang < uAngs.y) || ang < uAngs.y-2.0*3.14159 || ang > uAngs.x+2.0*3.14159) )
        discard;
    
}
