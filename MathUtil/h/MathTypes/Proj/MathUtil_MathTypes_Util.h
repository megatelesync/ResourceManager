#pragma once

/***********************************************************************************
* Contents of this file is similar to the contents of the  MathUtil_3DMath_Utils.h;
* The difference is that its functions use SquareMatrix class everywhere
* (instead of the XMMATRIX)
***********************************************************************************/

#include "../../3DMath/MathUtil_3DMath_Utils.h" // Temporarily included
#include "../BLAS/MathUtil_MathTypes_Matrix.h" // for SquareMatrix

namespace DvUtils
{
	namespace Math
	{
		/***************************************************************************
		* TODO:
		* 1. CalcSquareMatrix_PerspectiveProjection(depth,Q,QZnear, widthCoeff, heightCoeff) +
		* 1.1. REMARK: Remove the depth parameter (DONE: 12:12/20.09.2016)
		* 2. CalcSquareMatrixInverse_PerspectiveProjection+
		* 3. CalcSquareMatrix_MultiplicationByProjection (DONE: 19:11/30.09.2016);
		* 3.1. 
		***************************************************************************/
		template<class FloatTypeArg>
		SquareMatrix<FloatTypeArg, 4> CalcSquareMatrix_PerspectiveProjection
		(			
			FloatTypeArg Q,
			FloatTypeArg QZnear,
			FloatTypeArg widthCoeff,
			FloatTypeArg heightCoeff
		);

		template<class FloatTypeArg>
		SquareMatrix<FloatTypeArg, 4> CalcSquareMatrixInverse_PerspectiveProjection
		(			
			FloatTypeArg inversedZNear,
			FloatTypeArg inversedQZnear,
			FloatTypeArg inversedWidthCoeff,
			FloatTypeArg inversedHeightCoeff
		);

		// Transform the given square matrix with the given perspective projection matrix
		// (projection matrix is represented by a set of coefficients)
		// The result matrix is returned.
		template<class FloatTypeArg>
		SquareMatrix<FloatTypeArg, 4> CalcSquareMatrix_MultiplicationByProjection
		(
			const SquareMatrix<FloatTypeArg, 4>&			InMatrix,
			FloatTypeArg									Q,
			FloatTypeArg									QZnear,
			FloatTypeArg									widthCoeff,
			FloatTypeArg									heightCoeff
		);

		/**************************************************************************
		* IMPLEMENTATION
		**************************************************************************/
		template<class FloatTypeArg>
		SquareMatrix<FloatTypeArg, 4> CalcSquareMatrixInverse_PerspectiveProjection
		(			
			FloatTypeArg inversedZNear,
			FloatTypeArg inversedQZnear,
			FloatTypeArg inversedWidthCoeff,
			FloatTypeArg inversedHeightCoeff
		)
		{
			SquareMatrix<FloatTypeArg, 4> res_matrix
			(
				inversedWidthCoeff,						  0,				0,				0,
				0,						inversedHeightCoeff,				0,				0,
				0,									      0,				0,				-inversedQZnear,
				0,									      0,				1,				inversedZNear
			);	
			return res_matrix;
		}

		template<class FloatTypeArg>
		SquareMatrix<FloatTypeArg, 4> CalcSquareMatrix_PerspectiveProjection
		(			
			FloatTypeArg Q,
			FloatTypeArg QZnear,
			FloatTypeArg widthCoeff,
			FloatTypeArg heightCoeff
		)
		{
			SquareMatrix<FloatTypeArg, 4> res_matrix
			(
				widthCoeff,							  0,				0,				0,
				0,							heightCoeff,				0,				0,
				0,									  0,				Q,				1,
				0,									  0,				-QZnear,		0
			);	
			return res_matrix;
		}

		template<class FloatTypeArg>
		SquareMatrix<FloatTypeArg, 4> CalcSquareMatrix_MultiplicationByProjection
		(
			const SquareMatrix<FloatTypeArg, 4>&			InMatrix,
			FloatTypeArg									Q,
			FloatTypeArg									QZnear,
			FloatTypeArg									widthCoeff,
			FloatTypeArg									heightCoeff
		)
		{
			assert(&InMatrix);			
			FloatTypeArg Coeff_RowTwo = -QZnear; // negated Q * Znear
			SquareMatrix<FloatTypeArg, 4>				res_matrix
			(
				// Row 0:
				*GetComponentPointer(InMatrix, 0, 0) * widthCoeff,
				*GetComponentPointer(InMatrix, 0, 1) * widthCoeff,
				*GetComponentPointer(InMatrix, 0, 2) * widthCoeff,
				*GetComponentPointer(InMatrix, 0, 3) * widthCoeff,
				// Row 1
				*GetComponentPointer(InMatrix, 1, 0) * heightCoeff,
				*GetComponentPointer(InMatrix, 1, 1) * heightCoeff,
				*GetComponentPointer(InMatrix, 1, 2) * heightCoeff,
				*GetComponentPointer(InMatrix, 1, 3) * heightCoeff,
				// Row 2 (it's ok that the SECOND row is multiplied by Q)
				*GetComponentPointer(InMatrix, 2, 0) * Q + *GetComponentPointer(InMatrix, 3, 0),
				*GetComponentPointer(InMatrix, 2, 1) * Q + *GetComponentPointer(InMatrix, 3, 1),
				*GetComponentPointer(InMatrix, 2, 2) * Q + *GetComponentPointer(InMatrix, 3, 2),
				*GetComponentPointer(InMatrix, 2, 3) * Q + *GetComponentPointer(InMatrix, 3, 3),
				// Row 3 (it's ok that the SECOND row is taken from the src matrix)
				*GetComponentPointer(InMatrix, 2, 0) * Coeff_RowTwo,
				*GetComponentPointer(InMatrix, 2, 1) * Coeff_RowTwo,
				*GetComponentPointer(InMatrix, 2, 2) * Coeff_RowTwo,
				*GetComponentPointer(InMatrix, 2, 3) * Coeff_RowTwo
			);
			return res_matrix;
		}
	} // Math
} // DvUtils