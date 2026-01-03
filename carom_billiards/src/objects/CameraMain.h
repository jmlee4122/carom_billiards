#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

class CameraMain
{
public:
	CameraMain();
	~CameraMain();
	void SetYaw(float a);
	void SetAt();
	void UpdateEye();
	void UpdateMat();
	void Update();
	glm::mat4 GetViewMat() const;

private:
	glm::vec3 eye;
	glm::vec3 at;
	float dis, height; // cue ball to camera
	float yaw, pitch;
	float speed;
	glm::mat4 viewMat;
};

