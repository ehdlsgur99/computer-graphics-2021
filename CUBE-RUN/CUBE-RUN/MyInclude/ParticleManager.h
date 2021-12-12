#pragma once

#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/glm/ext.hpp>
#include <vector>
#include <iostream>
#include "Singleton.h"
#include "Particle.h"

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
	void createParticle(glm::vec3 vec);
	bool Update(float time_);
	void Draw(float angle,  int numTriangle, int sNum, int bindTex);
};