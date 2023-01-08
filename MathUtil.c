#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "MathUtil.h"

bool WorldToScreen(Vec3 pos, Vec3 *screen, float matrix[16], int windowWidth, int windowHeight)
{
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f) return false;
	
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen->x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen->y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	
	return true;
}

float GetDistance3D(Vec3 m_pos, Vec3 en_pos)
{
	return (float) (sqrt(((en_pos.x - m_pos.x) * (en_pos.x - m_pos.x)) + ((en_pos.y - m_pos.y) * (en_pos.y - m_pos.y)) + ((en_pos.z - m_pos.z) * (en_pos.z - m_pos.z))));
}

float GetDistance2D(Vec3 m_pos, Vec3 en_pos)
{
	return ((float) (sqrt(((en_pos.x - m_pos.x) * (en_pos.x - m_pos.x)) + ((en_pos.z - m_pos.z) * (en_pos.z - m_pos.z)))));
}

struct Vec3 CalculateAngles(Vec3 m_pos, Vec3 en_pos)
{
	Vec3 values;

	float aTriangle = en_pos.x - m_pos.x;
	float bTriangle = en_pos.z - m_pos.z;
	float yTriangle = en_pos.y - m_pos.y;

	float triangleHyp = GetDistance2D(m_pos, en_pos);

	float yaw = -(float) (atan2(aTriangle, bTriangle) * (HALF_CIRCLE / PI) + HALF_CIRCLE);
	float pitch = (float) ((atan2(yTriangle, triangleHyp)) * (HALF_CIRCLE / PI));

	values.x = yaw;
	values.y = pitch;
	values.z = 0;
	return values;
}