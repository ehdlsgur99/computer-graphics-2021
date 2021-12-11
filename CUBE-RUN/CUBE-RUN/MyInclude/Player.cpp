#include "Player.h"
#include "TextureClass.h"


glm::vec3 Player::m_vDir = glm::vec3(0.0f);
glm::vec3 Player::m_vForward = glm::vec3(0.0f, 0.0f, 1.0f);

Player::Player(float size, glm::vec3 pivot) :
	Mesh("Objs/Cube.obj", glm::vec3(1.0f)* size, glm::vec3(0.0f), (pivot)+glm::vec3(0.0f, 0.5f, 0.0f) * size),
	m_vRightRot(glm::vec3(0.0f)),
	m_vLeftRot(glm::vec3(0.0f)),
	m_vBackRot(glm::vec3(20.0f, 0.0f, 0.0f)),
	m_angle_y(glm::vec3(0.0f)),
	angle(0)
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
	glm::vec3 dir(0.0f);
	
	switch (key) {
	case 'w':	dir = m_vForward;	break;
	case 's':	dir = -m_vForward;	break;
	case 'a':	dir = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(m_vForward, 0.0f);		break;
	case 'd':	dir = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(m_vForward, 0.0f);	break;
	case 'z':
		printf("z\n");
		break;
	case 'x':
		printf("x\n");
		break;
	}

	m_vDir = glm::normalize(m_vDir + dir);
}

void Player::update(float deltaTime,glm::vec3 veye)
{
	static float fMoveSpeed = 5.0f;
	static float fRotateSpeed = -100.0f;
	static bool bIncreaseFront = true;
	static bool bIncreaseBack = false;

	

	glm::vec3 dir(0.0f);
	dir = glm::rotate(glm::mat4(1.0f), glm::radians(veye.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(m_vForward, 0.0f);
	m_vDir = glm::normalize(m_vDir + dir);

	glm::vec3 rotate = glm::vec3(1.0f, 0.0f, 0.0f);
	m_angle_y -= rotate * fRotateSpeed * deltaTime;
	if (m_angle_y.x <= 0.0f) m_angle_y.x = 360;
	this->setRotate(m_angle_y);

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
	m_pBody->draw();
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


