
#define _CRT_SECURE_NO_WARNINGS

#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#define MAP_WIDTH 10
#define MAP_HEIGHT 10

GLuint modelLoc;
GLuint VAO[3], VBO[6];
GLUquadricObj* qobj;
GLuint s_program;
int winId;

void make_vertexShaders();
void make_fragmentShaders();

void Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
void Timer(int val);

int lightColorLocation;

bool isLight = true;

float yAngle = 30.0f;

float g_window_w = 0;
float g_window_h = 0;

glm::vec3 cameraMovePos;
float cameraRotate = 0;
bool isCameraRotate = false;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 12.0f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

class Player {
public:
	bool isJump;
	float xDirection;
	float yDirection;
	float zDirection;
	GLuint playerVAO, playerVBO[2];
	glm::vec3 pos;
	float playerPos[36][3] = {
	0.25,0.25,0.5, -0.25,0.25,0.5, -0.25,-0.25,0.5, -0.25,-0.25,0.5, 0.25,-0.25,0.5, 0.25,0.25,0.5, //앞
	0.25,0.25,0.0, 0.25,-0.25,0.0, -0.25,-0.25,0.0, -0.25,-0.25,0.0, -0.25,0.25,0.0, 0.25,0.25,0.0,//뒤
	0.25,0.25,0.5, 0.25,-0.25,0.5, 0.25,-0.25,0.0, 0.25,-0.25,0.0, 0.25,0.25,0.0, 0.25,0.25,0.5,//오
	-0.25,0.25,0.5, -0.25,0.25,0.0, -0.25,-0.25,0.0, -0.25,-0.25,0.0, -0.25,-0.25,0.5, -0.25,0.25,0.5,//왼
	0.25,0.25,0.5, 0.25,0.25,0.0, -0.25,0.25,0.0, -0.25,0.25,0.0, -0.25,0.25,0.5, 0.25,0.25,0.5,//위
	0.25,-0.25,0.5, -0.25,-0.25,0.5, -0.25,-0.25,0.0, -0.25,-0.25,0.0, 0.25,-0.25,0.0, 0.25,-0.25,0.5//아래
	};
	float playerColor[36][3] = { 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, //앞
		0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, //뒤
		0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, //오
		1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, //왼
		0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, //위
		0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5  //아래 
	};
public:
	void init()
	{
		isJump = false;

		xDirection = 0;
		yDirection = 0;
		zDirection = 0;
		pos.x = 0;
		pos.y = 0.24f;
		pos.z = 0;

		glGenVertexArrays(1, &playerVAO);
		glGenBuffers(2, playerVBO);
		//---모든 사각형을 위한 VAO
		glBindVertexArray(playerVAO);
		glBindBuffer(GL_ARRAY_BUFFER, playerVBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(playerPos), playerPos, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//--- 변수 colors에서 버텍스 색상을 복사한다.
		glBindBuffer(GL_ARRAY_BUFFER, playerVBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(playerColor), playerColor, GL_STATIC_DRAW);
		//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 1번을 사용 가능하게 함.
		glEnableVertexAttribArray(1);
	}
	void render()
	{
		glUseProgram(s_program);
		// renderPlayer
		glBindVertexArray(playerVAO);
		int modelLoc = glGetUniformLocation(s_program, "transform"); //--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.

		//--- 모델링 변환, 뷰잉 변환, 투영 변환 행렬을 설정한 후, 버텍스 세이더에 저장한다.
		glm::mat4 mTransform = glm::mat4(1.0f);
		mTransform = glm::translate(mTransform, glm::vec3(pos.x * 0.25, pos.y , pos.z * 0.25));
		mTransform = glm::scale(mTransform, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
};




class Map {
public:
	// 0 = empty, 1 = coin, 2 = obstacle 
	int map[MAP_WIDTH][MAP_HEIGHT];
	GLuint tilesVAO[MAP_WIDTH * MAP_HEIGHT], tilesVBO[2][MAP_WIDTH * MAP_HEIGHT];
	GLuint coinVAO, coinVBO[2];
	GLuint obstacleVAO, obstacleVBO[2];
	float obstacleRotation = 0;

	float tilePos[36][3] = { 
		0.25,0.25,0.5, -0.25,0.25,0.5, -0.25,-0.25,0.5, -0.25,-0.25,0.5, 0.25,-0.25,0.5, 0.25,0.25,0.5, //앞
		0.25,0.25,0.0, 0.25,-0.25,0.0, -0.25,-0.25,0.0, -0.25,-0.25,0.0, -0.25,0.25,0.0, 0.25,0.25,0.0,//뒤
		0.25,0.25,0.5, 0.25,-0.25,0.5, 0.25,-0.25,0.0, 0.25,-0.25,0.0, 0.25,0.25,0.0, 0.25,0.25,0.5,//오
		-0.25,0.25,0.5, -0.25,0.25,0.0, -0.25,-0.25,0.0, -0.25,-0.25,0.0, -0.25,-0.25,0.5, -0.25,0.25,0.5,//왼
		0.25,0.25,0.5, 0.25,0.25,0.0, -0.25,0.25,0.0, -0.25,0.25,0.0, -0.25,0.25,0.5, 0.25,0.25,0.5,//위
		0.25,-0.25,0.5, -0.25,-0.25,0.5, -0.25,-0.25,0.0, -0.25,-0.25,0.0, 0.25,-0.25,0.0, 0.25,-0.25,0.5//아래
	};
	float tileColor[36][3] = { 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, //앞
		0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, //뒤
		0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, //오
		1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, //왼
		0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, //위
		0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5  //아래 
	};
	float coinColor[36][3] = { 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, //앞
		0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, //뒤
		0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, //오
		1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, //왼
		0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, //위
		0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5  //아래 
	};
	float obstacleColor[36][3] = { 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, 1.0,0.0,0.0, //앞
		0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, 0.0,1.0,0.0, //뒤
		0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, 0.0,0.0,1.0, //오
		1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, 1.0,1.0,0.0, //왼
		0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, 0.0,1.0,1.0, //위
		0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5, 0.5,0.5,0.5  //아래 
	};
public:
	void init()
	{
		// map 초기화
		int random = 0;
		for (int i = 0; i < MAP_WIDTH; i++)
		{
			for (int j = 0; j < MAP_HEIGHT; j++)
			{
				random = rand() % 20;
				if (random == 1) // coin
				{
					map[i][j] = 1;
				}
				else if (random == 2)
				{
					map[i][j] = 2;
				}
				else
					map[i][j] = 0;
				
			}
		}
		// init tile vao, vbo
		glGenVertexArrays(MAP_WIDTH * MAP_HEIGHT, tilesVAO);
		glGenBuffers(MAP_WIDTH * MAP_HEIGHT, tilesVBO[0]);
		glGenBuffers(MAP_WIDTH * MAP_HEIGHT, tilesVBO[1]);
		for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
			//---모든 사각형을 위한 VAO
			glBindVertexArray(tilesVAO[0]);
			glBindBuffer(GL_ARRAY_BUFFER, tilesVBO[0][0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(tilePos), tilePos, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			//--- 변수 colors에서 버텍스 색상을 복사한다.
			glBindBuffer(GL_ARRAY_BUFFER, tilesVBO[1][0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(tileColor), tileColor, GL_STATIC_DRAW);
			//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			//--- attribute 인덱스 1번을 사용 가능하게 함.
			glEnableVertexAttribArray(1);
		}

		// init coin
		glGenVertexArrays(1, &coinVAO);
		glGenBuffers(2, coinVBO);
		//---모든 사각형을 위한 VAO
		glBindVertexArray(coinVAO);
		glBindBuffer(GL_ARRAY_BUFFER, coinVBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tilePos), tilePos, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//--- 변수 colors에서 버텍스 색상을 복사한다.
		glBindBuffer(GL_ARRAY_BUFFER, coinVBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coinColor), coinColor, GL_STATIC_DRAW);
		//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 1번을 사용 가능하게 함.
		glEnableVertexAttribArray(1);

		// ================
		// init obstacle
		// ================
		glGenVertexArrays(1, &obstacleVAO);
		glGenBuffers(2, obstacleVBO);
		//---모든 사각형을 위한 VAO
		glBindVertexArray(obstacleVAO);
		glBindBuffer(GL_ARRAY_BUFFER, obstacleVBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tilePos), tilePos, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//--- 변수 colors에서 버텍스 색상을 복사한다.
		glBindBuffer(GL_ARRAY_BUFFER, obstacleVBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(obstacleColor), obstacleColor, GL_STATIC_DRAW);
		//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//--- attribute 인덱스 1번을 사용 가능하게 함.
		glEnableVertexAttribArray(1);

		printf("플레이어 좌우상하 이동 : j,k,i,k : \n");
		printf("카메라 좌우상하 이동 : a,d,w,s : \n");
		printf("조명 on/off : t \n");
		printf("플레이어 점프 : e \n");
		printf("모든 값 초기화 : r \n");
		printf("카메라 y축 +/- 회전 : c/C \n");
		printf("종료 : q \n");
	}

	void redner()
	{
		glUseProgram(s_program);
		// renderTiles
		glBindVertexArray(tilesVAO[0]);
		int modelLoc = glGetUniformLocation(s_program, "transform"); //--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.
	
		for (int z = 0; z < MAP_WIDTH; ++z)
		{
			for (int x = 0; x < MAP_HEIGHT; ++x)	
			{
				//--- 모델링 변환, 뷰잉 변환, 투영 변환 행렬을 설정한 후, 버텍스 세이더에 저장한다.
				glm::mat4 mTransform = glm::mat4(1.0f);
			/*	mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0f, 0.0, 0.0f));
				mTransform = glm::rotate(mTransform, glm::radians(yAngle), glm::vec3(0.0f, 1.0, 0.0f));
		*/		mTransform = glm::translate(mTransform, glm::vec3(-(MAP_WIDTH*0.25)/2 +  x * 0.25, 0.0f, z * 0.25));

				mTransform = glm::scale(mTransform, glm::vec3(0.5f, 0.5f, 0.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
				glDrawArrays(GL_TRIANGLES, 0, 36);

				// 만약 위에 있는게 코인이라면
				if (map[z][x] == 1)
				{
					glBindVertexArray(coinVAO);
					int modelLoc = glGetUniformLocation(s_program, "transform"); //--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.

					glm::mat4 mTransform = glm::mat4(1.0f);
					mTransform = glm::translate(mTransform, glm::vec3(-(MAP_WIDTH * 0.25) / 2 + x * 0.25, 0.25f, z * 0.25));

					mTransform = glm::scale(mTransform, glm::vec3(0.25f, 0.75f, 0.25f));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				// 위에 있는게 장애물 이라면
				if (map[z][x] == 2)
				{
					glBindVertexArray(obstacleVAO);
					int modelLoc = glGetUniformLocation(s_program, "transform"); //--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.

					glm::mat4 mTransform = glm::mat4(1.0f);
					mTransform = glm::translate(mTransform, glm::vec3(-(MAP_WIDTH * 0.25) / 2 + x * 0.25, 0.25f, z * 0.25));
					//mTransform = glm::rotate(mTransform, glm::radians(obstacleRotation), glm::vec3(1.0f, 0.0, 1.0f));
					mTransform = glm::rotate(mTransform, glm::radians(45.0f  + obstacleRotation), glm::vec3(0.0f, 1.0, 0.0f));
					mTransform = glm::rotate(mTransform, glm::radians(45.0f), glm::vec3(1.0f, 0.0, 0.0f));

					obstacleRotation += 0.5f;
					mTransform = glm::scale(mTransform, glm::vec3(0.25f, 0.25f, 0.25f));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		}
	}
};


float vertices[] = { //--- 버텍스 속성: 좌표값(FragPos), 노말값 (Normal)
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};

// 육면체 좌표
float hexahedron_vertex[36][3] = {
   0.25,0.25,0.5, -0.25,0.25,0.5, -0.25,-0.25,0.5, -0.25,-0.25,0.5, 0.25,-0.25,0.5, 0.25,0.25,0.5, //앞
   0.25,0.25,0.0, 0.25,-0.25,0.0, -0.25,-0.25,0.0, -0.25,-0.25,0.0, -0.25,0.25,0.0, 0.25,0.25,0.0,//뒤
   0.25,0.25,0.5, 0.25,-0.25,0.5, 0.25,-0.25,0.0, 0.25,-0.25,0.0, 0.25,0.25,0.0, 0.25,0.25,0.5,//오
   -0.25,0.25,0.5, -0.25,0.25,0.0, -0.25,-0.25,0.0, -0.25,-0.25,0.0, -0.25,-0.25,0.5, -0.25,0.25,0.5,//왼
   0.25,0.25,0.5, 0.25,0.25,0.0, -0.25,0.25,0.0, -0.25,0.25,0.0, -0.25,0.25,0.5, 0.25,0.25,0.5,//위
   0.25,-0.25,0.5, -0.25,-0.25,0.5, -0.25,-0.25,0.0, -0.25,-0.25,0.0, 0.25,-0.25,0.0, 0.25,-0.25,0.5//아래
};

float hexahedron_colors[36][3] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
};

//--- 삼각형 데이터 방식 1: 
float tetrahedron_vertex[16][3] = { {-0.5, 0.0, 0.5}, {0.5, 0.0, 0.5}, {0.0, 0.0, -0.5}, // 아래
	{-0.5, 0.0, 0.5},{0.0, 0.0, -0.5},{0.0, 0.5, 0.0}, // 왼
	{0.5, 0.0, 0.5}, {0.0, 0.0, -0.5}, {0.0, 0.5, 0.0}, // 오
	{-0.5, 0.0, 0.5}, {0.5, 0.0, 0.5}, {0.0, 0.5, 0.0}, };//앞 };

float tetrahedron_colors[16][3] = { 0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f };//앞 };

// -- - 좌표계 데이터 : 선 2개에 대한 속성
float LineData[] = {
-1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f,
0.0f, -1.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 1.0f,
0.0f, 0.0f, -1.0f
};

float lineColor[6][3] = {
1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f ,
0.0f, 1.0f, 0.0f };

Map map;
Player player;
std::vector<Player> players;

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	g_window_w = w;
	g_window_h = h;
	glViewport(0, 0, w, h);
}

void followPlayer()
{
	for (int i = 0; i < players.size(); i++)
	{

		if (i == 0)
		{
			float distance = sqrt(pow(player.pos.x - players[i].pos.x, 2) + pow(player.pos.z - players[i].pos.z, 2));
			if (distance > 1.0f)
			{
				glm::vec3 toDir = player.pos - players[i].pos;
				toDir = glm::normalize(toDir);

				players[i].pos += toDir * 0.01f;
			}
		}
		else
		{
			float distance = sqrt(pow(players[i - 1].pos.x - players[i].pos.x, 2) + pow(players[i - 1].pos.z - players[i].pos.z, 2));
			if (distance > 1.0f)
			{
				glm::vec3 toDir = players[i - 1].pos - players[i].pos;
				toDir = glm::normalize(toDir);

				players[i].pos += toDir * 0.01f;
			}
		}
	}
}

int checkCollision(glm::vec3 pos)
{
	float distance = 10.0f;
	for (int z = 0; z < MAP_WIDTH; ++z)
	{
		for (int x = 0; x < MAP_HEIGHT; ++x)
		{
			if (map.map[z][x] == 2)
			{
				distance = sqrt(pow(x  - (pos.x + 5.0f), 2) + pow(z  - pos.z, 2));
				if (distance < 1.0f)
				{
					return 2;
				}
			}
			if (map.map[z][x] == 1)
			{
				distance = sqrt(pow(x - (pos.x + 5.0f), 2) + pow(z - pos.z, 2));
				if (distance < 1.0f)
				{
					map.map[z][x] = 0;
					Player p;
					p.init();
					p.pos.x = x - 5.0f;
					p.pos.z = z;
					players.push_back(p);
					return 1;
				}
			}
		
		}
	}

	return 0;
}

//--- 버퍼 생성하고 데이터 받아오기
// 사각형, 삼각형, 라인

void InitBuffer()
{
	glGenVertexArrays(3, VAO);
	glGenBuffers(5, VBO);

	//---모든 사각형을 위한 VAO
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexahedron_vertex), hexahedron_vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//--- 변수 colors에서 버텍스 색상을 복사한다.
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexahedron_colors), hexahedron_colors, GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);

	glUseProgram(s_program);
	int lightPosLocation = glGetUniformLocation(s_program, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0, 0, 2.0);
	lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1, 1, 1);
	int objColorLocation = glGetUniformLocation(s_program, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, 1.0, 0.5, 0.3);


	//	//--- 모든 삼각형을 위한 VAO
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedron_vertex), tetrahedron_vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//--- colors 배열의 사이즈: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedron_colors), tetrahedron_colors, GL_DYNAMIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);

	// -- - 라인데이터: 좌표계를 그리기 위한 VAO
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	//--- vertex data 데이터 입력.
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineData), LineData, GL_STATIC_DRAW);
	//--- 위치 속성: 속성 인덱스 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fopen_s(&fptr, file, "rb"); // Open file for reading 
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

GLuint vertexShader;
void make_vertexShaders()
{
	GLchar* vertexsource;
	vertexsource = filetobuf("vertex.glsl");
	// 버텍스세이더 읽어오기
	//--- filetobuf: 사용자 정의함수로 텍스트를 읽어서 문자열에 저장하는 함수
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexsource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
}

GLuint fragmentShader;
void make_fragmentShaders()
{
	GLchar* fragmentsource;
	fragmentsource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentsource, NULL);
	glCompileShader(fragmentShader);
}

