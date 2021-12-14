#pragma once
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/glm/ext.hpp>
#include <vector>
#include <iostream>
#include "Singleton.h"
#include "Box.h"



class Particle
{
private:
	float own_x, own_y;
	float speed;
	float angle;
	float r, g, b, a;
	float size;
	int Ptype;

	Box* coinParticle;
	Box* PlayerDestroyParticle;
public:
	Particle(int type);
	~Particle() = default;

	void Update(float deltaTime);
	void init(int type);

	void Draw(glm::vec3 pos, int sNum, int bindTex, float ch_angle, unsigned int modelLocation, unsigned int colorLocation, int numTriangle);
};