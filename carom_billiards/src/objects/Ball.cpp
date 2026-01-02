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
#include "Collider.h"

#include "../MyExtern.h"
#include "../MyStruct.h"
#include "../MyUtils.h"
#include "../game_world.h"

Ball::Ball(Model* model, glm::vec3 pos, glm::vec3 v, std::string color) : VAO(0), VBO_pos(0), VBO_nol(0), EBO(0) {
	this->model = model;
	this->vCount = model->vertex_count, this->fCount = model->face_count;
	this->uColor = glm::vec3(0, 0, 0), this->uAlpha = 1.0f;
	this->radius = 3.275f;
	this->position = pos;
	this->prevPosition = this->position;
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(1.0f);
	this->velocity = v;
	this->modelMat = glm::mat4(1.0f);
	this->objectName = "ball";
	this->color = color;
	this->isWallCollision = false;
	this->hasCollided = false;

	std::cout << "[Ball] Vertex Count: " << vCount << ", Face Count: " << fCount << std::endl;

	if (model->normals == nullptr) {
		std::cerr << "ERROR: Model normals are not loaded!" << std::endl;
		return;
	}

	InitBuffers(VAO, VBO_pos, VBO_nol, EBO, model->vertices, model->normals,
		model->faces, vCount, fCount, model->normal_count);

	std::cout << "[Ball] VAO: " << VAO << ", VBO_pos: " << VBO_pos << ", EBO: " << EBO << std::endl;

	SetColor();

	World::AddCollisionPair("ball:ball", this, this);
	World::AddCollisionPair("ball:wall", this, nullptr);
}

Ball::~Ball() {

}

void Ball::SetColor() {
	if (this->color == "red") {
		this->uColor = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (this->color == "white") {
		this->uColor = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	else if (this->color == "yellow") {
		this->uColor = glm::vec3(1.0f, 1.0f, 0.0f);
	}
	else {
		std::cerr << "ERROR: Invalid ball color '" << this->color << "'. Program will exit." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Ball::UpdatePosition(float dt) {
	this->prevPosition = this->position;
	this->position = this->position + (this->velocity * dt);
}

void Ball::UpdateRotation(float dt) {

}

void Ball::SetScale() {

}

void Ball::SetVelocity(glm::vec3 v) {
	this->velocity = v;
}

void Ball::UpdateVelocity(float dt) {
	float vSize = glm::length(this->velocity);
	if (vSize <= 0.1f) {
		this->velocity.x = 0.0f;
		this->velocity.z = 0.0f;
		return;
	}
	this->velocity.x *= 0.995f;
	this->velocity.z *= 0.995f;
}

void Ball::UpdateModelMat(float dt) {
	glm::mat4 transMat = glm::translate(glm::mat4(1.0), this->position);
	this->modelMat = transMat;
}

void Ball::Update(float dt) {
	UpdateVelocity(dt);
	UpdatePosition(dt);
	UpdateRotation(dt);
	SetScale();
	UpdateModelMat(dt);
}

void Ball::Render(GLuint shaderID) {
	glBindVertexArray(VAO);

	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	if (modelLoc != -1) {
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->modelMat));
	}

	GLint colorLoc = glGetUniformLocation(shaderID, "objectColor");
	if (colorLoc != -1) {
		glUniform3fv(colorLoc, 1, glm::value_ptr(uColor));
	}

	GLint alphaLoc = glGetUniformLocation(shaderID, "alpha");
	if (alphaLoc != -1) {
		glUniform1f(alphaLoc, uAlpha);
	}

	glDrawElements(GL_TRIANGLES, fCount * 3, GL_UNSIGNED_INT, (void*)(0));

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "[Ball::Render] OpenGL Error: " << err << std::endl;
	}

	glBindVertexArray(0);
}

std::string Ball::GetObjectName() {
	return this->objectName;
}

void Ball::HandleCollision(std::string name, GameObject* other) {
	if (name == "ball:ball") {
		Ball* ball = static_cast<Ball*>(other);
		
		glm::vec3 delta = this->position - ball->GetPosition();
		float distance = glm::length(delta);
		
		if (distance < 0.001f || distance >= (this->radius + ball->GetRadius())) {
			return;
		}
		
		glm::vec3 normal = glm::normalize(delta);
		
		float overlap = (this->radius + ball->GetRadius()) - distance;
		this->position += normal * (overlap * 0.5f);
		ball->position -= normal * (overlap * 0.5f);
		
		glm::vec3 relativeVelocity = this->velocity - ball->velocity;
		float velocityAlongNormal = glm::dot(relativeVelocity, normal);
		
		if (velocityAlongNormal > 0) {
			return;
		}
		
		float restitution = 0.95f;
		
		float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
		impulseScalar /= 2.0f;
		
		glm::vec3 impulse = impulseScalar * normal;
		
		this->velocity += impulse;
		ball->velocity -= impulse;
		
		this->prevPosition = this->position;
		ball->prevPosition = ball->position;

		if (this->color == "red" && this->hasCollided == false) {
			if (ball->GetColor() == gScore.cueBallColor) {
				gCollisionInfo.cntRed++;
			}
		}
		else if (this->color == "white" && this->color == gScore.cueBallColor) {
			if (ball->GetColor() == "yellow") {
				gCollisionInfo.cntYellow++;
			}
		}
		else if (this->color == "yellow" && this->color == gScore.cueBallColor) {
			if (ball->GetColor() == "white") {
				gCollisionInfo.cntWhite++;
			}
		}
	}
	else if (name == "ball:wall") {
		Collider* wall = static_cast<Collider*>(other);
		std::vector<Wall> walls = wall->GetWalls();
		glm::vec3 collisionNol = glm::vec3(0, 0, 0);

		for (auto& r : walls) {
			if (r.isCollide) {
				collisionNol = r.nol;
			}
		}

		if (glm::length(collisionNol) < 0.01f) return;

		float minX = wall->GetMinX();
		float maxX = wall->GetMaxX();
		float minZ = wall->GetMinZ();
		float maxZ = wall->GetMaxZ();
		
		if (std::abs(collisionNol.x) > std::abs(collisionNol.z)) {
			if (collisionNol.x > 0) {
				this->position.x = minX + this->radius;
			}
			else {
				this->position.x = maxX - this->radius;
			}
			this->velocity.x *= -0.95f;
		}
		else {
			float centerZ = (minZ + maxZ) / 2.0f;
			
			if (collisionNol.z > 0) {
				this->position.z = minZ + this->radius;
			}
			else {
				this->position.z = maxZ - this->radius;
			}
			this->velocity.z *= -0.95f;
		}
		
		this->prevPosition = this->position;

		UpdateModelMat(0.0f);
	}
}

glm::vec3 Ball::GetPosition() const {
	return this->position;
}

glm::vec3 Ball::GetPrevPosition() const {
	return this->prevPosition;
}

float Ball::GetRadius() const {
	return this->radius;
}

void Ball::SetIsWallCollision(bool a) {
	this->isWallCollision = a;
}

bool Ball::GetIsWallCollision() {
	return this->isWallCollision;
}

std::string Ball::GetColor() {
	return this->color;
}

glm::vec3 Ball::GetVelocity() const {
	return this->velocity;
}