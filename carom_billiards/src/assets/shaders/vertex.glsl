#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // 노말이 1번이라고 가정

out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace; // [핵심] 빛의 관점에서 본 좌표

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix; // [핵심] C++에서 넘겨준 빛 공간 행렬

void main()
{
    // 1. 월드 공간 위치 계산
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // 2. 노말 계산 (비균등 스케일링 대응을 위한 역전치 행렬 사용)
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // 3. 현재 정점을 '빛의 공간'으로 변환해서 프래그먼트 쉐이더로 전달
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    
    // 4. 최종 화면 위치 계산
    gl_Position = projection * view * vec4(FragPos, 1.0);
}