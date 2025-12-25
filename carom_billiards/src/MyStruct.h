#pragma once

#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

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