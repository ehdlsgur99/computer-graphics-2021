#include "RotatingCube.h"
#include "TextureClass.h"

RotatingCube::RotatingCube(float size, glm::vec3 pivot) :
	Mesh("Objs/Cube.obj", glm::vec3(size), glm::vec3(90.0f, 45.0f, 45.0f), pivot + glm::vec3(0.0f, glm::sqrt(3.0f) * size / 2, 0.0f))
{
	//m_pTextures = new TextureClass("");
}

void RotatingCube::update(float deltaTime)
{
	static float rotateSpeed = 60.0f;
	m_fRotateDegree += rotateSpeed * deltaTime;
}

void RotatingCube::draw(unsigned int shaderNum, int textureBind)
{
	glm::mat4 modeling = this->m_mTranslate *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_fRotateDegree), glm::vec3(0.0f, 1.0f, 0.0f)) *
		this->m_mRotate * this->m_mScale;

	unsigned int modelLocation = glGetUniformLocation(shaderNum, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modeling));
	Mesh::draw();
}

glm::vec3 RotatingCube::getSize() const
{
	return m_vScale * sqrt(3.0f);
}
