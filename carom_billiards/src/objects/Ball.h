#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

#include "GameObject.h"

struct Model;

class Ball : public GameObject
{
public:
    Ball(Model* model, glm::vec3 pos, glm::vec3 v);
    ~Ball() override;
    void SetColor();
    void Update(float dt) override;
    void Render(GLuint shaderID) override;
    void HandleCollision(std::string name, GameObject* other) override;

    void UpdatePosition(float dt);
    void UpdateRotation(float dt);
    void SetScale();
    void SetVelocity(glm::vec3 v);
    void UpdateVelocity(float dt);
    void UpdateModelMat(float dt);
    std::string GetObjectName() override;

    glm::vec3 GetPosition() const;
    glm::vec3 GetPrevPosition() const;
    float GetRadius() const;
    void SetIsWallCollision(bool a);
    bool GetIsWallCollision();

private:
    Model* model;
    GLuint VAO, VBO_pos, VBO_nol, EBO;
    GLuint vCount, fCount;
    glm::vec3 uColor;
    float uAlpha;
    float radius;

    glm::vec3 position;
    glm::vec3 prevPosition;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 velocity;
    glm::mat4 modelMat;

    std::string objectName;
    bool isWallCollision;
};

