#include "Plane.h"
#include "TextureClass.h"

Plane::Plane(const char* objFile, glm::vec3 s, glm::vec3 r, glm::vec3 t, const char* texFile) :
	Mesh(objFile, s, r, t)
{
	m_pTexture = new TextureClass(texFile);
}

Plane::~Plane()
{
	delete m_pTexture;
}

void Plane::draw(unsigned int shaderNum, int bindTex)
{
	m_pTexture->bindTexture(bindTex);
	this->uniformModelingMat(shaderNum);
	Mesh::draw();
}

