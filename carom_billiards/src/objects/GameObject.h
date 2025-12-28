#pragma once

#include <string>

class GameObject {
public:
    virtual ~GameObject() = default;
    virtual void Update(float dt) = 0;
    virtual void Render(GLuint shaderID) = 0;
    virtual std::string GetObjectName() = 0;
    virtual void HandleCollision(std::string name, GameObject* other) = 0;
};