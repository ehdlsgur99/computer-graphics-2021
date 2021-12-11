#pragma once



// for stb_image


class TextureClass {
private:
	unsigned int m_iTexture;

private:
	int m_iWidth;
	int m_iHeight;
	int m_iNumOfChannel;

public:
	TextureClass(const char* fileName);
	~TextureClass();

public:
	void bindTexture(unsigned int activeAt);
};

