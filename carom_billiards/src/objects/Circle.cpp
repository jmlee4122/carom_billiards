#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "Circle.h"

Circle::Circle(float scale) : VAO(0), VBO_pos(0), EBO(0) {
	this->color = glm::vec3(0, 0, 0);
	this->position = glm::vec2(0, 0);
	this->radius = 0.0f;
}

Circle::~Circle() {

}

void Circle::DrawCircle() {

}

void Circle::SetColor(float scale) {
	if (scale == 1.0f) {

	}
	else {
		this->color = glm::vec3(1, 0, 0);
	}
}

void Circle::SetPosition() {

}

void Circle::SetRadius(float scale) {
	if (scale == 1.0f) {
		this->radius = 300.0f;
	}
	else {
		this->radius = 50.0f;
	}
}