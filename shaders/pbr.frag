#version 450

#define PI 3.1415926538

in vec3 Position;
in vec3 WorldNormal;
in vec2 TextureCoords;

layout(binding = 0) uniform sampler2D diffuseTexture;
layout(binding = 1) uniform sampler2D metallicTexture;
layout(binding = 2) uniform sampler2D roughnessTexture;

layout (std140) uniform SceneUniformBlock {
    float time;
    float deltaTime;
    float padding;
    float morePadding;

    vec4 cameraPosition;
    mat4 cameraView;
    mat4 cameraProjection;
    vec4 sceneLightDirection;
    mat4 sceneLightProjection;
};

uniform mat4 model;
uniform float roughness;
uniform float light_intensity;
uniform bool is_metal;
uniform vec3 color;

out vec4 FragColor;


vec3 schlickFresnel(float vDotH)
{
    vec3 F0 = vec3(0.04);

    if (is_metal) {
        F0 = color;
    }

    vec3 ret = F0 + (1 - F0) * pow(clamp(1.0 - vDotH, 0.0, 1.0), 5);

    return ret;
}

float geomSmith(float dp, float rough)
{
    float k = (rough + 1.0) * (rough + 1.0) / 8.0;
    float denom = dp * (1 - k) + k;
    return dp / denom;
}

float ggxDistribution(float nDotH, float rough)
{
    float alpha2 = rough * rough * rough * rough;
    float d = nDotH * nDotH * (alpha2 - 1) + 1;
    float ggxdistrib = alpha2 / (PI * d * d);
    return ggxdistrib;
}

void main() {

    vec3 l = -sceneLightDirection.xyz;
    vec3 n = normalize(WorldNormal);

    vec3 v = normalize(cameraPosition.xyz - Position.xyz);
    vec3 h = normalize(v + l);

    float nDotH = max(dot(n, h), 0.0);
    float vDotH = max(dot(v, h), 0.0);
    float nDotL = max(dot(n, l), 0.0);
    float nDotV = max(dot(n, v), 0.0);

    vec3 F = schlickFresnel(vDotH);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;

    float RoughTexture = texture(roughnessTexture, TextureCoords).r;
    float RoughnessTxt = min(roughness + RoughTexture, 1.0);
    vec3 SpecBRDF_nom  = ggxDistribution(nDotH, RoughnessTxt) * F * geomSmith(nDotL, RoughnessTxt) * geomSmith(nDotV, RoughnessTxt);

    float SpecBRDF_denom = 4.0 * nDotV * nDotL + 0.0001;

    vec3 SpecBRDF = SpecBRDF_nom / SpecBRDF_denom;

    vec3 fLambert = vec3(0.0);

    if (!is_metal) {
        fLambert = color;
    }

    vec3 DiffTexture = texture(diffuseTexture, TextureCoords).rgb;
    vec3 DiffuseBRDF = kD * DiffTexture * (fLambert / PI) ;

    vec3 FinalColor = (DiffuseBRDF + SpecBRDF) * light_intensity * nDotL;

    FragColor = vec4(FinalColor, 1.0);
}
