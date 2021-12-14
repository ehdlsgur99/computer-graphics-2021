#pragma once

#include "common.h"



#define SIDE_MOVE_SPEED 0.1

class TextureClass;

// body is main
class Player : public Object {
//private:
	// body
	TextureClass* m_pTextureBody;
	TextureClass* m_pTextureLeg;

	Object* m_pBody;

public:
	Player() = delete;
	Player(float size, glm::vec3 pivot);
	~Player();

private:
	bool m_bAlive = false;
public:
	static float angle;
	bool life = true;
	bool invincibility = false;
private:
	glm::vec3 m_vLeftRot;
	glm::vec3 m_vRightRot;
	glm::vec3 m_vBackRot;
	glm::vec3 m_rotate_x;
	glm::vec3 m_rotate_z;
	glm::vec3 dir;
	int direction;
	int GameOverType;
	bool jumpdir;
	bool isjump;
	float max_jump_height;
	glm::vec3 org_pos;
private:
	static glm::vec3 m_vDir;
	static glm::vec3 m_vForward;
	
public:
	void input(char key);
	void update(float deltaTime);
	void draw(unsigned int shaderNum, int textureBind);

	// coin 획득하면 발생하는 파티클
	void getCoin();
	void destroyparticle();

	static void setDirZero();
	static void setForward(glm::vec3 newFow);
	void setRotateByCamera(glm::vec3 veye);
	float get_angle();
	void Player_side_move(char key);
	void jump(float dt);
public:
	void moveBack(glm::vec3 backHow);
	void collision(int type);
	void GameOver(float dt);

};

