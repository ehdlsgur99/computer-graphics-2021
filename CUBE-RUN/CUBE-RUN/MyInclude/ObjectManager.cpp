#include "ObjectManager.h"
#include "Player.h"
#include "Tiles.h"

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

}

void ObjectManager::init()
{
	
}

ObjectBox ObjectManager::checkObjFile(std::string objFileName)
{
	ObjectBox obj;
	for (int i = 0; i < objBox.size(); i++)
	{
		if (strcmp(objFileName.c_str(), objBox[i].objName.c_str()) == 0)
		{
			return objBox[i];
		}
	}
	return obj;
}

void ObjectManager::addObjtoBox(std::string objName, GLuint VAO, GLuint VBO, std::vector<Vertexes> v)
{
	ObjectBox box;
	box.objName = objName;
	box.VAO = VAO;
	box.VBO = VBO;
	box.m_vVertexes.resize((int)(v.size()));
	std::copy(v.begin(), v.end(), box.m_vVertexes.begin());

	objBox.push_back(box);
}

void ObjectManager::addTextureBox(std::string textureName, unsigned char* pImageData, int m_iWidth, int m_iHeight, int	m_iNumOfChannel)
{
	TextureBox box;
	box.textureName = textureName;
	box.pImageData = pImageData;
	box.m_iHeight = m_iHeight;
	box.m_iWidth = m_iWidth;
	box.m_iNumOfChannel = m_iNumOfChannel;

	textureBox.push_back(box);
}

TextureBox ObjectManager::checktextureFile(std::string textureFileName)
{
	TextureBox texture;
	for (int i = 0; i < textureBox.size(); i++)
	{
		if (strcmp(textureFileName.c_str(), textureBox[i].textureName.c_str()) == 0)
		{
			return textureBox[i];
		}
	}
	return texture;
}