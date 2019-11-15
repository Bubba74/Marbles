#version 330 core

in vec3 out_pos;
out vec4 FragColor;

void main () {
  FragColor = 10*vec4(out_pos.z, out_pos.z, out_pos.z, 0.1);
}
