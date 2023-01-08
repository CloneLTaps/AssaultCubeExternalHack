#ifndef Draw_h
#define Draw_h

#include <Windows.h>
#include <wingdi.h>

HDC hdc; // This structure is used to obtain graphical components 
HFONT Font;
COLORREF TextCOLOR;
RECT m_Rect;

void initilize();

void DrawFilledRect(int x, int y, int w, int h, HBRUSH brushColor);

void DrawBorderBox(int x, int y, int w, int h, int thickness, HBRUSH brushColor);
	
void DrawString(int x, int y, COLORREF color, const char* text);

void DrawESP(int x, int y, float distance, int health, char name[20], HBRUSH hBrush, COLORREF Pen);

void esp();

#endif // Draw.h