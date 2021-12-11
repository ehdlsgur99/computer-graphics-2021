#pragma once


class DepthMap {
private:
	unsigned int m_iDepthMapFBO;
	unsigned int m_iDepthMap;

	unsigned int m_iW = 1024;
	unsigned int m_iH = 1024;

public:
	DepthMap(unsigned int w, unsigned int h);

	void useTexMap();

	void bindTexture(int bindAt = 0);
};
