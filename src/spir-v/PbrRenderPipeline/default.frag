#version 450

#define LIGHT_BLOCKS_AMOUNT 100
#define PI 3.14159265359



layout(location = 0) in vec3 fragmentPosition;
layout(location = 1) in vec3 Normals;
layout(location = 2) in vec2 UvsCoords;
layout(location = 3) in vec3 cameraPosition;
layout (location = 0) out vec4 FragColor;


struct PointLight{
    vec3 position;
    vec3 color;
    float intensity;
    float add;
};

struct DirectLight{
    vec3 direction;
    vec3 color;
    float intensity;
    float add;
};

layout(set = 0, binding = 1) uniform sampler2D albedoMap;
layout(set = 0, binding = 2) uniform sampler2D normalMap;
layout(set = 0, binding = 3) uniform sampler2D metallicMap;
layout(set = 0, binding = 4) uniform sampler2D roughnessMap;
layout(set = 0, binding = 5) uniform sampler2D aoMap;
layout(set = 0, binding = 6) uniform sampler2D emissiveMap;


layout(binding = 0) uniform LightUbo {
    DirectLight directLights[LIGHT_BLOCKS_AMOUNT];
    PointLight pointLights[LIGHT_BLOCKS_AMOUNT];

    vec4 enabledDirects;
    vec4 enabledPoints;


    float emissiveIntensity;
    float emissiveShininess;
    float gammaCorrect;
    float ambientIntensity;
} lightUbo;

vec3 getNormalFromMap(vec2 uvsCoords, vec3 normals, vec3 fragmentPosition)
{
    vec4 baseNormal = texture(normalMap, uvsCoords);
    vec3 tangentNormal = baseNormal.xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fragmentPosition);
    vec3 Q2  = dFdy(fragmentPosition);
    vec2 st1 = dFdx(uvsCoords);
    vec2 st2 = dFdy(uvsCoords);
    vec3 N   = normalize(normals);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    return normalize(TBN * tangentNormal);
}

float distributeGGX(vec3 normals, vec3 halfWayVector, float roughness)
{
    float roughness4 = roughness*roughness*roughness*roughness;
    float halwayDot2 = max(dot(normals, halfWayVector), 0.0)*max(dot(normals, halfWayVector), 0.0);
    float numerator   = roughness4;
    float denominator = (halwayDot2 * (roughness4 - 1.0) + 1.0);
    denominator = PI * denominator * denominator;
    return numerator / denominator;
}

float schlickGeometryGGX(float dotWorldViewVector, float roughness)
{
    float roughnessKoef = ((roughness + 1.0)*(roughness + 1.0)) / 8.0;
    float numerator   = dotWorldViewVector;
    float denominator = dotWorldViewVector * (1.0 - roughnessKoef) + roughnessKoef;
    return numerator / denominator;
}

float smithGeometry(vec3 processedNormals, vec3 worldViewVector, vec3 lightPosition, float roughness)
{
    float worldViewVectorDot = max(dot(processedNormals, worldViewVector), 0.0);
    float lightDot = max(dot(processedNormals, lightPosition), 0.0);
    float ggx2 = schlickGeometryGGX(worldViewVectorDot, roughness);
    float ggx1 = schlickGeometryGGX(lightDot, roughness);
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 startFresnelSchlick)
{
    return startFresnelSchlick + (1.0 - startFresnelSchlick) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 processPointLight(PointLight light, vec3 normals, vec3 worldViewVector, vec3 startFresnelSchlick, float roughness, float metallic, vec3 albedo){
    vec3 processedLightPos = normalize(light.position - fragmentPosition);
    vec3 halfWay = normalize(worldViewVector + processedLightPos);
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light.color * light.intensity * attenuation;
    float halfWayGGX = distributeGGX(normals, halfWay, roughness);
    float geometrySmith   = smithGeometry(normals, worldViewVector, processedLightPos, roughness);
    vec3 fresnelSchlick    = fresnelSchlick(clamp(dot(halfWay, worldViewVector), 0.0, 1.0), startFresnelSchlick);
    vec3 numerator    = halfWayGGX * geometrySmith * fresnelSchlick;
    float denominator = 4.0 * max(dot(normals, worldViewVector), 0.0) * max(dot(normals, processedLightPos), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;
    vec3 finalFresnelSchlick = vec3(1.0) - fresnelSchlick;
    finalFresnelSchlick *= 1.0 - metallic;
    float NdotL = max(dot(normals, processedLightPos), 0.0);
    return (finalFresnelSchlick * albedo / PI + specular) * radiance * NdotL;
}

vec3 processDirectionaLight(DirectLight light, vec3 normals, vec3 worldViewVector, vec3 startFresnelSchlick, float roughness, float metallic, vec3 albedo){
    vec3 processedLightPos = normalize(-light.direction);
    vec3 halfWay = normalize(worldViewVector + processedLightPos);
    vec3 radiance = light.color * light.intensity;
    float halfWayGGX = distributeGGX(normals, halfWay, roughness);
    float geometrySmith   = smithGeometry(normals, worldViewVector, processedLightPos, roughness);
    vec3 fresnelSchlick    = fresnelSchlick(clamp(dot(halfWay, worldViewVector), 0.0, 1.0), startFresnelSchlick);
    vec3 numerator    = halfWayGGX * geometrySmith * fresnelSchlick;
    float denominator = 4.0 * max(dot(normals, worldViewVector), 0.0) * max(dot(normals, processedLightPos), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;
    vec3 finalFresnelSchlick = vec3(1.0) - fresnelSchlick;
    finalFresnelSchlick *= 1.0 - metallic;
    float NdotL = max(dot(normals, processedLightPos), 0.0);
    return (finalFresnelSchlick * albedo / PI + specular) * radiance * NdotL;
}

vec3 postProcessColor(vec3 color){
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(lightUbo.gammaCorrect));
    return color;
}

void main() {
    vec3 albedo = pow(texture(albedoMap, UvsCoords).rgb, vec3(2.2));
    float metallic = texture(metallicMap, UvsCoords).r;
    float roughness = texture(roughnessMap, UvsCoords).r;
    float ao = texture(aoMap, UvsCoords).r;
    vec4 emissive = texture(emissiveMap, UvsCoords);
    vec3 processedNormals = normalize(getNormalFromMap(UvsCoords, Normals, fragmentPosition));
    vec3 worldViewVector = normalize(cameraPosition - fragmentPosition);


    vec3 startFresnelSchlick = vec3(0.04);
    startFresnelSchlick = mix(startFresnelSchlick, albedo, metallic);

    vec3 Lo = vec3(0.0);

    for(int c = 0; c<lightUbo.enabledDirects.x; c++){
        Lo+=processDirectionaLight(lightUbo.directLights[c], processedNormals, worldViewVector, startFresnelSchlick, roughness, metallic, albedo);
    }
    for(int c = 0; c<lightUbo.enabledPoints.x; c++){
        Lo+=processPointLight(lightUbo.pointLights[c], processedNormals, worldViewVector, startFresnelSchlick, roughness, metallic, albedo);
    }


    vec3 ambient = vec3(lightUbo.ambientIntensity) * albedo * ao;

    vec3 color = ambient + Lo;

    color+=(emissive*pow(emissive.a, lightUbo.emissiveShininess)*lightUbo.emissiveIntensity).rgb;
    color = postProcessColor(color);
    FragColor = vec4(color, 1.0);


}
