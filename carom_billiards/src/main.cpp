#include <iostream>
#include <vector>
#include <memory>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "MyExtern.h"
#include "MyUtils.h"
#include "MyStruct.h"
#include "MyCallback.h"
#include "game_world.h"
#include "play_mode.h"
#include "game_framework.h"

#include "./objects/GameObject.h"
#include "./objects/Table.h"

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	Framework myGame;
	GameMode* startMode = new PlayMode();
	myGame.Run(argc, argv, startMode);

}