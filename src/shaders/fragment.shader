#version 330

in vec2 ex_texCoord;
in float colMul;
in float out_alpha;
out vec4 glFragColor;


uniform sampler2D atexture;
uniform bool isWater;

void main(){
    vec4 texColor = texture(atexture,ex_texCoord);
    glFragColor = texColor*colMul;
    glFragColor.w = texColor.w;
    if(out_alpha==1)glFragColor.w = 0.5f;
    
}