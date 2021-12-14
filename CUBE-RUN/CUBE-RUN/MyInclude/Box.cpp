#include "Box.h"
#include "TextureClass.h"

Box::Box(const char* objFile, glm::vec3 s, glm::vec3 r, glm::vec3 t, const char* texFile) :
	Object(objFile, s, r, t)
{
	m_pTexture = new TextureClass(texFile);
}

Box::~Box()
{
	delete m_pTexture;
}

void Box::draw(unsigned int shaderNum, int bindTex)
{
	m_pTexture->bindTexture(bindTex);
	this->uniformModelingMat(shaderNum);
	Object::draw();
}

