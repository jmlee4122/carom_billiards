#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm> 
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "MyCallback.h"
#include "MyExtern.h"
#include "MyStruct.h"
#include "MyUtils.h"

#include "./objects/GameObject.h"

GLvoid DrawScene() {
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (const auto& obj : world) {
		obj->Render();
	}
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {

}

GLvoid Keyboard(unsigned char key, int x, int y) {

}

GLvoid KeyboardUp(unsigned char key, int x, int y) {

}

GLvoid SpecialKey(int key, int x, int y) {
	glutPostRedisplay();
}

GLvoid SpecialKeyUp(int key, int x, int y) {
	glutPostRedisplay();
}

GLvoid Timer(int value) {

}

GLvoid Mouse(int button, int state, int x, int y) {

}
GLvoid MouseMotion(int x, int y) {

}

GLvoid PassiveMotion(int x, int y) {

}