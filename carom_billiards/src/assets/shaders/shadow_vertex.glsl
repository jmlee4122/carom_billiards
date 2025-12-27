#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    // 빛의 관점에서 본 위치로 변환
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}