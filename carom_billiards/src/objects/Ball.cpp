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
	this->angularVelocity = glm::vec3(0.0f);
	this->pendingImpulse = glm::vec3(0.0f);
	this->modelMat = glm::mat4(1.0f);
	this->objectName = "ball";
	this->color = color;
	this->isWallCollision = false;
	this->hasCollided = false;

	if (this->color == "white") {
		this->isCueBall = true;
	}
	else {
		this->isCueBall = false;
	}

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
    // 회전 적용 (단순히 현재 각속도만큼 회전각을 더함)
    if (glm::length(this->angularVelocity) > 0.001f) {
        glm::vec3 axis = glm::normalize(this->angularVelocity);
        float angle = glm::length(this->angularVelocity) * dt;
        
        // GLM 쿼터니언이나 회전 행렬 누적을 권장하지만, 
        // 기존 코드 스타일을 유지한다면 아래처럼 오일러 각에 더하는 방식 사용 (짐벌락 주의)
        // 여기서는 기존 로직 유지하되 감속 코드만 제거함
        this->rotation += axis * angle; 
    }
}

void Ball::SetScale() {

}

void Ball::SetVelocity(glm::vec3 v) {
	this->velocity = v;
}

void Ball::UpdateVelocity(float dt) {
	// 속도가 너무 작으면 정지 처리
	if (glm::length(this->velocity) < 0.01f && glm::length(this->angularVelocity) < 0.1f) {
		this->velocity = glm::vec3(0.0f);
		this->angularVelocity = glm::vec3(0.0f);
		return;
	}

	// --- 물리 상수 설정 (게임 스케일에 맞춰 조절 필요) ---
	const float GRAVITY = 980.0f; // 중력 가속도 (단위가 cm라면 980, m라면 9.8)
	const float MU_SLIDE = 0.2f;  // 미끄러짐 마찰 계수 (오시/시끼가 먹는 정도)
	const float MU_ROLL = 0.015f; // 구름 저항 계수 (자연스러운 감속)

	// 1. 접촉점(공의 맨 아래)의 속도 계산
	// v_contact = v_center + (angularVelocity X radius_vector)
	// radius_vector는 중심에서 바닥으로 향하는 벡터 (0, -radius, 0)
	glm::vec3 rVec = glm::vec3(0, -this->radius, 0);
	glm::vec3 slipVelocity = this->velocity + glm::cross(this->angularVelocity, rVec);
	slipVelocity.y = 0.0f; // 바닥 평면에서의 미끄러짐만 고려

	// 2. 미끄러짐(Sliding) 상태인지 확인
	if (glm::length(slipVelocity) > 0.5f) {
		// --- [미끄러짐 상태] : 오시/시끼가 작용하는 구간 ---

		// 마찰력은 미끄러지는 방향의 반대
		glm::vec3 frictionDir = -glm::normalize(slipVelocity);

		// 마찰력 F = mu * m * g (여기서는 m=1로 가정하여 가속도 a = F)
		glm::vec3 frictionAccel = frictionDir * MU_SLIDE * GRAVITY;

		// 선속도 변화 (F = ma)
		this->velocity += frictionAccel * dt;

		// 각속도 변화 (Torque = r X F, I = 2/5 * m * r^2)
		// 각가속도 alpha = Torque / I
		float I = 0.4f * this->radius * this->radius; // 관성 모멘트
		glm::vec3 torque = glm::cross(rVec, frictionAccel * 1.0f); // Force * mass(1.0)
		this->angularVelocity += (torque / I) * dt;

	}
	else {
		// --- [구름(Rolling) 상태] : 자연스럽게 굴러가는 구간 ---

		// 단순 구름 저항만 적용 (자연 감속)
		float speed = glm::length(this->velocity);
		if (speed > 0) {
			glm::vec3 moveDir = glm::normalize(this->velocity);
			glm::vec3 resistAccel = -moveDir * MU_ROLL * GRAVITY;
			this->velocity += resistAccel * dt;

			// 구름 상태에서는 각속도가 선속도에 동기화됨 (v = r * w)
			// 굴러가는 축: 진행 방향의 수직
			glm::vec3 rollAxis = glm::cross(glm::vec3(0, 1, 0), moveDir);
			float omegaMag = speed / this->radius;
			this->angularVelocity = rollAxis * omegaMag;
		}
	}
}

