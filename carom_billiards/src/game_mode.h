#pragma once

#include <iostream>
#include <vector>
#include <GL/glut.h>

class GameMode
{
public:
    virtual ~GameMode() {}

    virtual void Init() = 0;
    virtual void Cleanup() = 0;
    virtual void HandleEvents(unsigned char key, int x, int y) = 0;
    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;
};