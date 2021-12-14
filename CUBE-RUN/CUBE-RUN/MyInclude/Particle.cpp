#include "Particle.h"

GLfloat cube[36][3] = { //--- ���ؽ� �Ӽ�: ��ǥ��(FragPos), �븻�� (Normal)
	{-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f},
	{0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f },
	-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f
};

GLfloat cube_normal[36][3] = { //--- ���ؽ� �Ӽ�: ��ǥ��(FragPos), �븻�� (Normal)
0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
};

Particle::Particle(int type)
{
	own_x = 0;
	own_y = 0; 
	speed = (float)(rand() % 10000) / (float)(10000) * -0.8 + 1.0;
	angle = (float)(rand() % 10000) / (float)(10000) * 360.0;
	r = (float)(rand() % 10000) / (float)(10000);
	g = (float)(rand() % 10000) / (float)(10000);
	b = (float)(rand() % 10000) / (float)(10000);
	if(type == 0) size = 0.05;
	if(type == 1) size = 0.15;
	Ptype = type;

	
	coinParticle = new Box("Objs/cube.obj", glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), "Texture/300coin.png");
	PlayerDestroyParticle = new Box("Objs/cube.obj", glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), "Texture/Player/player_texture.png");

}

void Particle::init(int type)
{
	own_x = 0;
	own_y = 0;
	speed = (float)(rand() % 10000) / (float)(10000) * -0.8 + 1.0;
	angle = (float)(rand() % 10000) / (float)(10000) * 360.0;
	r = (float)(rand() % 10000) / (float)(10000);
	g = (float)(rand() % 10000) / (float)(10000);
	b = (float)(rand() % 10000) / (float)(10000);
	if (type == 0) size = 0.05;
	if (type == 1) size = 0.15;
	Ptype = type;
}

void Particle::Update(float deltaTime)
{
	own_x += deltaTime * speed * cos(angle);
	own_y += deltaTime * speed * sin(angle);
}

void Particle::Draw(glm::vec3 pos, int sNum, int bindTex,float ch_angle, unsigned int modelLocation, unsigned int colorLocation, int numTriangle)
{
	glm::mat4 Si = glm::scale(glm::mat4(1.0f), glm::vec3(size, size, size));
	glm::mat4 Tr = glm::translate(glm::mat4(1.0f), pos);
	glm::mat4 Rt = glm::rotate(glm::mat4(1.0f), glm::radians(float(-ch_angle)), glm::vec3(1.0, 0.0, 0.0))
		* glm::translate(glm::mat4(1.0f), glm::vec3(own_x, own_y, 0));
	glm::mat4 End = Tr * Rt * Si;
	//glm::mat4 Light = Tr * Si;

	if (Ptype == 0)
	{
		coinParticle->m_mSRTModel = End;
		coinParticle->draw(sNum, bindTex);
	}
	else if (Ptype == 1)
	{
		PlayerDestroyParticle->m_mSRTModel = End;
		PlayerDestroyParticle->draw(sNum, bindTex);
	}
}