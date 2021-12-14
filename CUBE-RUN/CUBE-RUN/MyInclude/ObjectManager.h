#pragma once

#include "common.h"
#include "Types.h"

struct ObjectBox
{
	std::string objName = "None";
	GLuint VAO;
	GLuint VBO;
	std::vector<Vertexes> m_vVertexes;
};

struct TextureBox
{
	std::string textureName = "None";
	unsigned char* pImageData = nullptr;
	int m_iWidth;
	int	m_iHeight;
	int	m_iNumOfChannel;
};

class ObjectManager : public CSingleton<ObjectManager>
{
public:
	ObjectManager();
	~ObjectManager();
public:
	std::vector<ObjectBox> objBox;
	std::vector<TextureBox> textureBox;

	void init();
	ObjectBox checkObjFile(std::string objFileName);
	TextureBox checktextureFile(std::string textureFileName);
	void addObjtoBox(std::string objName, GLuint VAO, GLuint VBO, std::vector<Vertexes> v);
	void addTextureBox(std::string textureName, unsigned char* pImageData, int m_iWidth, int m_iHeight,int	m_iNumOfChannel);
};


