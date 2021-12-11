#pragma once
#include "Mesh.h"

class TextureClass;


class Portal :
    public Mesh
{
	TextureClass* m_pTexture;

public:
	Portal() = delete;
	Portal(float size, int faceNum, glm::vec3 translate);
	~Portal();

public:
	void draw(unsigned int shaderNum, int bindTex);

};

