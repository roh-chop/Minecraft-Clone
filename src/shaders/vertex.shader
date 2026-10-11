#version 330

layout(location=0)in vec4 in_position;
layout(location=1)in vec3 in_texCoord;

 

uniform mat4 globalMatrix;
uniform mat4 camPos;
uniform mat4 camRot;

out vec2 ex_texCoord;
out float colMul;
out float out_alpha;

void main(){
    gl_Position = globalMatrix*camRot*camPos*vec4(in_position.x,in_position.y,in_position.z,1.0);
    ex_texCoord = vec2(in_texCoord.x,in_texCoord.y);
    out_alpha = in_texCoord.z;
    colMul = in_position.w;
}