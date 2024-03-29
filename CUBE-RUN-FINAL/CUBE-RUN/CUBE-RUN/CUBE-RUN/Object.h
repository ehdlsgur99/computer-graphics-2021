#pragma once

#include "common.h"


class Object {
public:
	GLuint m_iVAO = 0;
	GLuint m_iVBO;

	int m_iBufferSize;
	GLuint m_iEBO;
	GLuint* m_iElementBuffer = nullptr;

	std::vector<Vertexes> m_vVertexes;

	glm::vec3 m_vColor;

	glm::vec3 m_vPivot;
	glm::vec3 m_vRotate;
	glm::vec3 m_vScale;

	glm::mat4 m_mTranslate;
	glm::mat4 m_mRotate;
	glm::mat4 m_mScale;

	glm::mat4 m_mSRTModel = glm::mat4(1.0f);

public:
	Object() = default;

public:
	// color random
	Object(const char* objFilename, glm::vec3 scale, glm::vec3 rotate, glm::vec3 translate);
	Object(const char* objFilename, glm::vec3 scale, glm::vec3 rotate, glm::vec3 translate, glm::vec3 color);
	virtual ~Object();

protected:
	void initailizeBuffer();

private:
	void loadObject(const char* fileName);

public:
	void draw();

public:
	glm::vec3 getScaleVec() const;
	glm::vec3 getRotateVec() const;
	glm::vec3 getTranslateVec() const;

public:
	void setTranslate(glm::vec3 vOffset);
	void setScale(glm::vec3 vOffset);
	void setRotate(glm::vec3 vOffset);

private:
	void updateModelTransfrom();

public:
	glm::mat4 getModelTransform() const;
	glm::mat4 getScaleTransform() const;
	glm::mat4 getRotateTransform() const;
	glm::mat4 getTranslateTransform() const;

public:
	void uniformModelingMat(unsigned int shaderNum);
	void uniformColor(unsigned int shaderNum);

};
