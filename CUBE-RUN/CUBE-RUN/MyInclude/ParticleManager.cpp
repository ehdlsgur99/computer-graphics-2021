#include "ParticleManager.h"

ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{

}

void ParticleManager::createParticle(glm::vec3 vec)
{
	isVisible = true;
	pos = vec;
	time = 0.0;
	if (p.size() > 0)
	{
		for (int i = 0; i < 100; ++i)
		{
			p[i].init();
		}
	}
	else
	{
		for (int i = 0; i < 100; ++i)
		{
			p.push_back(Particle());
		}
	}


}

bool ParticleManager::Update(float time_)
{
	time += time_;
	if (time > 2.0)
	{
		isVisible = false;
		return true;
	}
	if (isVisible == true)
	{
		for (int i = 0; i < p.size(); ++i)
		{
			p[i].Update(time_);
		}
	}
	return false;
}

void ParticleManager::Draw(float angle, int numTriangle, int sNum, int bindTex)
{
	unsigned int modelLocation = glGetUniformLocation(3, "modelTransform");
	unsigned int colorLocation = glGetUniformLocation(3, "objectColor");
	if (isVisible == true)
	{
		for (int i = 0; i < p.size(); ++i)
		{
			p[i].Draw(this->pos, sNum, bindTex, angle, modelLocation, colorLocation, numTriangle);
		}
	}

}