void Ball::UpdateModelMat(float dt) {
	glm::mat4 transMat = glm::translate(glm::mat4(1.0), this->position);
	
	// 회전 적용
	glm::mat4 rotMat = glm::mat4(1.0f);
	if (glm::length(this->rotation) > 0.001f) {
		rotMat = glm::rotate(rotMat, this->rotation.y, glm::vec3(0, 1, 0));
		rotMat = glm::rotate(rotMat, this->rotation.x, glm::vec3(1, 0, 0));
		rotMat = glm::rotate(rotMat, this->rotation.z, glm::vec3(0, 0, 1));
	}

	this->modelMat = transMat * rotMat;
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

		// 충돌 감지 실패 혹은 이미 겹침이 해결된 경우
		if (distance < 0.001f || distance >= (this->radius + ball->GetRadius())) {
			return;
		}

		glm::vec3 normal = glm::normalize(delta);

		// 1. 위치 보정 (공이 서로 파고들지 않게 밀어냄)
		float overlap = (this->radius + ball->GetRadius()) - distance;
		this->position += normal * (overlap * 0.5f);

		// 2. 상대 속도 계산
		glm::vec3 relativeVelocity = this->velocity - ball->velocity;
		float velocityAlongNormal = glm::dot(relativeVelocity, normal);

		// 이미 서로 멀어지고 있는 경우 충돌 처리 안 함
		if (velocityAlongNormal > 0) {
			return;
		}

		// 3. 반발 계수 (당구공은 탄성이 매우 높음)
		// 0.98f로 설정하여 충돌 후 에너지 손실을 최소화 (깔끔한 분리)
		float restitution = 0.98f;

		// 4. 충격량(Impulse) 계산 (질량 m=1 가정)
		// j = -(1 + e) * (v_rel . n) / (1/m1 + 1/m2)
		// m1=m2=1 이므로 분모는 2
		float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
		impulseScalar /= 2.0f;

		glm::vec3 impulse = impulseScalar * normal;

		// 5. 충격량 적용
		this->pendingImpulse += impulse;
		this->prevPosition = this->position;

		// [중요] 공과 공 사이의 마찰/회전 전달 코드를 모두 제거했습니다.
		// 이제 충돌 순간에는 '선속도'만 변하고 '회전(Angular Velocity)'은 보존됩니다.
		// 보존된 회전이 UpdateVelocity()에서 바닥 마찰을 일으켜 오시/시끼를 만듭니다.

		// --- 게임 로직: 점수 및 충돌 체크 ---
		if (this->color == "red" && this->hasCollided == false) {
			if (ball->GetColor() == gScore.cueBallColor) {
				gCollisionInfo.cntRed++;
				this->hasCollided = true;
			}
		}
		else if (this->color == "white" && this->color != gScore.cueBallColor && this->hasCollided == false) {
			if (ball->GetColor() == "yellow") {
				gCollisionInfo.cntWhite++;
				this->hasCollided = true;
			}
		}
		else if (this->color == "yellow" && this->color != gScore.cueBallColor && this->hasCollided == false) {
			if (ball->GetColor() == "white") {
				gCollisionInfo.cntYellow++;
				this->hasCollided = true;
			}
		}
	}
	else if (name == "ball:wall") {
		Collider* wall = static_cast<Collider*>(other);
		std::vector<Wall> walls = wall->GetWalls();
		glm::vec3 collisionNol = glm::vec3(0, 0, 0);

		// 어떤 벽과 부딪혔는지 확인
		for (auto& r : walls) {
			if (r.isCollide) {
				collisionNol = r.nol;
				break;
			}
		}

		if (glm::length(collisionNol) < 0.01f) return;

		// 벽(쿠션) 충돌 시에는 회전이 반사각에 영향을 줌 (히네리 효과)
		float sideSpin = this->angularVelocity.y;
		glm::vec3 tangent = glm::normalize(glm::cross(collisionNol, glm::vec3(0, 1, 0)));

		// 쿠션에서의 스핀 반응성 (필요시 조절)
		float spinFactor = 2.0f;

		float minX = wall->GetMinX();
		float maxX = wall->GetMaxX();
		float minZ = wall->GetMinZ();
		float maxZ = wall->GetMaxZ();

		// X축 벽 충돌 (좌우 쿠션)
		if (std::abs(collisionNol.x) > std::abs(collisionNol.z)) {
			if (collisionNol.x > 0) {
				this->position.x = minX + this->radius;
			}
			else {
				this->position.x = maxX - this->radius;
			}
			this->velocity.x *= -0.95f; // 벽 반발 계수
			// 스핀에 의한 반사각 변화 적용
			this->velocity.z += tangent.z * sideSpin * spinFactor;
		}
		// Z축 벽 충돌 (상하 쿠션)
		else {
			if (collisionNol.z > 0) {
				this->position.z = minZ + this->radius;
			}
			else {
				this->position.z = maxZ - this->radius;
			}
			this->velocity.z *= -0.95f; // 벽 반발 계수
			// 스핀에 의한 반사각 변화 적용
			this->velocity.x += tangent.x * sideSpin * spinFactor;
		}

		this->prevPosition = this->position;

		// 쿠션에 맞으면 회전이 약간 감소함
		this->angularVelocity.y *= 0.9f;
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

bool Ball::GetIsWallCollision() const {
	return this->isWallCollision;
}

std::string Ball::GetColor() {
	return this->color;
}

glm::vec3 Ball::GetVelocity() const {
	return this->velocity;
}

void Ball::SetIsCueBall(bool a) {
	this->isCueBall = a;
}
bool Ball::GetIsCueBall() const {
	return this->isCueBall;
}

void Ball::SetHasCollided(bool a) {
	this->hasCollided = a;
}

void Ball::ApplyImpulse() {
	this->velocity += this->pendingImpulse;
}

void Ball::ClearImpulse() {
	this->pendingImpulse = glm::vec3(0.0f);
}

void Ball::SetAngularVelocity(glm::vec3 angVel) {
	this->angularVelocity = angVel;
}

glm::vec3 Ball::GetAngularVelocity() const {
	return this->angularVelocity;
}