#pragma once
#include "Mesh.h"

class TextureClass;

class RotatingCube :
	public Mesh {
private:
	TextureClass* m_pTextures;
	float m_fRotateDegree = 0.0f;

public:
	RotatingCube(float size, glm::vec3 pivot);

public:
	void update(float deltaTime);
	void draw(unsigned int shaderNum, int textureBind);

	glm::vec3 getSize() const;
};

