#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

#include "GameObject.h"

struct Model;

class Ball : public GameObject
{
public:
    Ball(Model* model);
    ~Ball() override;
    void SetColor();
    void Update(float dt) override;
    void Render(GLuint shaderID) override;

    void SetPosition(float dt);
    void SetRotation(float dt);
    void SetScale();
    void SetVelocity(float dt);
    void SetModelMat(float dt);
    std::string GetObjectName() override;

private:
    Model* model;
    GLuint VAO, VBO_pos, VBO_nol, EBO;
    GLuint vCount, fCount;
    glm::vec3 uColor;
    float uAlpha;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 velocity;
    glm::mat4 modelMat;

    std::string objectName;
};

