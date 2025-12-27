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
	if (model != nullptr) {
		float max_x = model->vertices[0].x;
		float max_z = model->vertices[0].z;

		for (int i = 0; i < model->vertex_count; i++) {

		}
	}
}

Collider::~Collider() {

}