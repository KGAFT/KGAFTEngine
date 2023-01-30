#version 450

layout(location = 0) in vec3 position;
layout(push_constant) uniform Push{
	vec2 offset;
	vec3 secondOffset;
} push;

layout(binding = 0) uniform UniformBufferObject {
    vec2 offset;
} ubo;


void main() {
  gl_Position = vec4(position, 1.0);
  gl_Position.x+=ubo.offset.x;
  gl_Position.y+=ubo.offset.y;
}