#pragma once

#include "theIncludes.h"

class ShaderProgram {
private:
	unsigned int m_iProgram;

public:
	ShaderProgram() = delete;
	ShaderProgram(const char* vertexFile, const char* fragmentFile);
	~ShaderProgram();

private:
	void getFileFrom(const char* fileName, std::string& target);

public:
	unsigned int getProgram() const;
	void use();

public:
	void setMat4(const char* varName, glm::mat4 matrix);
	void setVec3(const char* varName, glm::vec3 vector);

};
