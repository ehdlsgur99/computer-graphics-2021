#include "Cube.h"
#include "TextureClass.h"

Cube::Cube(const char* objFile, glm::vec3 s, glm::vec3 r, glm::vec3 t, const char* texFile) :
	Mesh(objFile, s, r, t)
{
	m_pTexture = new TextureClass(texFile);
}

Cube::~Cube()
{
	delete m_pTexture;
}

void Cube::draw(unsigned int shaderNum, int bindTex)
{
	m_pTexture->bindTexture(bindTex);
	this->uniformModelingMat(shaderNum);
	Mesh::draw();
}

