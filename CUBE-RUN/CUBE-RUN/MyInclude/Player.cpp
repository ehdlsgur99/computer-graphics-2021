#include "Player.h"
#include "TextureClass.h"
#include "ParticleManager.h"
#include "SoundManager.h"

glm::vec3 Player::m_vDir = glm::vec3(0.0f);
glm::vec3 Player::m_vForward = glm::vec3(0.0f, 0.0f, 1.0f);
float Player::angle(0);

Player::Player(float size, glm::vec3 pivot) :
	Mesh("Objs/Cube.obj", glm::vec3(1.0f)* size, glm::vec3(0.0f), (pivot)+glm::vec3(1.0f, 1.0f, 0.0f) * size),
	m_vRightRot(glm::vec3(0.0f)),
	m_vLeftRot(glm::vec3(0.0f)),
	m_vBackRot(glm::vec3(20.0f, 0.0f, 0.0f)),
	m_rotate_x(glm::vec3(0.0f)),
	m_rotate_z(glm::vec3(0.0f)),
	dir(glm::vec3(0.0f)),
	direction (0)
{
	m_pBody = new Mesh("Objs/Cube.obj", glm::vec3(1.0f) * size, glm::vec3(0.0f), pivot + glm::vec3(0.0f, 0.0f, 0.0f) * size);

	m_pTextureBody = new TextureClass("Texture/Player/player_texture.png");
}

Player::~Player()
{
	delete m_pTextureBody;
	delete m_pTextureLeg;

	delete m_pBody;	
}

void Player::input(char key)
{
	switch (key) {
	case 'a':
	{
		dir = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(m_vForward, 0.0f);	
		break;
	}
	case 'd':
	{
		dir = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(m_vForward, 0.0f);	
		break;
	}
	case 'z':
		direction--;
		if (direction <= -1) direction = 3;
		angle -= 90.0f;
		dir = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(m_vForward, 0.0f);
		//this->setRotate(glm::vec3(0.0f,90.0f,0.0f));
		break;
	case 'x':
		direction++;
		direction = direction % 4;
		angle += 90.0f;
		dir = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(m_vForward, 0.0f);
		//this->setRotate(glm::vec3(0.0f,-90.0f,0.0f));
		break;
	}
	printf("%d\n", direction);
	m_vDir = glm::normalize(m_vDir + dir);
}

void Player::update(float deltaTime)
{
	static float fMoveSpeed = 1.0f;
	static float fRotateSpeed = 100.0f;
	static bool bIncreaseFront = true;
	static bool bIncreaseBack = false;
	glm::vec3 rotate = glm::vec3(0.0f);

	switch (direction)
	{
	case 0:
		rotate = glm::vec3(0.0f, 0.0f, 1.0f);
		m_rotate_z += rotate * fRotateSpeed * deltaTime;
		this->setRotate(m_rotate_z);
		break;
	case 1:
		rotate = glm::vec3(1.0f, 0.0f, 0.0f);
		m_rotate_x -= rotate * fRotateSpeed * deltaTime;
		this->setRotate(m_rotate_x);
		break;
	case 2:
		rotate = glm::vec3(0.0f, 0.0f, 1.0f);
		m_rotate_z -= rotate * fRotateSpeed * deltaTime;
		this->setRotate(m_rotate_z);
		break;
	case 3:
		rotate = glm::vec3(1.0f, 0.0f, 0.0f);
		m_rotate_x += rotate * fRotateSpeed * deltaTime;
		this->setRotate(m_rotate_x);
		break;
	}
	

	m_vDir = m_vForward;
	glm::vec3 offset = m_vDir * fMoveSpeed * deltaTime;
	this->setTranslate(m_vPivot + offset);


}

void Player::setRotateByCamera(glm::vec3 veye)
{
	this->setRotate(veye);
}

void Player::draw(unsigned int shaderNum, int textureBind)
{
	glm::mat4 modeling(1.0f);

	m_pTextureBody->bindTexture(textureBind);

	// body
	modeling = this->m_mSRTModel * m_pBody->getModelTransform();
	glUniformMatrix4fv(glGetUniformLocation(shaderNum, "modelTransform"), 1, GL_FALSE, glm::value_ptr(modeling));

	//
	// 
	unsigned int color = glGetUniformLocation(shaderNum, "objectColor");
	unsigned int ml = glGetUniformLocation(shaderNum, "modelTransform");

}

void Player::setDirZero()
{
	m_vDir = glm::vec3(0.0f);
}

void Player::setForward(glm::vec3 newFow)
{
	m_vForward = newFow;
}

void Player::moveBack(glm::vec3 backHow)
{
	this->setTranslate(m_vPivot - backHow);
}
float Player::get_angle()
{
	return angle;
}


void Player::Player_side_move(char key)
{
	glm::vec3 move = glm::vec3(0.0f);
	switch (direction)
	{
	case 1:
		if (key == 'a') move.x = -1;
		if (key == 'd') move.x = 1;
		break;
	case 3:
		if (key == 'a') move.x = 1;
		if (key == 'd') move.x = -1;
		break;
	case 0:
		if (key == 'a') move.z = 1;
		if (key == 'd') move.z = -1;
		break;
	case 2:
		if (key == 'a') move.z = -1;
		if (key == 'd') move.z = 1;
		break;
	}
	glm::vec3 gettransfrom = this->getTranslateVec();
	this->setTranslate(glm::vec3(gettransfrom.x + move.x, gettransfrom.y + move.y, gettransfrom.z + move.z));
}

void Player::getCoin()
{
	ParticleManager::GetInstance()->createParticle(this->getTranslateVec());
}