#pragma once

#include <string>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/quaternion.hpp>

#include "GameObject.h"

struct Model;

class Ball : public GameObject
{
public:
    Ball(Model* model, glm::vec3 pos, glm::vec3 v, std::string color);
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
    std::string GetColor();
    glm::vec3 GetPosition() const;
    glm::vec3 GetPrevPosition() const;
    glm::vec3 GetVelocity() const;
    float GetRadius() const;

    void SetIsWallCollision(bool a);
    bool GetIsWallCollision() const;

    void SetHasCollided(bool a);

    void SetIsCueBall(bool a);
    bool GetIsCueBall() const;

    void ApplyImpulse();
    void ClearImpulse();

    void SetAngularVelocity(glm::vec3 angVel);
    glm::vec3 GetAngularVelocity() const;

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
    glm::vec3 angularVelocity;
    glm::vec3 pendingImpulse;
    glm::mat4 modelMat;
    glm::quat rotationQuat;

    std::string objectName;
    std::string color;

    bool isWallCollision; // ball:wall
    bool hasCollided;     // ball:ball
    bool isCueBall;
};

