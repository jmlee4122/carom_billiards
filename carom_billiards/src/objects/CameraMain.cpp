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
	this->eye = glm::vec3(0, 0, 0);
	this->at = glm::vec3(0, 106, 0);
	this->dis = 300.0f * pixel_per_cm;
	this->height = 50.0f * pixel_per_cm;
	this->yaw = 0.0f;
	this->pitch = 45.0f;
	this->speed = 5.0f;
	this->viewMat = glm::mat4(1.0);
}

CameraMain::~CameraMain() {

}

void CameraMain::SetYaw(float a) {

}

void CameraMain::SetAt() {
	this->at = GetCueBallPos();
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
	UpdateEye();
	UpdateMat();
}

glm::mat4 CameraMain::GetViewMat() const {
	return this->viewMat;
}