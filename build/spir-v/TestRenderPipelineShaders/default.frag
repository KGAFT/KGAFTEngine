#version 450

layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 uv;

layout(set = 0, binding = 1) uniform sampler2D baseColorSampler;

layout (location = 0) out vec4 outColor;

layout(binding = 0) uniform UniformBufferObject {
    vec2 firstAdder;
    vec3 secondAdder;
    vec4 firstAmplifier;
    float secondAmplifier;
} ubo;



void main() {
    outColor = texture(baseColorSampler, uv);
}