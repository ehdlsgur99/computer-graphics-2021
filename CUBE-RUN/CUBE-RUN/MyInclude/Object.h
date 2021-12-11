#pragma once

#include "Mesh.h"

// Abstract class
class Object : Mesh {
protected:
	glm::vec3 m_vPivot;

public:
	virtual ~Object() = 0;

public:
	virtual void update() = 0;
	virtual void draw(unsigned int shaderNum) = 0;
};

