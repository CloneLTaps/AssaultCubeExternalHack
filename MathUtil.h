#ifndef MathUtil_h
#define MathUtil_h

#include <Windows.h>
#include <stdbool.h>

#define PI 3.14159265359f
#define HALF_CIRCLE 180

// Using typedef so that I don't need to intilize this as 'struct Vec4 <name>' and can instead use 'Vec4 <name>'
typedef struct Vec4 
{
    float x, y, z, w;
} Vec4;

typedef struct Vec3 
{
    float x, y, z;
} Vec3;

Vec3 screen;
Vec4 clipCoords;
Vec4 NDC;

bool WorldToScreen(Vec3 pos, Vec3 *screen, float matrix[16], int windowWidth, int windowHeight);

float GetDistance3D(Vec3 m_pos, Vec3 en_pos);
float GetDistance2D(Vec3 m_pos, Vec3 en_pos);

struct Vec3 CalculateAngles(Vec3 m_pos, Vec3 en_pos);
	
void GetInfo();
void Print();

#endif // MathUtil.h