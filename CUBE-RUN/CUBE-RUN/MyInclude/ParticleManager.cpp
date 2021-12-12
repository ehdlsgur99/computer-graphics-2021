#include "ParticleManager.h"

ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{

}

void ParticleManager::createParticle(glm::vec3 vec)
{
	pos = vec;
	time = 0.0;
	for (int i = 0; i < 100; ++i)
	{
		p.push_back(Particle());
	}

}

bool ParticleManager::Update(float time_)
{
	time += time_;
	if (time > 2.0)
	{
		p.clear();
		return true;
	}

	for (int i = 0; i < p.size(); ++i)
	{
		p[i].Update(time_);
	}

	return false;
}

void ParticleManager::Draw(float angle, int numTriangle)
{
	unsigned int modelLocation = glGetUniformLocation(3, "modelTransform");
	unsigned int colorLocation = glGetUniformLocation(3, "objectColor");
	for (int i = 0; i < p.size(); ++i)
	{
		p[i].Draw(this->pos, angle, modelLocation, colorLocation, numTriangle);
	}
}