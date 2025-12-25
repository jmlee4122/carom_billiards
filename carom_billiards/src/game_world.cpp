#include <iostream>
#include <vector>
#include <memory>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "MyExtern.h"
#include "MyUtils.h"
#include "MyStruct.h"
#include "MyCallback.h"
#include "game_world.h"

#include "./objects/GameObject.h"

void AddObject(std::unique_ptr<GameObject> o) {
	world.push_back(std::move(o));
}
void AddObjects() {

}
void RemoveObject() {

}

void Update() {

}
void Render() {

}
void Clear() {

}

void Collide() {

}
void AddCollisionPair() {

}
void HandleCollisions() {

}
void RemoveCollisionObject() {

}