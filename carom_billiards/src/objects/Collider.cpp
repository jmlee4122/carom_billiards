#include <iostream>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "../MyStruct.h"

#include "Collider.h"

Collider::Collider(Model* model) {
	this->max_x = model->vertices[0].x;
	this->max_z = model->vertices[0].z;

	this->min_x = model->vertices[0].x;
	this->min_y = model->vertices[0].y;
	this->min_z = model->vertices[0].z;

	if (model != nullptr) {
		for (int i = 0; i < model->vertex_count; i++) {
			glm::vec3 target = glm::vec3(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
			this->max_x = std::max(max_x, target.x);
			this->max_z = std::max(max_z, target.z);

			this->min_x = std::min(min_x, target.x);
			this->min_y = std::min(min_y, target.y);
			this->min_z = std::min(min_z, target.z);
		}
	}
}

Collider::~Collider() {

}

void Collider::PrintMaxAndMin() const {
	std::cout << "====== Collider Test ======" << std::endl;
	std::cout << "max x : " << this->max_x << " / min x : " << this->min_x << std::endl;
	std::cout << "min y : " << this->min_y << std::endl;
	std::cout << "max z : " << this->max_z << " / min z : " << this->min_z << std::endl;
}