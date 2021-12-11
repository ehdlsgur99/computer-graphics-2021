#pragma once
#include "Mesh.h"

class TextureClass;

class Plane : public Mesh {
	TextureClass* m_pTexture;

public:
	Plane() = delete;
	Plane(const char* objFile, glm::vec3 s, glm::vec3 r, glm::vec3 t, const char* texFile);
	~Plane();

public:
	void draw(unsigned int shaderNum, int bindTex);	
};