//--- 세이더 프로그램 생성하기

void InitShader()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program //--- 한 개 이상의 셰이더 프로그램을 만들 수 있다.
	s_program = glCreateProgram();

	glAttachShader(s_program, vertexShader); //--- 다른 속성을 가지고 있는 셰이더를 사용하여 셰이더 프로그램을 따로 만들 수 있다.
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	modelLoc = glGetUniformLocation(s_program, "transform"); //--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.


	//checkCompileErrors(s_program, "PROGRAM");
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

//--- 그리기 콜백 함수
void drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glUseProgram(s_program);

	int viewLoc = glGetUniformLocation(s_program, "viewTransform"); //--- 버텍스 세이더에서 뷰잉 변환 행렬 변수값을 받아온다.
	int projLoc = glGetUniformLocation(s_program, "projectionTransform"); //--- 버텍스 세이더에서 투영 변환 행렬 변수값을 받아온다.

	glm::mat4 vTransform = glm::mat4(1.0f);
	vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	vTransform = glm::rotate(vTransform, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	vTransform = glm::rotate(vTransform, glm::radians(cameraRotate), glm::vec3(1.0f, 0.0f, 0.0f));
	vTransform = glm::translate(vTransform, glm::vec3(cameraMovePos.x , cameraMovePos.y + 0.5f, cameraMovePos.z));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &vTransform[0][0]);

	glm::mat4 pTransform = glm::mat4(1.0f);
	pTransform = glm::perspective(glm::radians(20.0f), (float)g_window_w / (float)g_window_h, 0.1f, 200.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &pTransform[0][0]);

	map.redner();
	player.render();

	for (int i = 0; i < players.size(); i++)
	{

		players[i].render();
		
	}

	glutSwapBuffers();
}


