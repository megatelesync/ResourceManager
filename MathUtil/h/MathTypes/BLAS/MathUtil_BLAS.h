#pragma once

/*********************************************************************************
* This header file includes all the stuff necessary for the matrix and vector 
* calculation
*********************************************************************************/

// Vec4 - this file automatically includes Vec4, Vec3, Vec2
#include "MathUtil_MathTypes_Vec4.h"
// Mat44 - this file automatically includes Mat44, Mat33
#include "MathUtil_MathTypes_Mat44.h"

namespace DvUtils
{
	namespace Math
	{
		/**********************************************************************************
		* Multiplying vector by matrix
		*
		* Available operations:
		* 1. By matrix type:
		*		1.1. SquareMatrix instance operations
		*		1.2. Memory operations
		*		1.3. std::array of Vec instance
		*
		* 2.By vector type:
		*		2.1. Vec instance operations
		*		2.2. Memory
		*		2.3. std::array<FloatTypeArg>
		*
		* 3. By vector size to matrix size:
		*		3.1. Multiplying on right side matrix instance
		*		3.2. Multiplying on right side matrix instance of lesser-than-vector size
		*
		* 4. Transposed or non-transposed:
		*		4.1. Multiplying on the right-side transposed matrix
		*		4.2. Multiplying on the right-side non-transposed matrix
		***********************************************************************************/

		// ------------- Operation --------------------------------------------
		// Vector on matrix right-side multiplication operation
		// ------------- Description ------------------------------------------
		//		Calculate the result of the vector on matrix multiplication
		// by multiplying the vector by each column of the matrix.		
		// (Vres = Vsrc * M)
		//
		//		The result vector is formed as the result value.
		//
		// -------------- Vector and matrix sizes ----------------------------	
		// 1) The src vector and src matrix
		//		The vector and the matrix must have equal sizes 
		// (Use the specialized non-operation function for multiplying
		// vectors and matrices of different sizes)
		//
		// 2) The result vector
		// The result vector have the size of the src vector.
		template
		<			
			class VecFloatTypeArg, 
			class MatrixFloatTypeArg, 
			size_t VecSize 
		>
		Vec<VecFloatTypeArg,VecSize> operator*
		(
			const Vec<VecFloatTypeArg, VecSize>						&InVec,
			const SquareMatrix<MatrixFloatTypeArg, VecSize>			&InMatrix
		);

		//------------- Operation ----------------------------------------------
		// Vector on matrix multiplication assign operation
		// ------------- Description -------------------------------------------
		//		Similar to the vector on matrix non-assign multiplication operation
		// The DIFFRENCE: 
		// The src vector is the dest vector 
		// (The result is placed in the src vector directly).
		//----------------------------------------------------------------------
		template
		<			
			class VecFloatTypeArg, 
			class MatrixFloatTypeArg, 
			size_t VecSize
		>
		Vec<VecFloatTypeArg,VecSize>& operator*=
		(
			Vec<VecFloatTypeArg, VecSize>							&InDestVec,
			const SquareMatrix<MatrixFloatTypeArg, VecSize>			&InMatrix
		);

		// ------------- Operation --------------------------------------------
		// Vector on matrix right-side multiplication		
		// ------------- Description ------------------------------------------
		//		Calculate the result of the vector on matrix multiplication
		// by multiplying the vector by each column of the matrix.		
		// (Vres = Vsrc * M)
		//
		//		Put the multiplication result to another	vector
		// (WARNING! The dest vector memory location may not overlap with the src vector!)
		//
		// -------------- Vector and matrix sizes ----------------------------		
		// 1) Src vector and src matrix 
		//		The src vector may a lesser size than the matrix size
		// (But src vector size may NOT be greater than the matrix size
		//  - use the MultOnRightMatrix_OfLesserSize function for this purpose!):
		// If the src vector have a lesser size, the dest vector
		// will be computed only the vector-sized matrix part will be 
		// considered during the computation (similar to multiplying the zero-padded
		// vector on the full src matrix)	
		//
		// 2) Result vector
		//		The result vector will be truncated to the size of
		//	the destination vector type (which is specified by the DestVecSize template argument)
		//
		template
		<
			class DestVecFloatTypeArg, size_t DestVecSize, 
			class VecFloatTypeArg, size_t VecSize, 
			class MatrixFloatTypeArg, size_t MatrixSize
		>
		void MultOnRightMatrix
		(
			Vec<DestVecFloatTypeArg, DestVecSize>				*pDestVec, 
			const Vec<VecFloatTypeArg, VecSize>					&InVec, 
			const SquareMatrix<MatrixFloatTypeArg, MatrixSize>	&InMatrix	
		);		
		
		//
		// ------------- Operation ------------------------------------------------
		// Vector on lesser-sized matrix right-side multiplication
		// ------------- Description ----------------------------------------------
		// Similar to the vector on matrix right-side multiplication
		// The DIFFRENCE: 
		// 1. the matrix to multiply on may have a lesser size 
		// than the vector that it to be multiplied on.
		// In this case: the multiplcation performed by
		// enhancing the matrix by putting the matrix over
		// the identity matrix of the vector size
		//		
		template
		<
			class DestVecFloatTypeArg, size_t DestVecSize, 
			class VecFloatTypeArg, size_t VecSize, 
			class MatrixFloatTypeArg, size_t MatrixSize
		>
		void MultOnRightMatrix_OfLesserSize
		(
			Vec<DestVecFloatTypeArg, DestVecSize>				*pDestVec, 
			const Vec<VecFloatTypeArg, VecSize>					&InVec, 
			const SquareMatrix<MatrixFloatTypeArg, MatrixSize>	&InMatrix	
		);
	} // Math
} // DvUtils