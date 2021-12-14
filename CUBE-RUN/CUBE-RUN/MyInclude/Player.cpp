#include "Player.h"
#include "TextureClass.h"
#include "ParticleManager.h"
#include "SoundManager.h"

glm::vec3 Player::m_vDir = glm::vec3(0.0f);
glm::vec3 Player::m_vForward = glm::vec3(0.0f, 0.0f, 1.0f);
float Player::angle(0);

Player::Player(float size, glm::vec3 pivot) :
	Object("Objs/Cube.obj", glm::vec3(1.0f)* size, glm::vec3(0.0f), (pivot)+glm::vec3(1.0f, 1.0f, 0.0f) * size),
	m_vRightRot(glm::vec3(0.0f)),
	m_vLeftRot(glm::vec3(0.0f)),
	m_vBackRot(glm::vec3(20.0f, 0.0f, 0.0f)),
	m_rotate_x(glm::vec3(0.0f)),
	m_rotate_z(glm::vec3(0.0f)),
	dir(glm::vec3(0.0f)),
	direction (0),
	isjump(false),
	max_jump_height(7.0f),
	org_pos(glm::vec3(0.0f)),
	jumpdir(true)
{
	m_pBody = new Object("Objs/Cube.obj", glm::vec3(1.0f) * size, glm::vec3(0.0f), pivot + glm::vec3(0.0f, 0.0f, 0.0f) * size);

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
	case VK_SPACE:
		if (isjump == false)
		{
			isjump = true;
			jumpdir = true;
			org_pos = this->getTranslateVec();
		}
		break;
	}
	
	m_vDir = glm::normalize(m_vDir + dir);
}

void Player::update(float deltaTime)
{
	static float fMoveSpeed = 5.0f;
	static float fRotateSpeed = 400.0f;
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

	if (life == true) //게임 진행중 직진 및 점프
	{
		m_vDir = m_vForward;
		glm::vec3 offset = m_vDir * fMoveSpeed * deltaTime;
		this->setTranslate(m_vPivot + offset);

		jump(deltaTime);
	}
	else if (life == false) //게임오버
	{
		GameOver(deltaTime);
	}

	
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

	unsigned int color = glGetUniformLocation(shaderNum, "objectColor");
	unsigned int ml = glGetUniformLocation(shaderNum, "modelTransform");

	Object::draw();

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
		if (key == 'a') move.x = -SIDE_MOVE_SPEED;
		if (key == 'd') move.x = SIDE_MOVE_SPEED;
		break;
	case 3:
		if (key == 'a') move.x = SIDE_MOVE_SPEED;
		if (key == 'd') move.x = -SIDE_MOVE_SPEED;
		break;
	case 0:
		if (key == 'a') move.z = SIDE_MOVE_SPEED;
		if (key == 'd') move.z = -SIDE_MOVE_SPEED;
		break;
	case 2:
		if (key == 'a') move.z = -SIDE_MOVE_SPEED;
		if (key == 'd') move.z = SIDE_MOVE_SPEED;
		break;
	}
	glm::vec3 gettransfrom = this->getTranslateVec();
	this->setTranslate(glm::vec3(gettransfrom.x + move.x, gettransfrom.y + move.y, gettransfrom.z + move.z));
}


void Player::getCoin()
{
	ParticleManager::GetInstance()->createParticle(this->getTranslateVec(),0);
}

void Player::destroyparticle()
{
	ParticleManager::GetInstance()->createParticle(this->getTranslateVec(), 1);
}

void Player::jump(float dt)
{
	static float jump_speed = 3.0f;
	static float up = 0;
	
	if (isjump == true)
	{
		if (jumpdir)
		{
			glm::vec3 pos = this->getTranslateVec();
			up += jump_speed * dt;
			this->setTranslate(glm::vec3(pos.x, pos.y + up, pos.z));

			pos = this->getTranslateVec();
			if (pos.y >= max_jump_height)
			{
				jumpdir = false;
				up = 0;
			}
		}
		else if (jumpdir == false)
		{
			glm::vec3 pos = this->getTranslateVec();
			up += jump_speed * dt;
			this->setTranslate(glm::vec3(pos.x, pos.y - up, pos.z));

			pos = this->getTranslateVec();
			if (pos.y <= org_pos.y)
			{
				this->setTranslate(glm::vec3(pos.x, org_pos.y, pos.z));
				jumpdir = true;
				isjump = false;
				up = 0;
			}
		}
	}
}

void Player::GameOver(float dt)
{
	// 큐브 정지
	// 0 = down 1 = up
	// 2 = 장외탈락
	static float fMoveSpeed = 5.0f;
	
	if (GameOverType == 0)
	{
		glm::vec3 pos = this->getTranslateVec();
		this->setTranslate(glm::vec3(pos.x, pos.y - fMoveSpeed * 2 * dt, pos.z));
	}
	else if (GameOverType == 1)
	{
		this->setTranslate(glm::vec3(0, 0, 0));
	}
}

void Player::collision(int type)
{
	if (invincibility) return; //무적상태

	life = false;
	GameOverType = type;
}

