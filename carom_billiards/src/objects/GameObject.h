#pragma once

class GameObject {
public:
    virtual ~GameObject() = default;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
};