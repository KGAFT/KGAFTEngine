#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec3 normals;
layout(location = 1) out vec2 UVS;



layout(push_constant) uniform Push{
	mat4 cameraMatrix;
	mat4 worldMatrix;
} push;



vec2 fixVectorPositioning(vec2 base){
	base.y*=-1;
	return base;
}

vec3 fixVectorPositioning(vec3 base){
	base.y*=-1;
	return base;
}

void main() {
	normals = fixVectorPositioning(normal);
	UVS = fixVectorPositioning(uv);
	gl_Position = push.cameraMatrix*push.worldMatrix*vec4(fixVectorPositioning(position), 1.0);
}