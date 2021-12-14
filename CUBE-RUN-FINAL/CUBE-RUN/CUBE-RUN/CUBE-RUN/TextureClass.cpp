

#include "common.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"

TextureClass::TextureClass(const char* fileName) 
{
	unsigned char* pImageData;
	TextureBox textureBox = ObjectManager::GetInstance()->checktextureFile(fileName);
	if (strcmp(textureBox.textureName.c_str(), fileName) != 0)
	{
		stbi_set_flip_vertically_on_load(true);
		pImageData = stbi_load(fileName, &m_iWidth, &m_iHeight, &m_iNumOfChannel, 0);

		if (!pImageData) {
			std::cerr << "Failed to load \"" << fileName << "\"!!!" << std::endl;
			exit(100);
		}

		glGenTextures(1, &m_iTexture);
		glBindTexture(GL_TEXTURE_2D, m_iTexture);

		// texture addressing mode
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// linear sampling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// texture type : rgb
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB + (m_iNumOfChannel == 4), m_iWidth, m_iHeight, 0,
			GL_RGB + (m_iNumOfChannel == 4), GL_UNSIGNED_BYTE, pImageData);

		textureBox.pImageData = pImageData;

		ObjectManager::GetInstance()->addTextureBox(fileName, pImageData, m_iWidth, m_iHeight, m_iNumOfChannel);
		//stbi_image_free(pImageData);
	}
	else
	{
		pImageData = textureBox.pImageData;
		m_iNumOfChannel = textureBox.m_iNumOfChannel;
		m_iWidth = textureBox.m_iWidth;
		m_iHeight = textureBox.m_iHeight;

		glGenTextures(1, &m_iTexture);
		glBindTexture(GL_TEXTURE_2D, m_iTexture);

		// texture addressing mode
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// linear sampling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// texture type : rgb
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB + (m_iNumOfChannel == 4), m_iWidth, m_iHeight, 0,
			GL_RGB + (m_iNumOfChannel == 4), GL_UNSIGNED_BYTE, pImageData);

		//stbi_image_free(pImageData);
	}

	
}

TextureClass::~TextureClass() {}

void TextureClass::bindTexture(unsigned int activeAt = 0) 
{
	glActiveTexture(GL_TEXTURE0 + activeAt);
	glBindTexture(GL_TEXTURE_2D, m_iTexture);
}

