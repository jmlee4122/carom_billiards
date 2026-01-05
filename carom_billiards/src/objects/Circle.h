#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

class Circle
{
public:
    Circle(float scale, glm::vec3 color);
    ~Circle();
    void DrawCircle(float x, float y);
    void SetColor();
    glm::vec2 GetPosition() const;
    void SetRadius(float scale);
    void UpdateColor();
    void UpdatePosition();
    void Update();

    void SetIsLeft(bool a);
    void SetIsRight(bool a);
    void SetIsUp(bool a);
    void SetIsDown(bool a);

private:
    GLuint VAO, VBO_pos, EBO;
    glm::vec3 color;
    glm::vec2 position;
    float radius;
    bool isLeft, isRight, isUp, isDown;
    float speed;
};

