#version 330 core

in vec3 out_pos;
out vec4 FragColor;

uniform vec3 rgb;

void main () {
  FragColor = vec4(rgb, 1.0);
//  FragColor = vec4(out_pos.z, out_pos.z, out_pos.z, 1);
}
