#pragma once
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/glm/ext.hpp>
#include <vector>
#include <iostream>
#include "Singleton.h"



class Particle
{
private:
	float own_x, own_y;
	float speed;
	float angle;
	float r, g, b, a;
	float size;

	GLuint vao;
	GLuint vbo[2];
public:
	Particle();
	~Particle() = default;

	void Update(float deltaTime);

	void Draw(glm::vec3 pos, float ch_angle, unsigned int modelLocation, unsigned int colorLocation, int numTriangle);
};