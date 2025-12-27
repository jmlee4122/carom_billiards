#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

#include "GameObject.h"

struct Model;

class Table : public GameObject
{
public:
    Table(Model* model);
    ~Table() override;
    void SetColor();
    void Update(float dt) override;
    void Render(GLuint shaderID) override;

private:
    Model* model;
    GLuint VAO, VBO_pos, VBO_nol, EBO;
    GLuint vCount, fCount;
    glm::vec3 uColor;
    float uAlpha;
};

