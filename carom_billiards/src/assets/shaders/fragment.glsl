#version 330 core

// 최대 조명 개수 정의 (C++ 코드와 맞춰야 함)
#define NR_LIGHTS 4 

in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

// 조명 속성을 구조체로 정의
struct Light {
    vec3 position;
    vec3 color;
};

// 구조체 배열과 현재 활성화된 조명 개수
uniform Light lights[NR_LIGHTS];
uniform int lightCount; 

uniform vec3 viewPos;
uniform vec3 objectColor;
uniform float alpha;

// 조명 계산 함수 분리
vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // 1. Ambient (주변광)
    // 주의: 여러 조명이면 Ambient가 너무 밝아질 수 있으니 계수를 줄이거나 별도로 처리하는 것이 좋습니다.
    vec3 ambientLight = vec3(0.1); // 0.3 -> 0.1로 조정 (조명이 여러개이므로)
    vec3 ambient = ambientLight * light.color;

    // 2. Diffuse (확산광)
    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseLight = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseLight * light.color;

    // 3. Specular (반사광)
    int shininess = 32;
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularLight = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularLight * light.color * 0.5;

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 양면 조명 처리: 노말이 뒤집혀도 조명 적용 (루프 밖에서 한 번만 계산)
    if (dot(norm, viewDir) < 0.0) {
        norm = -norm; 
    }

    vec3 result = vec3(0.0);

    // 활성화된 조명 개수만큼 반복하며 빛의 영향을 더해줌(Accumulate)
    for(int i = 0; i < lightCount; i++)
    {
        // 범위를 벗어나는 인덱스 방지 (선택 사항)
        if(i >= NR_LIGHTS) break; 
        
        result += CalculateLight(lights[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result * objectColor, alpha);
}