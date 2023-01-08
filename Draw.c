#include <Windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <wingdi.h>
#include "Draw.h"
#include "Addresses.h"
#include "MathUtil.h"
#include "Main.h"

static HBRUSH hBrushEnemy;
static HBRUSH hBrushTeam;
static HBRUSH hBrushNeutral;
static COLORREF enemyColor;
static COLORREF teamColor;
static COLORREF neutralColor;

void initilize()
{
    hBrushEnemy = CreateSolidBrush(RGB(255, 0, 0));
    hBrushTeam = CreateSolidBrush(RGB(0, 0, 255));
    hBrushNeutral = CreateSolidBrush(RGB(255, 255, 255));
	enemyColor = RGB(255, 0, 0);
	teamColor = RGB(0, 0, 255);
	neutralColor = RGB(255, 255, 255);
	TextCOLOR = RGB(0, 255, 0);
}

void DrawFilledRect(int x, int y, int w, int h, HBRUSH brushColor)
{
	RECT rect = { x, y, x + w, y + h };
	FillRect(hdc, &rect, brushColor);
}

void DrawBorderBox(int x, int y, int w, int h, int thickness, HBRUSH brushColor)
{
	DrawFilledRect(x, y, w, thickness, brushColor);
	DrawFilledRect(x, y, thickness, h, brushColor);
	DrawFilledRect((x + w), y, thickness, h, brushColor);
	DrawFilledRect(x, y + h, w + thickness, thickness, brushColor);
}

void DrawString(int x, int y, COLORREF color, const char* text)
{
	SetTextAlign(hdc, TA_CENTER | TA_NOUPDATECP);
	SetBkColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, color);
	SelectObject(hdc, Font);
	TextOutA(hdc, x, y, text, strlen(text));
	DeleteObject(Font);
}

void DrawESP(int x, int y, float distance, int health, char name[20], HBRUSH hBrush, COLORREF Pen)
{
	int width = 1600 / distance;	

	int height = 5000 / distance;	

	DrawBorderBox(x - (width / 2), y - height, width, height, 1, hBrush);

	char healthChar[4];
	char distanceChar[4];
	sprintf(distanceChar, "%f", distance);
	sprintf(healthChar, "%i", health);
}

void esp()
{
	for (int i = 0; i < entityCount; i++) 
	{
		bool world = WorldToScreen(entityArray[i]->vector, &screen, pData.matrix, width, height);
		if (world && entityArray[i]->health > 0) {
			if (true) { 
				const COLORREF colorOfTeam = entityArray[i]->entityTeam == pData.team ? teamColor : enemyColor;
				const HBRUSH brushOfTeam = entityArray[i]->entityTeam == pData.team ? hBrushTeam : hBrushEnemy;
				DrawESP(screen.x, screen.y, GetDistance3D(pData.pos, entityArray[i]->vector), entityArray[i]->health, entityArray[i]->name, brushOfTeam, colorOfTeam);
			}
		}
	}
} 