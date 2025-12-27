#include <iostream>
#include <vector>
#include <string>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "Ball.h"

#include "../MyExtern.h"
#include "../MyStruct.h"
#include "../MyUtils.h"

Ball::Ball(Model* model) : VAO(0), VBO_pos(0), VBO_nol(0), EBO(0) {
	this->model = model;
	this->vCount = model->vertex_count, this->fCount = model->face_count;
	this->uColor = glm::vec3(0, 0, 0), this->uAlpha = 1.0f;
	this->position = glm::vec3(0.0f, 120.0f, 0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(1.0f);
	this->velocity = glm::vec3(0.0f);
	this->modelMat = glm::mat4(1.0f);

	std::cout << "[Ball] Vertex Count: " << vCount << ", Face Count: " << fCount << std::endl;

	if (model->normals == nullptr) {
		std::cerr << "ERROR: Model normals are not loaded!" << std::endl;
		return;
	}

	InitBuffers(VAO, VBO_pos, VBO_nol, EBO, model->vertices, model->normals,
		model->faces, vCount, fCount, model->normal_count);

	std::cout << "[Ball] VAO: " << VAO << ", VBO_pos: " << VBO_pos << ", EBO: " << EBO << std::endl;

	SetColor();
}

Ball::~Ball() {

}

void Ball::SetColor() {
	this->uColor = glm::vec3(1.0f, 0.0f, 1.0f);
}

void Ball::SetPosition(float dt) {

}
void Ball::SetRotation(float dt) {

}
void Ball::SetScale() {

}
void Ball::SetVelocity(float dt) {

}
void Ball::SetModelMat(float dt) {
	glm::mat4 transMat = glm::translate(glm::mat4(1.0), this->position);

	// ### Test ###
	this->modelMat = transMat;
	// ### Test ###
}

void Ball::Update(float dt) {
	SetPosition(dt);
	SetRotation(dt);
	SetScale();
	SetVelocity(dt);
	SetModelMat(dt);
}

void Ball::Render(GLuint shaderID) {
	static bool firstRender = true;
	if (firstRender) {
		std::cout << "[Ball::Render] ShaderID: " << shaderID
			<< ", Face Count: " << fCount
			<< ", Triangle Count: " << (fCount * 3) << std::endl;
		firstRender = false;
	}

	glBindVertexArray(VAO);


	// 각 쉐이더에 맞는 uniform location을 동적으로 가져옴

	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	if (modelLoc != -1) {
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->modelMat));
	}
	else {
		std::cout << "[Ball::Render] WARNING: 'model' uniform not found in shader " << shaderID << std::endl;
	}

	// Pass 2 (일반 렌더링)일 때만 색상 전송
	GLint colorLoc = glGetUniformLocation(shaderID, "objectColor");
	if (colorLoc != -1) {
		glUniform3fv(colorLoc, 1, glm::value_ptr(uColor));
	}

	GLint alphaLoc = glGetUniformLocation(shaderID, "alpha");
	if (alphaLoc != -1) {
		glUniform1f(alphaLoc, uAlpha);
	}

	glDrawElements(GL_TRIANGLES, fCount * 3, GL_UNSIGNED_INT, (void*)(0));

	// OpenGL 에러 체크
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "[Ball::Render] OpenGL Error: " << err << std::endl;
	}

	glBindVertexArray(0);
}