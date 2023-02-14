#version 450


layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 textureCoordinates;


layout(location = 0)out vec3 fragmentPosition;
layout(location = 1) out vec3 Normals;
layout(location = 2) out vec2 UvsCoords;
layout(location = 3) out vec3 cameraPosition;
layout(push_constant) uniform WorldTransformData{
    mat4 viewMatrix;
    mat4 worldMatrix;
    vec3 cameraPosition;
} worldTransformData;

vec2 fixVectorPositioning(vec2 base){
    base.y*=-1;
    return base;
}

vec3 fixVectorPositioning(vec3 base){
    base.y*=-1;
    return base;
}

vec4 fixVectorPositioning(vec4 base){
    base.y*=-1;
    return base;
}

void main() {
    UvsCoords = textureCoordinates;
    Normals = (vec4(normals, 0.0f)*worldTransformData.worldMatrix).xyz;
    fragmentPosition = vec3(worldTransformData.worldMatrix*vec4(position, 1.0));
    vec4 temp = boneIds*weights;
    cameraPosition = worldTransformData.cameraPosition;
    gl_Position = fixVectorPositioning(worldTransformData.viewMatrix*worldTransformData.worldMatrix*vec4(position, 1.0));
}
