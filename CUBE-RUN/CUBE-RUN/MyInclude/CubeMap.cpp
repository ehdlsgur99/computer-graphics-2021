#include "CubeMap.h"

#include "Game.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"


CubeMap::CubeMap(const char* fileName)
{
	stbi_set_flip_vertically_on_load(false);
	glGenTextures(1, &m_iCubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_iCubeMapTexture);

	// +x, -x, +y, -y, +z, -z order
	std::string faces[6] = {
		"right", "left", "top", "bottom", "front", "back"
	};

	// load image and attach to each face
	for (int i = 0; i < 6; ++i) {
		std::string temp = fileName;
		temp += "_" + faces[i] + ".png";

		unsigned char* data = stbi_load(temp.c_str(), &m_iWidth, &m_iHeight, &m_iNumOfChannel, 0);

		if (!data) {
			std::cerr << "Failed to load \"" << temp.c_str() << "\"!!!" << std::endl;
			exit(100);
		}
		// +x, -x, +y, -y, +z, -z order
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB + (m_iNumOfChannel == 4),
			m_iWidth, m_iHeight, 0, GL_RGB + (m_iNumOfChannel == 4), GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

CubeMap::~CubeMap()
{
}

void CubeMap::bindTexture(unsigned int activeAt = 0)
{
	glActiveTexture(GL_TEXTURE0 + activeAt);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_iCubeMapTexture);
}

