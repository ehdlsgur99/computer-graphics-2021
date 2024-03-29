


#include "common.h"


GLvoid TextManager::BuildFontWithEnum(FONT fontEnum, int fontSize)
{
    LPCWSTR str = L"Arial";
    switch (fontEnum)
    {
    case ARIAL:
        str = L"Arial";
        BuildFont(str, fontSize);
        break;
    //case COURIERNEW:
    //    str = L"Courier New";
    //    BuildFont("Courier New", fontSize);
    //    break;
    //case CALIBRI:
    //    str = L"Courier New";
    //    BuildFont("Calibri", fontSize);
    //    break;
    //case TIMESNEWROMAN:
    //    str = L"Courier New";
    //    BuildFont(L"Times New Roman", fontSize);
    //    break;
    //case BOOKANTIQUA:
    //    BuildFont("Book Antiqua", fontSize);
    //    break;
    //case TREBUCHETMS:
    //    BuildFont("Book Antiqua", fontSize);
    //    break;
    }
}

TextManager::TextManager()
{
    helper.push_back("      <-,-> player move!");
    helper.push_back("      Space Player Jump!");
    helper.push_back("      z,x Player Rotate!");
    helper.push_back("      GOOD LUCK!");
    nowHelperIndex = -1;
    helperTime = GetTickCount();
}

/** 새로운 폰트를 생성하는 함수입니다.
 *  char* fontName: 폰트 이름
 *  int fontSize: 폰트 크기
 */
GLvoid TextManager::BuildFont(LPCWSTR fontName, int fontSize)
{
    HFONT   font;     // Windows Font ID
    HFONT   oldfont;  // Used For Good House Keeping

    base = glGenLists(96);  // Storage For 96 Characters

    font = CreateFont(-fontSize, // Height Of Fonts
        0,              // Width Of Font
        0,              // Angle Of Escapement
        0,              // Orientation Angle
        FW_BOLD,        // Font Weight
        FALSE,          // Italic     (취소선)
        FALSE,          // Underline (밑줄)
        FALSE,          // Strikeout (취소선)
        ANSI_CHARSET,   // Character Set Identifier
        OUT_TT_PRECIS,  // Output Precision
        CLIP_DEFAULT_PRECIS,        // Clipping Precision
        ANTIALIASED_QUALITY,        // Output Quality
        FF_DONTCARE | DEFAULT_PITCH,  // Family And Pitch
        fontName);         // Font Name

    oldfont = (HFONT)SelectObject(hDC, font); // Selects The Font We Want
    wglUseFontBitmaps(hDC, 32, 96, base);     // Builds 96 Characters Starting At Character 32
    SelectObject(hDC, oldfont);               // Selects The Font We Want
    DeleteObject(font);                       // Delete The Font
}

GLvoid TextManager::KillFont(GLvoid)                     // Delete The Font List
{
    glDeleteLists(base, 96);                // Delete All 96 Characters
}

GLvoid TextManager::glPrint(const char* fmt, ...)                // Custom GL "Print" Routine
{
    char        text[256];          // Holds Our String
    va_list     ap;                 // Pointer To List Of Arguments

    if (fmt == NULL)                // If There's No Text
        return;                     // Do Nothing

    va_start(ap, fmt);              // Parses The String For Variables
    vsprintf(text, fmt, ap);    // And Converts Symbols To Actual Numbers
    va_end(ap);                     // Results Are Stored In Text

    glPushAttrib(GL_LIST_BIT);              // Pushes The Display List Bits
    glListBase(base - 32);                  // Sets The Base Character to 32

    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);  // Draws The Display List Text
    glPopAttrib();                      // Pops The Display List Bits
}

int TextManager::InitFont(GLvoid)                      // All Setup For OpenGL Goes Here
{
    glShadeModel(GL_SMOOTH);                // Enable Smooth Shading
    // glClearColor(0.0f, 0.0f, 0.0f, 0.5f);           // Black Background
    // glClearDepth(1.0f);                 // Depth Buffer Setup
    // glEnable(GL_DEPTH_TEST);                // Enables Depth Testing
    // glDepthFunc(GL_LEQUAL);                 // The Type Of Depth Testing To Do
    // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations

    hDC = wglGetCurrentDC();            // 현재 openGL 윈도우의 hDC를 가져온다.

    BuildFontWithEnum(ARIAL, DEFAULTFONTSIZE);       // Build The Font

    return TRUE;                        // Initialization Went OK
}


void TextManager::drawTutorial()
{
    if (nowHelperIndex == -1)
    {
        helperTime = GetTickCount();
        nowHelperIndex += 1;
    }

    if (GetTickCount() - helperTime >= 3000.0f )
    {
        if (nowHelperIndex < helper.size() - 1)
        {
            helperTime = GetTickCount();
            nowHelperIndex += 1;  
        }
    }
    if (nowHelperIndex != -1)
    {
        glColor3f(1, 0, 0);
        glRasterPos3f(0.0, 0.0, 0.0);
        glPrint(helper[nowHelperIndex].c_str());
    }
 
}