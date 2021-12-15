#version 450

in vec4 ex_Color;
in vec2 ex_TexCoords;

layout(location=3) uniform sampler2D uTexture;

uniform float uTextureBool;

out vec4 out_Color;

void main()
{
    //if(uTextureBool > 0.0){
        out_Color = texture2D(uTexture, ex_TexCoords);
        if(out_Color.w < 0.5) {
            discard;
        }
    /*}
    else{
        out_Color = ex_Color;
    }*/
}
