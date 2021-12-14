#pragma once

#include "common.h"

class TextureClass;

class Box : public Object {
public:
	TextureClass* m_pTexture;

public:
	Box() = delete;
	Box(const char* objFile, glm::vec3 s, glm::vec3 r, glm::vec3 t, const char* texFile);
	~Box();

public:
	void draw(unsigned int shaderNum, int bindTex);	
};

