/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix2D.c
Purpose: header file for vector functions
Language: c; gcc
Platform: Windows gcc 8.1.0
Project: dylan.washburne, CS529_project1_1
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: Septeber 9, 2020
- End Header --------------------------------------------------------*/

#include "Matrix2D.h"


/*
This function sets the matrix Result to the identity matrix
*/
void Matrix2DIdentity(Matrix2D* pResult)
{    
    for (int i = 0; i < 16; i++) {
        pResult->m[i] = 0.0f;
    }
    pResult->m[0] = pResult->m[5] = pResult->m[10] = pResult->m[15] = 1.0f;

}

// ---------------------------------------------------------------------------

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
/*
void Matrix2DTranspose(Matrix2D* pResult, Matrix2D* pMtx)
{
    pResult->m[0][0] = pMtx->m[0][0];
    pResult->m[1][0] = pMtx->m[0][1];
    pResult->m[2][0] = pMtx->m[0][2];

    pResult->m[0][1] = pMtx->m[1][0];
    pResult->m[1][1] = pMtx->m[1][1];
    pResult->m[2][1] = pMtx->m[1][2];

    pResult->m[0][2] = pMtx->m[2][0];
    pResult->m[1][2] = pMtx->m[2][1];
    pResult->m[2][2] = pMtx->m[2][2];

}
*/

// ---------------------------------------------------------------------------

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D* pResult, Matrix2D* pMtx0, Matrix2D* pMtx1)
{
    Matrix2D output;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int v = 0; v < 4; v++) {
                output.m[(4 * i) + j] = 0;
                output.m[(4 * i) + j] += pMtx0->m[4*i + v] * pMtx1->m[(4 * v) + i];
            }
        }
    }
    /*
    output.m[0][0] = pMtx0->m[0][0] * pMtx1->m[0][0] + pMtx0->m[0][1] * pMtx1->m[1][0] + pMtx0->m[0][2] * pMtx1->m[2][0];
    output.m[0][1] = pMtx0->m[0][0] * pMtx1->m[0][1] + pMtx0->m[0][1] * pMtx1->m[1][1] + pMtx0->m[0][2] * pMtx1->m[2][1];
    output.m[0][2] = pMtx0->m[0][0] * pMtx1->m[0][2] + pMtx0->m[0][1] * pMtx1->m[1][2] + pMtx0->m[0][2] * pMtx1->m[2][2];

    output.m[1][0] = pMtx0->m[1][0] * pMtx1->m[0][0] + pMtx0->m[1][1] * pMtx1->m[1][0] + pMtx0->m[1][2] * pMtx1->m[2][0];
    output.m[1][1] = pMtx0->m[1][0] * pMtx1->m[0][1] + pMtx0->m[1][1] * pMtx1->m[1][1] + pMtx0->m[1][2] * pMtx1->m[2][1];
    output.m[1][2] = pMtx0->m[1][0] * pMtx1->m[0][2] + pMtx0->m[1][1] * pMtx1->m[1][2] + pMtx0->m[1][2] * pMtx1->m[2][2];

    output.m[2][0] = pMtx0->m[2][0] * pMtx1->m[0][0] + pMtx0->m[2][1] * pMtx1->m[1][0] + pMtx0->m[2][2] * pMtx1->m[2][0];
    output.m[2][1] = pMtx0->m[2][0] * pMtx1->m[0][1] + pMtx0->m[2][1] * pMtx1->m[1][1] + pMtx0->m[2][2] * pMtx1->m[2][1];
    output.m[2][2] = pMtx0->m[2][0] * pMtx1->m[0][2] + pMtx0->m[2][1] * pMtx1->m[1][2] + pMtx0->m[2][2] * pMtx1->m[2][2];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            pResult->m[i][j] = output.m[i][j];
            */

    for (int i = 0; i < 16; i++) {
        pResult->m[i] = output.m[i];
    }
    // pResult->m[0] = 3;
    // pResult->m[5] = 3;


}

// ---------------------------------------------------------------------------

/*
This function creates a translation matrix from x & y and saves it in Result
*/
void Matrix2DTranslate(Matrix2D* pResult, float x, float y)
{
    Matrix2DIdentity(pResult);
    pResult->m[12] = x;
    pResult->m[13] = y;

}

// ---------------------------------------------------------------------------

/*
This function creates a scaling matrix from x & y and saves it in Result
*/
void Matrix2DScale(Matrix2D* pResult, float x, float y)
{
    Matrix2DIdentity(pResult);
    pResult->m[0] = x;
    pResult->m[5] = y;

}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
/*
void Matrix2DRotDeg(Matrix2D* pResult, float Angle)
{
    //   360 / (2*pi) = 57.2957795131
    pResult->m[0][0] = cos(Angle / 57.2957795131);
    pResult->m[0][1] = -sin(Angle / 57.2957795131);
    pResult->m[0][2] = 0;

    pResult->m[1][0] = sin(Angle / 57.2957795131);
    pResult->m[1][1] = cos(Angle / 57.2957795131);
    pResult->m[1][2] = 0;

    pResult->m[2][0] = 0;
    pResult->m[2][1] = 0;
    pResult->m[2][2] = 1;

}
*/
// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix2DRotRad(Matrix2D* pResult, float Angle)
{
    Matrix2DIdentity(pResult);
    pResult->m[0] = cosf(Angle);
    pResult->m[1] = -sinf(Angle);
    pResult->m[4] = sinf(Angle);
    pResult->m[5] = cosf(Angle);

}

// ---------------------------------------------------------------------------

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
/*
void Matrix2DMultVec(Vector2D* pResult, Matrix2D* pMtx, Vector2D* pVec)
{
    float x, y;

    x = pVec->x * pMtx->m[0][0] + pVec->y * pMtx->m[0][1] + pMtx->m[0][2];
    y = pVec->x * pMtx->m[1][0] + pVec->y * pMtx->m[1][1] + pMtx->m[1][2];

    pResult->x = x;
    pResult->y = y;

}
*/

// ---------------------------------------------------------------------------
