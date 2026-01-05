#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "Circle.h"

#include "../MyExtern.h"
#include "../MyStruct.h"
#include "../MyUtils.h"

Circle::Circle(float scale, glm::vec3 color) : VAO(0), VBO_pos(0), EBO(0) {
	this->color = color;
	this->position = glm::vec2(0, 0);
	this->radius = 0.0f;
	this->isLeft = false, this->isRight = false, this->isUp = false, this->isDown = false;
	this->speed = 0.1f;
	
	SetRadius(scale);
}

Circle::~Circle() {

}

void Circle::DrawCircle(float x, float y) {
	// 셰이더 비활성화
	glUseProgram(0);

	// 렌더링 설정
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	// 원 색상 설정
	glColor3f(this->color.r, this->color.g, this->color.b);

	// 원 그리기 (삼각형 팬 방식)
	const int segments = 100; // 원의 부드러움 정도
	const float PI = 3.14159265f;
	const float angleStep = 2.0f * PI / segments;

	glBegin(GL_TRIANGLE_FAN);

	// 중심점
	glVertex2f(x, y);

	// 원 둘레의 점들
	for (int i = 0; i <= segments; ++i) {
		float angle = i * angleStep;
		float dx = this->radius * cosf(angle);
		float dy = this->radius * sinf(angle);
		glVertex2f(x + dx, y + dy);
	}

	glEnd();

	// 원래 상태로 복원
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

glm::vec2 Circle::GetPosition() const {
	return this->position;
}

void Circle::SetRadius(float scale) {
	this->radius = 80.0f * scale;
}

void Circle::UpdateColor() {
	if (gScore.cueBallColor == "white") {
		this->color = glm::vec3(1, 1, 1);
	}
	else if (gScore.cueBallColor == "yellow") {
		this->color = glm::vec3(1, 1, 0);
	}
}

void Circle::SetIsLeft(bool a) {
	this->isLeft = a;
}

void Circle::SetIsRight(bool a) {
	this->isRight = a;
}

void Circle::SetIsUp(bool a) {
	this->isUp = a;
}

void Circle::SetIsDown(bool a) {
	this->isDown = a;
}

void Circle::UpdatePosition() {
	if (isLeft && !isRight) {
		this->position.x -= this->speed;
	}
	else if (!isLeft && isRight) {
		this->position.x += this->speed;
	}

	if (isLeft && !isRight) {
		this->position.y -= this->speed;
	}
	else if (!isLeft && isRight) {
		this->position.y += this->speed;
	}
}

void Circle::Update() {
	if (this->radius == 80.0f) {
		UpdateColor();
	}
	else {
		UpdatePosition();
	}
}