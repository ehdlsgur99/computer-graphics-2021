#pragma once



// for stb_image


class TextureClass {
public:
	unsigned int m_iTexture;

public:
	int m_iWidth;
	int m_iHeight;
	int m_iNumOfChannel;

public:
	TextureClass(const char* fileName);
	~TextureClass();

public:
	void bindTexture(unsigned int activeAt);
};

