/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector2D.c
Purpose: header file for vector functions
Language: c; gcc
Platform: Windows gcc 8.1.0
Project: dylan.washburne, CS529_project1_1
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: Septeber 9, 2020
- End Header --------------------------------------------------------*/

#include "Vector2D.h"

#define EPSILON 0.0001

// ---------------------------------------------------------------------------

void Vector2DZero(Vector2D* pResult)
{
	pResult->x = 0.0f;
	pResult->y = 0.0f;
}

// ---------------------------------------------------------------------------

void Vector2DSet(Vector2D* pResult, float x, float y)
{
	pResult->x = x;
	pResult->y = y;
}

// ---------------------------------------------------------------------------

void Vector2DNeg(Vector2D* pResult, Vector2D* pVec0)
{
	pResult->x = -pVec0->x;
	pResult->y = -pVec0->y;
}

// ---------------------------------------------------------------------------

void Vector2DAdd(Vector2D* pResult, Vector2D* pVec0, Vector2D* pVec1)
{
	pResult->x = pVec0->x + pVec1->x;
	pResult->y = pVec0->y + pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DSub(Vector2D* pResult, Vector2D* pVec0, Vector2D* pVec1)
{
	pResult->x = pVec0->x - pVec1->x;
	pResult->y = pVec0->y - pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DNormalize(Vector2D* pResult, Vector2D* pVec0)
{
	float len = sqrtf((pVec0->x * pVec0->x) + (pVec0->y * pVec0->y));
	pResult->x = pVec0->x / len;
	pResult->y = pVec0->y / len;
}

// ---------------------------------------------------------------------------

void Vector2DScale(Vector2D* pResult, Vector2D* pVec0, float c)
{
	pResult->x = pVec0->x * c;
	pResult->y = pVec0->y * c;
}

// ---------------------------------------------------------------------------

void Vector2DScaleAdd(Vector2D* pResult, Vector2D* pVec0, Vector2D* pVec1, float c)
{
	pResult->x = (pVec0->x * c) + pVec1->x;
	pResult->y = (pVec0->y * c) + pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DScaleSub(Vector2D* pResult, Vector2D* pVec0, Vector2D* pVec1, float c)
{
	pResult->x = (pVec0->x * c) - pVec1->x;
	pResult->y = (pVec0->y * c) - pVec1->y;
}

// ---------------------------------------------------------------------------

float Vector2DLength(Vector2D* pVec0)
{
	return sqrtf((pVec0->x * pVec0->x) + (pVec0->y * pVec0->y));
	// return 1.0f;
}

// ---------------------------------------------------------------------------

float Vector2DSquareLength(Vector2D* pVec0)
{
	return (pVec0->x * pVec0->x) + (pVec0->y * pVec0->y);
	// return 1.0f;
}

// ---------------------------------------------------------------------------

float Vector2DDistance(Vector2D* pVec0, Vector2D* pVec1)
{
	return sqrtf(powf(pVec0->x - pVec1->x, 2) + powf(pVec0->y - pVec1->y, 2));
	// return 1.0f;
}

// ---------------------------------------------------------------------------

float Vector2DSquareDistance(Vector2D* pVec0, Vector2D* pVec1)
{
	return powf(pVec0->x - pVec1->x, 2) + powf(pVec0->y - pVec1->y, 2);
	// return -1.0f;
}

// ---------------------------------------------------------------------------

float Vector2DDotProduct(Vector2D* pVec0, Vector2D* pVec1)
{
	return (pVec0->x * pVec1->x) + (pVec0->y * pVec1->y);
	// return -1.0f;
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleDeg(Vector2D* pResult, float angle)
{
	//   360 / (2*pi) = 57.2957795131
	pResult->x = cos(angle * 57.2957795131);
	pResult->y = sin(angle * 57.2957795131);
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleRad(Vector2D* pResult, float angle)
{
	pResult->x = cos(angle);
	pResult->y = sin(angle);
}

// ---------------------------------------------------------------------------
