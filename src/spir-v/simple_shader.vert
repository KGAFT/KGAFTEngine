#version 450

layout(location = 0) in vec3 position;
layout(push_constant) uniform Push{
	vec2 offset;
	vec3 secondOffset;
} push;
void main() {
  gl_Position = vec4(position, 1.0);
  gl_Position.x+=push.offset.x;
  gl_Position.y+=push.secondOffset.y;
}