void Timer(int val)
{
	glutTimerFunc(10, Timer, 0);

	if (player.isJump)
	{
		player.pos.y += 0.05f;
		if (player.pos.y >= 1.0f)
		{
			player.isJump = false;
		}
	}
	if (player.isJump == false && player.pos.y >= 0.25f)
	{
		player.pos.y -= 0.05f;
	}

	followPlayer();
	glutPostRedisplay(); //--- 바뀐 좌표값을 사용하여 재 출력한다.
}

// 마우스
void Mouse(int button, int state, int x, int y)
{
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	int checkColl = 0;
	//i/j/k/l: 로봇의 이동방향을 앞/뒤/좌우 로 바꾸기
	if (key == 'i')
	{
		player.pos.z -= 0.1f;
		checkColl = checkCollision(player.pos);
		if (checkColl == 2)
		{
			player.pos.z += 0.1f;
		}
	}
	if (key == 'j')
	{
		player.pos.x -= 0.1f;
		checkColl = checkCollision(player.pos);
		if (checkColl == 2)
		{
			player.pos.x += 0.1f;
		}
	}
	if (key == 'k')
	{
		player.pos.z += 0.1f;
		checkColl = checkCollision(player.pos);
		if (checkColl == 2)
		{
			player.pos.z -= 0.1f;
		}
	}
	if (key == 'l')
	{
		player.pos.x += 0.1f;
		checkColl = checkCollision(player.pos);
		if (checkColl == 2)
		{
			player.pos.x -= 0.1f;
		}
	}
	//• 카메라 이동:
	// • w / a / s / d: 카메라 앞 / 뒤 / 좌 / 우 이동
	if (key == 'w')
	{
		cameraMovePos.z += 0.1f;
	}
	if (key == 'a')
	{
		cameraMovePos.x += 0.1f;
	}
	if (key == 's')
	{
		cameraMovePos.z -= 0.1f;
	}
	if (key == 'd')
	{
		cameraMovePos.x -= 0.1f;
	}

	// c/C: 화면의 중점에 대하여 y축 양/음 방향 회전
	if (key == 'c')
	{
		cameraRotate += 0.5f;
	}
	else if (key == 'C')
	{
		cameraRotate -= 0.5f;
	}

	// e: 주인공 점프
	if (key == 'e')
	{
		if (player.isJump == false && player.pos.y <= 0.25)
		{
			player.isJump = true;
		}
	}
	//• t: 조명 켜기/끄기
	if (key == 't')
	{
		isLight = !isLight;
		if (isLight == true)
		{
			lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
			glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
		}
		else
		{
			lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
			glUniform3f(lightColorLocation, 0.3, 0.3, 0.3);
		}
	}
	// • r: 모든 값 초기화
	if (key == 'r')
	{
		cameraRotate = 0;
		cameraMovePos = glm::vec3();
		isLight = !isLight;
		lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
		glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
		map.init();
		player.init();
		players.clear();
	}
	// • q: 프로그램 종료
	if (key == 'q')
	{
		glutDestroyWindow(winId);
	}

	glutPostRedisplay();
}



void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	srand(time(NULL));
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 700);
	winId = glutCreateWindow("hw2");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();
	map.init();
	player.init();
	//glutMouseFunc(Mouse);
	glutTimerFunc(100, Timer, 1);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}