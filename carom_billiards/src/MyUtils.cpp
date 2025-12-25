#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE_LENGTH 1200
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm> 
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "MyUtils.h"
#include "MyStruct.h"
#include "MyExtern.h"

void read_newline(char* str) {
	char* pos;
	if ((pos = strchr(str, '\n')) != NULL)
		*pos = '\0';
}
void read_obj_file(const char* filename, Model* model) {
	FILE* file;
	fopen_s(&file, filename, "r");
	if (!file) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	char line[MAX_LINE_LENGTH];
	model->vertex_count = 0;
	model->face_count = 0;
	model->normal_count = 0;

	// 첫 번째 패스: 개수 세기
	while (fgets(line, sizeof(line), file)) {
		read_newline(line);
		if (line[0] == 'v' && line[1] == ' ')
			model->vertex_count++;
		else if (line[0] == 'f' && line[1] == ' ')
			model->face_count++;
		else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
			model->normal_count++;
	}

	fseek(file, 0, SEEK_SET);

	// 메모리 할당
	model->vertices = (Vertex*)malloc(model->vertex_count * sizeof(Vertex));
	model->faces = (Face*)malloc(model->face_count * sizeof(Face));
	model->normals = (Vertex*)malloc(model->normal_count * sizeof(Vertex));

	size_t vertex_index = 0;
	size_t face_index = 0;
	size_t normal_index = 0;

	// 두 번째 패스: 데이터 읽기
	while (fgets(line, sizeof(line), file)) {
		read_newline(line);
		if (line[0] == 'v' && line[1] == ' ') {
			sscanf_s(line + 2, "%f %f %f",
				&model->vertices[vertex_index].x,
				&model->vertices[vertex_index].y,
				&model->vertices[vertex_index].z);
			vertex_index++;
		}
		else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
			sscanf_s(line + 2, "%f %f %f",
				&model->normals[normal_index].x,
				&model->normals[normal_index].y,
				&model->normals[normal_index].z);
			normal_index++;
		}
		else if (line[0] == 'f' && line[1] == ' ') {
			unsigned int v1, v2, v3, vn1, vn2, vn3;
			// f 1//1 2//2 3//3 형식 파싱
			int parsed = sscanf_s(line + 2, "%u//%u %u//%u %u//%u",
				&v1, &vn1, &v2, &vn2, &v3, &vn3);

			if (parsed == 6) {
				// v//vn 형식
				model->faces[face_index].v1 = v1 - 1;
				model->faces[face_index].v2 = v2 - 1;
				model->faces[face_index].v3 = v3 - 1;
				model->faces[face_index].vn1 = vn1 - 1;
				model->faces[face_index].vn2 = vn2 - 1;
				model->faces[face_index].vn3 = vn3 - 1;
			}
			else {
				// v 형식 (노말 없음)
				sscanf_s(line + 2, "%u %u %u", &v1, &v2, &v3);
				model->faces[face_index].v1 = v1 - 1;
				model->faces[face_index].v2 = v2 - 1;
				model->faces[face_index].v3 = v3 - 1;
				model->faces[face_index].vn1 = 0;
				model->faces[face_index].vn2 = 0;
				model->faces[face_index].vn3 = 0;
			}
			face_index++;
		}
	}
	fclose(file);
}
void print_model_info(const Model* model) {
	std::cout << "========== Model Information ==========" << std::endl;
	std::cout << "Vertex Count: " << model->vertex_count << std::endl;
	std::cout << "Normal Count: " << model->normal_count << std::endl;
	std::cout << "Face Count: " << model->face_count << std::endl;
	std::cout << std::endl;

	std::cout << "---------- Vertices ----------" << std::endl;
	for (size_t i = 0; i < model->vertex_count; ++i) {
		std::cout << "v[" << i << "]: ("
			<< model->vertices[i].x << ", "
			<< model->vertices[i].y << ", "
			<< model->vertices[i].z << ")" << std::endl;
	}
	std::cout << std::endl;

	std::cout << "---------- Normals ----------" << std::endl;
	if (model->normals != nullptr && model->normal_count > 0) {
		for (size_t i = 0; i < model->normal_count; ++i) {
			std::cout << "vn[" << i << "]: ("
				<< model->normals[i].x << ", "
				<< model->normals[i].y << ", "
				<< model->normals[i].z << ")" << std::endl;
		}
	}
	else {
		std::cout << "No normals loaded." << std::endl;
	}
	std::cout << std::endl;

	std::cout << "---------- Faces ----------" << std::endl;
	for (size_t i = 0; i < model->face_count; ++i) {
		std::cout << "f[" << i << "]: v("
			<< model->faces[i].v1 << ", "
			<< model->faces[i].v2 << ", "
			<< model->faces[i].v3 << ") vn("
			<< model->faces[i].vn1 << ", "
			<< model->faces[i].vn2 << ", "
			<< model->faces[i].vn3 << ")" << std::endl;
	}
	std::cout << "=======================================" << std::endl;
}

void InitBuffers(GLuint& VAO, GLuint& VBO_pos, GLuint& VBO_nol, GLuint& EBO,
	Vertex* vData, Vertex* vNormal, Face* fData, size_t vCount, size_t fCount, size_t nCount) {

	// 면마다 정점과 노말을 확장 (인덱스 기반 → 직접 데이터)
	std::vector<Vertex> expandedVertices;
	std::vector<Vertex> expandedNormals;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < fCount; ++i) {
		// 삼각형의 세 정점
		expandedVertices.push_back(vData[fData[i].v1]);
		expandedVertices.push_back(vData[fData[i].v2]);
		expandedVertices.push_back(vData[fData[i].v3]);

		// 해당하는 노말
		expandedNormals.push_back(vNormal[fData[i].vn1]);
		expandedNormals.push_back(vNormal[fData[i].vn2]);
		expandedNormals.push_back(vNormal[fData[i].vn3]);

		// 인덱스
		indices.push_back(i * 3 + 0);
		indices.push_back(i * 3 + 1);
		indices.push_back(i * 3 + 2);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_pos);
	glGenBuffers(1, &VBO_nol);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, expandedVertices.size() * sizeof(Vertex),
		expandedVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_nol);
	glBufferData(GL_ARRAY_BUFFER, expandedNormals.size() * sizeof(Vertex),
		expandedNormals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}
char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}
void make_vertexShaders()
{
	GLchar* vertexSource;
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
void make_fragmentShaders()
{
	GLchar* fragmentSource;
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
GLuint make_shaderProgram()
{
	GLint result;
	GLchar* errorLog = NULL;
	GLuint shaderID;
	shaderID = glCreateProgram(); //--- 세이더 프로그램 만들기
	glAttachShader(shaderID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(shaderID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(shaderID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(shaderID); //--- 만들어진 세이더 프로그램 사용하기
	//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
	//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
	//--- 사용하기 직전에 호출할 수 있다.
	return shaderID;
}