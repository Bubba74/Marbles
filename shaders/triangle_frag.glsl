#version 330 core

in vec3 out_pos;
out vec4 FragColor;

void main () {
  FragColor = vec4(out_pos,0.9);
}
