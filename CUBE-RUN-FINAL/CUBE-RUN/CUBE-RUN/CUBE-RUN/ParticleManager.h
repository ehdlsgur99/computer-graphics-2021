#pragma once


#include "common.h"

class Particle;
class ParticleManager :public CSingleton<ParticleManager>
{
public:
	glm::vec3 pos;
	float time;	// ���� �ð� ������ �����
	std::vector<Particle> p;
	bool isVisible;

public:
	ParticleManager();
	~ParticleManager();
	void createParticle(glm::vec3 vec,int type);
	bool Update(float time_);
	void Draw(float angle,  int numTriangle, int sNum, int bindTex);
};