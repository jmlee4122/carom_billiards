#include <iostream>
#include <vector>
#include <string>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "Table.h"

#include "../MyExtern.h"
#include "../MyStruct.h"
#include "../MyUtils.h"

Table::Table(Model* model) : VAO(0), VBO_pos(0), VBO_nol(0), EBO(0) {
	this->model = model;
	this->vCount = model->vertex_count, this->fCount = model->face_count;
	this->uColor = glm::vec3(0, 0, 0), this->uAlpha = 1.0f;
	this->objectName = "table";

	std::cout << "[Table] Vertex Count: " << vCount << ", Face Count: " << fCount << std::endl;

	if (model->normals == nullptr) {
		std::cerr << "ERROR: Model normals are not loaded!" << std::endl;
		return;
	}

	InitBuffers(VAO, VBO_pos, VBO_nol, EBO, model->vertices, model->normals,
		model->faces, vCount, fCount, model->normal_count);

	std::cout << "[Table] VAO: " << VAO << ", VBO_pos: " << VBO_pos << ", EBO: " << EBO << std::endl;

	SetColor();
}

Table::~Table() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_pos);
	glDeleteBuffers(1, &VBO_nol);
	glDeleteBuffers(1, &EBO);
	if (this->model != nullptr) {
		if (this->model->vertices) free(this->model->vertices);
		if (this->model->normals) free(this->model->normals);
		if (this->model->faces) free(this->model->faces);
		delete this->model;
		this->model = nullptr;
	}
}

void Table::SetColor() {
	this->uColor = glm::vec3(0, 0, 1);
	this->uAlpha = 1.0f;
	std::cout << "[Table] Color: (" << uColor.r << ", " << uColor.g << ", " << uColor.b << ")" << std::endl;
}

void Table::Update(float dt) {

}

void Table::Render(GLuint shaderID) {
	static bool firstRender = true;
	if (firstRender) {
		std::cout << "[Table::Render] ShaderID: " << shaderID 
		          << ", Face Count: " << fCount 
		          << ", Triangle Count: " << (fCount * 3) << std::endl;
		firstRender = false;
	}

	glBindVertexArray(VAO);

	glm::mat4 modelMat = glm::mat4(1.0f);

	// 각 쉐이더에 맞는 uniform location을 동적으로 가져옴
	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	if (modelLoc != -1) {
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
	}
	else {
		std::cout << "[Table::Render] WARNING: 'model' uniform not found in shader " << shaderID << std::endl;
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
		std::cerr << "[Table::Render] OpenGL Error: " << err << std::endl;
	}
	
	glBindVertexArray(0);
}

std::string Table::GetObjectName() {
	return this->objectName;
}

void Table::HandleCollision(std::string name, GameObject* other) {

}