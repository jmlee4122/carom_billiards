#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

class Circle
{
public:
    Circle(float scale);
    ~Circle();
    void DrawCircle();
    void SetColor(float scale);
    void SetPosition();
    void SetRadius(float scale);

private:
    GLuint VAO, VBO_pos, EBO;
    glm::vec3 color;
    glm::vec2 position;
    float radius;
};

