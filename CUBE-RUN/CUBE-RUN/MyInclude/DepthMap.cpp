#include "DepthMap.h"
#include "Game.h"

DepthMap::DepthMap(unsigned int w, unsigned int h) :
	m_iW(w), m_iH(h)
{
	// generate new frame buffer
	glGenFramebuffers(1, &m_iDepthMapFBO);

	glGenTextures(1, &m_iDepthMap);
	glBindTexture(GL_TEXTURE_2D, m_iDepthMap);
	// texture type : depth component
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_iW, m_iH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	// nearest sampling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// texture addressing mode : clamp to border and border color to white
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float border[] = { 1.0f ,1.0f ,1.0f ,1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

	// attatch depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_iDepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_iDepthMap, 0);

	// dont have to care about color
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthMap::useTexMap()
{
	glViewport(0, 0, m_iW, m_iH);							// set shadow w, h
	glBindFramebuffer(GL_FRAMEBUFFER, m_iDepthMapFBO);		// bind shadow buffer
	glClear(GL_DEPTH_BUFFER_BIT);							// clear first

}

void DepthMap::bindTexture(int bindAt)
{
	glActiveTexture(GL_TEXTURE0 + bindAt);
	glBindTexture(GL_TEXTURE_2D, m_iDepthMap);
}
