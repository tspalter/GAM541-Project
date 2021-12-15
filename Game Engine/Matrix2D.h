/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix2D.h
Purpose: header file for vector functions
Language: c; gcc
Platform: Windows gcc 8.1.0
Project: dylan.washburne, CS529_project1_1
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: Septeber 9, 2020
- End Header --------------------------------------------------------*/

#pragma once


#include "Vector2D.h"

typedef struct Matrix2D
{
	float m[16];
}Matrix2D;


/*
This function sets the matrix Result to the identity matrix
*/
void Matrix2DIdentity(Matrix2D* pResult);

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
// void Matrix2DTranspose(Matrix2D* pResult, Matrix2D* pMtx);

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D* pResult, Matrix2D* pMtx0, Matrix2D* pMtx1);

/*
This function creates a translation matrix from x & y and saves it in Result
*/
void Matrix2DTranslate(Matrix2D* pResult, float x, float y);

/*
This function creates a scaling matrix from x & y and saves it in Result
*/
void Matrix2DScale(Matrix2D* pResult, float x, float y);

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
// void Matrix2DRotDeg(Matrix2D* pResult, float Angle);

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix2DRotRad(Matrix2D* pResult, float Angle);

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
// void Matrix2DMultVec(Vector2D* pResult, Matrix2D* pMtx, Vector2D* pVec);


