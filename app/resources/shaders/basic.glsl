//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

uniform vec3 viewPos;

uniform vec3 dirLightDirection;
uniform vec3 dirLightAmbient;
uniform vec3 dirLightDiffuse;
uniform vec3 dirLightSpecular;

uniform float shininess;

uniform vec3 spotLightPosition;
uniform vec3 spotLightDirection;
uniform vec3 spotLightAmbient;
uniform vec3 spotLightDiffuse;
uniform vec3 spotLightSpecular;

uniform float spotLightCutOff;
uniform float spotLightOuterCutOff;
uniform float spotLightC;
uniform float spotLightL;
uniform float spotLightQ;


vec3 NadjiSpotLight(vec3 norm, vec3 viewDir, vec3 baseColor) {
    vec3 lightDir = normalize(spotLightPosition - FragPos);
    float t = dot(lightDir, normalize(-spotLightDirection));
    float gama = spotLightCutOff - spotLightOuterCutOff;
    float intensity = clamp((t - spotLightOuterCutOff) / gama, 0.0, 1.0);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    float distance = length(spotLightPosition - FragPos);
    float attenuation = 1.0 / (spotLightC + spotLightL * distance + spotLightQ * distance * distance);

    vec3 ambient = spotLightAmbient * baseColor;
    vec3 diffuse = spotLightDiffuse * diff * baseColor;
    vec3 specular = spotLightSpecular * spec;

    return (ambient + diffuse + specular) * attenuation * intensity;
}

void main() {
    vec3 baseColor = texture(texture_diffuse1, TexCoords).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dirLightDirection);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    vec3 ambient = dirLightAmbient * baseColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dirLightDiffuse * diff * baseColor;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = dirLightSpecular * spec;

    vec3 rez = ambient + diffuse + specular;
    rez += NadjiSpotLight(norm,viewDir,baseColor);

    FragColor = vec4(rez, 1.0);
}
