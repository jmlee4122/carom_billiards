#version 330 core

#define NR_LIGHTS 5

in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lights[NR_LIGHTS];
uniform int lightCount; 

uniform vec3 viewPos;
uniform vec3 objectColor;
uniform float alpha;

uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    if(projCoords.z > 1.0)
        return 0.0;

    float currentDepth = projCoords.z;
    
    // 바이어스 조정
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.001);

    // PCF 샘플링 범위 확대 (더 부드러운 그림자)
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 25.0; // 5x5 샘플링
    
    return shadow;
}

vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow)
{
    // Ambient 증가
    vec3 ambientLight = vec3(0.2); 
    vec3 ambient = ambientLight * light.color;

    // Diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseLight = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseLight * light.color;

    // Specular 감소 (당구대는 반짝이지 않음)
    int shininess = 16; // 32 -> 16
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularLight = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularLight * light.color * 0.2; // 0.5 -> 0.2

    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    if (dot(norm, viewDir) < 0.0) {
        norm = -norm; 
    }

    vec3 result = vec3(0.0);

    for(int i = 0; i < lightCount; i++)
    {
        if(i >= NR_LIGHTS) break; 
        
        float shadow = 0.0;
        
        if(i == 0) 
        {
            vec3 lightDir = normalize(lights[i].position - FragPos);
            shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);
        }

        result += CalculateLight(lights[i], norm, FragPos, viewDir, shadow);
    }

    // 감마 보정 (더 자연스러운 색상)
    result = pow(result, vec3(1.0/2.2));

    FragColor = vec4(result * objectColor, alpha);
}