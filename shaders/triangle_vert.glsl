#version 330 core

layout (location = 0) in vec3 pos;

out vec3 out_pos;

uniform mat4 perspective;
uniform mat4 view;
uniform mat4 model;
uniform mat4 local;

void main () {

//  vec4 tmp = perspective * view * model * local * vec4(pos,1);
//  out_pos.x = tmp.x; out_pos.y = tmp.y; out_pos.z = tmp.z;
  out_pos = pos;
  gl_Position = perspective * view * model * local * vec4(pos,1);
//  gl_Position.w = 1;
}
