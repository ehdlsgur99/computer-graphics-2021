#pragma once
#include "Object.h"
#include "Mesh.h"


class TextureClass;

// body is main
class Player : public Mesh {
//private:
	// body
	TextureClass* m_pTextureBody;
	TextureClass* m_pTextureLeg;

	Mesh* m_pBody;
public:
	Player() = delete;
	Player(float size, glm::vec3 pivot);
	~Player();

private:
	bool m_bAlive = false;

public:
	static float angle;
private:
	glm::vec3 m_vLeftRot;
	glm::vec3 m_vRightRot;
	glm::vec3 m_vBackRot;
	glm::vec3 m_angle_x;
	glm::vec3 m_rotate_y;
private:
	static glm::vec3 m_vDir;
	static glm::vec3 m_vForward;

public:
	static void input(char key);
	void update(float deltaTime, glm::vec3 veye);
	void draw(unsigned int shaderNum, int textureBind);

	static void setDirZero();
	static void setForward(glm::vec3 newFow);
	void setRotateByCamera(glm::vec3 veye);
	float get_angle();
public:
	void moveBack(glm::vec3 backHow);

};

