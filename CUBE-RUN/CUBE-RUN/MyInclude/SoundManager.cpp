#include "common.h"

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{

}

void SoundManager::PlayBg(std::string path)
{
	std::string str;
	str += "play ";
	str += path;
	str += " repeat";

	// �ڷ��� ��ȯ ������� ;;;
	// string to tchar ��..
	int slength = (int)str.length() + 1;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
	TCHAR* buf = new TCHAR[len];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
	TCHAR* r(buf);


	mciSendString(r, 0, 0, 0);
}

void SoundManager::PlayEf(std::string path)
{

	// �ڷ��� ��ȯ ������� ;;;
	// string to tchar ��..
	int slength = (int)path.length() + 1;
	int len = MultiByteToWideChar(CP_ACP, 0, path.c_str(), slength, 0, 0);
	TCHAR* buf = new TCHAR[len];
	MultiByteToWideChar(CP_ACP, 0, path.c_str(), slength, buf, len);
	TCHAR* r(buf);

	// ȿ���� ���� ���
	sndPlaySound(r, SND_ASYNC);
}
