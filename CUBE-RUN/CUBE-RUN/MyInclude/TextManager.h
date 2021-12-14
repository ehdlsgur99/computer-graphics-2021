#pragma once



#include "common.h"

#define DEFAULTFONTSIZE 20

enum FONT
{
	ARIAL = 1,      // Arial = 1
	COURIERNEW,     // Courier New = 2
	CALIBRI,        // Calibri = 3
	TIMESNEWROMAN,  // Times New Roman = 4
	BOOKANTIQUA,    // Book Antiqua = 5
	TREBUCHETMS     // Trebuchet MS = 6
};

//std::string hepler[] = { "<-,-> press player move!","Space Player Jump!","z,x " };
class TextManager : public CSingleton<TextManager>
{
public:
	TextManager() {};
	~TextManager() {};
public:
	HDC hDC;              // Private GDI Device Context
	HGLRC hRC;            // Permanent Rendering Context
	HWND hWnd;            // Holds Our Window Handle

	GLuint  base;  // Base Display List For The Font Set

	GLvoid BuildFontWithEnum(FONT fontEnum, int fontSize);
	GLvoid BuildFont(LPCWSTR fontName, int fontSize);
	GLvoid KillFont(GLvoid);
	GLvoid glPrint(const char* fmt, ...);
	int InitFont(GLvoid);

};

