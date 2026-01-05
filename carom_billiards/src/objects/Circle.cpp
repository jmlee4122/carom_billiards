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
	this->speed = 0.5f;
	
	SetRadius(scale);
}

Circle::~Circle() {

}

void Circle::DrawCircle(float x, float y) {
	glUseProgram(0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor3f(this->color.r, this->color.g, this->color.b);

	const int segments = 100;
	const float PI = 3.14159265f;
	const float angleStep = 2.0f * PI / segments;

	glBegin(GL_TRIANGLE_FAN);

	glVertex2f(x, y);

	for (int i = 0; i <= segments; ++i) {
		float angle = i * angleStep;
		float dx = this->radius * cosf(angle);
		float dy = this->radius * sinf(angle);
		glVertex2f(x + dx, y + dy);
	}

	glEnd();

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
		float minX = -sqrt(80.0f * 80.0f - position.y * position.y);
		float maxX = sqrt(80.0f * 80.0f - position.y * position.y);
		this->position.x = glm::clamp(this->position.x , minX + this->radius, maxX - this->radius);
	}
	else if (!isLeft && isRight) {
		this->position.x += this->speed;
		float minX = -sqrt(80.0f * 80.0f - position.y * position.y);
		float maxX = sqrt(80.0f * 80.0f - position.y * position.y);
		this->position.x = glm::clamp(this->position.x, minX + this->radius, maxX - this->radius);
	}

	if (isUp && !isDown) {
		this->position.y += this->speed;
		float minY = -sqrt(80.0f * 80.0f - position.x * position.x);
		float maxY = sqrt(80.0f * 80.0f - position.x * position.x);
		this->position.y = glm::clamp(this->position.y, minY + this->radius, maxY - this->radius);
	}
	else if (!isUp && isDown) {
		this->position.y -= this->speed;
		float minY = -sqrt(80.0f * 80.0f - position.x * position.x);
		float maxY = sqrt(80.0f * 80.0f - position.x * position.x);
		this->position.y = glm::clamp(this->position.y, minY + this->radius, maxY - this->radius);
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