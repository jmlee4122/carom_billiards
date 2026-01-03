#pragma once

#include <iostream>
#include <vector>
#include <gl/glew.h>

class GameMode
{
public:
    virtual ~GameMode() {}

    virtual void Init() = 0;
    virtual void Cleanup() = 0;
    virtual void HandleKey(unsigned char key, int x, int y) = 0;
    virtual void HandleSpecialKey(int key, int x, int y) = 0;
    virtual void HandleSpecialKeyUp(int key, int x, int y) = 0;
    virtual void Update(float dt) = 0;
    virtual void Draw(GLuint shaderID) = 0;
};