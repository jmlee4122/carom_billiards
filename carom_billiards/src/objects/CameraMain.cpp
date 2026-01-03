#include <iostream>
#include <vector>
#include <string>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "CameraMain.h"

#include "../MyExtern.h"
#include "../MyStruct.h"
#include "../MyUtils.h"

CameraMain::CameraMain() {
	this->eye = glm::vec3(0, 0, 0), this->at = glm::vec3(0, 106, 0);
	this->dis = 300.0f * pixel_per_cm, this->height = 50.0f * pixel_per_cm;
	this->yaw = 0.0f, this->pitch = 45.0f;
	this->speed = 0.5f;
	this->viewMat = glm::mat4(1.0);
	this->isLeft = false, this->isRight = false;
}

CameraMain::~CameraMain() {

}

void CameraMain::SetAt() {
	this->at = GetCueBallPos();
}

void CameraMain::SetIsLeft(bool a) {
	this->isLeft = a;
}

void CameraMain::SetIsRight(bool a) {
	this->isRight = a;
}

void CameraMain::UpdateYaw() {
	if (isLeft && !isRight) {
		this->yaw -= this->speed;
	}
	else if (!isLeft && isRight) {
		this->yaw += this->speed;
	}
}

void CameraMain::UpdateEye() {
	this->eye.x = this->at.x + this->dis * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	this->eye.y = this->at.y + this->height * sin(glm::radians(pitch));
	this->eye.z = this->at.z + this->dis * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
}

void CameraMain::UpdateMat() {
	this->viewMat = glm::lookAt(this->eye, this->at, glm::vec3(0, 1, 0));
}

void CameraMain::Update() {
	UpdateYaw();
	UpdateEye();
	UpdateMat();
}

glm::mat4 CameraMain::GetViewMat() const {
	return this->viewMat;
}

glm::vec3 CameraMain::GetEye() const {
	return this->eye;
}