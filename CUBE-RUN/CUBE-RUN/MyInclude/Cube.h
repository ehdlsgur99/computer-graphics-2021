#pragma once
#include "Mesh.h"

class TextureClass;

class Cube : public Mesh {
	TextureClass* m_pTexture;

public:
	Cube() = delete;
	Cube(const char* objFile, glm::vec3 s, glm::vec3 r, glm::vec3 t, const char* texFile);
	~Cube();

public:
	void draw(unsigned int shaderNum, int bindTex);	
};

