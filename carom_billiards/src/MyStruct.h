#pragma once

#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

class GameObject;

struct Vertex {
	float x, y, z;
};

struct Face {
	unsigned int v1, v2, v3;
	unsigned int vn1, vn2, vn3;  // 노말 인덱스 추가
};

struct Model {
	Vertex* vertices;
	size_t vertex_count;
	Face* faces;
	size_t face_count;
	Vertex* normals;
	size_t normal_count;  // 노말 개수 추가
};

struct Wall {
	glm::vec3 start;
	glm::vec3 end;
	glm::vec3 nol;
	bool isCollide;
};

struct CollisionPair {
	std::string pairName;
	std::vector<GameObject*> objects[2];
};