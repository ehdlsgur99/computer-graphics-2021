#pragma once


// for stb_image


class CubeMap {
private:
	unsigned int m_iCubeMapTexture;

private:
	int m_iWidth;
	int m_iHeight;
	int m_iNumOfChannel;

public:
	CubeMap(const char* fileName);
	~CubeMap();

public:
	void bindTexture(unsigned int activeAt);
};

