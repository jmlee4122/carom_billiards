#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

#include "GameObject.h"

struct Model;

class Plane : public GameObject
{
public:
    Plane(Model* model);
    ~Plane() override;
    void SetColor();
    void Update(float dt) override;
    void Render() override;

private:
    Model* model;
    GLuint VAO, VBO_pos, VBO_nol, EBO;
    GLuint vCount, fCount;
    GLuint uModelLoc, uViewLoc, uProjLoc;
    GLuint uObjColorLoc, uViewPosLoc, uAlphaLoc;
    GLuint uLightsPosLoc[4], uLightsColorLoc[4];
    GLuint uLightsCountLoc;
    glm::vec3 uColor; float uAlpha;
};

