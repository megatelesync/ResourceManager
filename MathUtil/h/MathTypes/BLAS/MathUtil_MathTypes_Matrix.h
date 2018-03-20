#pragma once

#include "MathUtil_MathTypes_Vec.h"

/**************************************************************************************
* TODO SCHEME 2:
* 1. Implement functions:
* 1.1. Reset_FromColumnMajor
* 1.1. 2x2 +
* 1.2. 3x3 +
* 1.3. 4x4 +
* 1.2. Reset_FromColumns
* 1.2.1. 2x2
* 1.2.2. 3x3
* 1.2.3. 4x4
* 1.3. Reset_FromRowMajor
* 1.3.1. 2x2 
* 1.3.2. 3x3 +
* 1.3.3. 4x4 +
* 1.4. Reset_FromRows
* 1.5. Reset_n +
* 1.6. ResetFromMatrix +
* 1.7. ResetZero_n +
* 1.8. SetSubmatrixIdentity_n +
* 1.9. Transpose		+
* 2.0. TransposeCopy	+
*
* TODO SCHEME 1: 
* 1. Implement the Det functions:
* 1.1. Solution A: Implement each of 2x2 determinant calculation functions as  +
*      template specializations (of course full template specialization):
*		1.1.1. Implement specialization for <float,2>
*		1.1.2. Implement specialization for <double,2>
*		1.1.2. Implement specialization for <int,2>
* 1.2. Solution B: Implement the Det22, Det33 and Det44 functions instead
*  - WRONG SOLUTION - because other operations that depend on the determinant calculation
*   (like, for example, the inverse function) will be harder to implement.
* 2. Parametrize the functions Det22, Det33, Det44 to be able to compute
*	 the submatrix determinant calculation.
*    2.1. Create the functions DetSubmatrix22, DetSubmatrix33, DetSubmatrix44 etc. + +
*	 2.2. Use functions DetSubmatrixXX to compute the Det. +
**************************************************************************************/

namespace DvUtils
{
	namespace Math
	{
		/****************************************************************************************
		* Square matrix class
		*
		* Matrix is stored as the column major matrix
		*
		* Implementation TODO:
		* 1. Member operations
		* 1.1. Constructors:
		* 1.1.1. Default ctor			(DONE: 16.05.2016/14:30)
		* 1.1.2. Construction from explicit matrix set
		* 1.1.2.1. 2*2					(DONE: 16.05.2016/14:30)
		* 1.1.2.2. 3*3					(DONE: 16.05.2016/14:35)
		* 1.1.2.3. 4*4					(DONE: 16.05.2016/14:50)
		* 1.1.3. Construction from explicit vector set
		* 1.1.3.1. from 3 vectors		(DONE: 16.05.2016/15:09)
		* 1.1.3.2. from 4 vectors		(DONE: 16.05.2016/15:12)
		* 1.1.4. Construction from range of vectors
		* 1.1.4.1. Body is declared, but needs implementation
		* 1.1.5. Construction from array of vectors
		* 1.1.5.1. Body is declared, but needs implementation
		* 1.1.6. Construction from SquareMatrix submatrix (DONE: 16.05.2016/16:18)
		* 1.1.7. Construction from memory (DONE:16.05.2016/16:47)
		* 1.2. Copy assignment and constructors 
		* 1.2.1. Std copy assignment and ctor (DONE:16.05.2016/16:55)
		* 2. Non-member operations
		* 2.1. Low-level access
		* 2.1.1. Get pointer				(CODED: 16.05.2016/17:45)
		* 2.1.2. Get column pointer			(CODED: 16.05.2016/17:45)
		* 2.1.3. Get row pointer			(CODED: 16.05.2016/17:45)
		* 2.1.4. Get component pointer		(CODED: 16.05.2016/17:45)
		* 2.2. String operations
		* 2.2.1. Body is declared, but needs implementation
		* 2.3. Component read-only access		
		* 2.3.1. Get column					(CODED: 16.05.2016/17:55)
		* 2.3.2. Get component				(CODED: 16.05.2016/17:55)
		* 2.4. Component setters
		* 2.4.1. Set Component				(CODED: 16.05.2016/18:08)
		* 2.4.2. Set column				
		* 2.4.2.1. Set column from explicit values
		* 2.4.2.2. Set column from vector		
		* 2.4.2.3. Set column from array
		* 2.4.2.4. Set column from range
		* 2.4.2. Set row	
		* 2.4.3. Other set operations
		* 2.4.3.1. Setting all components to the same value
		* 2.4.3.2. Set submatrict identity
		* 2.4.3.3. Reset from row major
		* 2.4.3.4. Reset from column major
		* 2.4.3.5. Reset from rows
		* 2.4.3.6. Reset from columns
		* 2.4.3.7. Reset zero
		* 2.5. Copying and settings
		* 2.6. Equality testing
		* 2.6.1. operators != and ==			(CODED: 16.05.2016/19:01)
		* 2.6.2. submatrix equality testing
		* 2.6.2.1. Body is declared, but needs implementation
		* 2.7. Math operations		
		* 2.7.1. Main math operations 
		* 2.7.1.1. Add_To operation						  (CODED: 16.05.2016/19:48)
		* 2.7.1.2. Multiplying two SquareMatrix instances (CODED: 16.05.2016/19:48)
		* 2.7.1.2.1. Multiply
		* 2.7.1.2.2. Assign multiply
		* 2.7.1.3. Multiplying square matrix by scalar
		* 2.7.1.3.1. Multiply
		* 2.7.1.3.1.1. From left
		* 2.7.1.3.1.2. From right
		* 2.7.1.3.2. Assign multiply
		* 2.7.1.4. Negating
		* 2.7.1.5. Dividing square matrix by scalar
		* 2.7.1.5.1. Divide
		* 2.7.1.5.2. Assign divide
		* 2.7.1.6. Add or subtract matrices
		* 2.7.1.6.1. Add 
		* 2.7.1.6.1.1. Copy add
		* 2.7.1.6.1.2. Assign add
		* 2.7.1.6.2. Subtract
		* 2.7.1.6.2.1. Copy subtract
		* 2.7.1.6.2.2. Assign subtract
		* 2.7.2. Math operations (part 2)
		* 2.7.2.1. Transposing
		* 2.7.2.1.1. Transpose
		* 2.7.2.1.2. Transpose and copy
		* 2.7.2.2. Inverse
		* 2.7.2.2.1. Inverse
		* 2.7.2.2.2. Inverse and use precomputed Det
		* 2.7.2.3. Det
		* 2.7.2.3.1. 2*2 matrix
		* 2.7.2.3.2. 3*3 matrix
		* 2.8. Special operations
		* 2.8.1. BiasNormalizedMatrix44	(CODED: 29.09.2016/17:10)
		****************************************************************************************/

		// This two enums are used as the argument types
		// to overload operations for row-major and column-major matrices
		// Row-major matrix (means row elements are sequential in memory)
		enum class EMatrixRowMajor			{ Value = 0 };
		// Column-major matrix (means column elements are sequential in memory)
		enum class EMatrixColumnMajor		{ Value = 0 };

		// The Square matrix class itself
		template<class FloatTypeArg, size_t SizeArg>
		class SquareMatrix
		{
		public:
			/************************************************************************************
			* Typedefs
			************************************************************************************/
			typedef FloatTypeArg												FloatType;			
			typedef SquareMatrix<FloatTypeArg, SizeArg>							ThisType;
			// Type of the vector that represents one column
			typedef Vec<FloatTypeArg, SizeArg>									ColumnVecType;
					
			// Type of the array that represents components of one column
			typedef typename ColumnVecType::ArrayType							ColumnComponentArrayType;

			// COMMENT: Type of the array of all columns
			typedef std::array<ColumnVecType, SizeArg>							ColumnArrayType;

			/************************************************************************************
			* static functions
			************************************************************************************/
			// Size of one component
			static size_t				GetComponentSize    ();
			// Size of one column
			static size_t				GetColumnSize		();

			/************************************************************************************
			* ctors
			************************************************************************************/
			// default ctor
			// (creates identity matrix)
			SquareMatrix();

			// Create square matrix of the size specified as the class template argument
			// from the row-major source matrix, where the 2*2 submatrix is filled
			// with the given elements and the other elements are equal to the elements of the
			// identity matrix of the same size.
			// (Arguments indices of element Inij meaning:
			// i - index of row; j - index of column;)			
			SquareMatrix
			(
				const FloatTypeArg& In00, const FloatTypeArg& In01,
				const FloatTypeArg& In10, const FloatTypeArg& In11,				
				EMatrixRowMajor InSrcMatrixRowMajor = EMatrixRowMajor::Value
			);

			// Create square matrix of the size specified as the class template argument
			// from the column-major source matrix, where the 2*2 submatrix is filled
			// with the given elements and the other elements are equal to the elements of the
			// identity matrix of the same size.
			// (Arguments indices of element Inij meaning:
			// i - index of column; j - index of row;)		
			// 
			// Usage guidelines;
			//		pass EMatrixColumnMajor::Value to mark the src matrix as the column-major
			SquareMatrix
			(
				const FloatTypeArg& In00, const FloatTypeArg& In01,
				const FloatTypeArg& In10, const FloatTypeArg& In11,				
				EMatrixColumnMajor InSrcMatrixColumnMajor
			);
			
			// Create square matrix of the size specified as the class template argument
			// from the row-major source matrix, where the 3*3 submatrix is filled
			// with the given elements and the other elements are equal to the elements of the
			// identity matrix of the same size.
			// (Arguments indices of element Inij meaning:
			// i - index of row; j - index of column;)		
			SquareMatrix
			(
				const FloatTypeArg& In00, const FloatTypeArg& In01, const FloatTypeArg& In02,
				const FloatTypeArg& In10, const FloatTypeArg& In11, const FloatTypeArg& In12,
				const FloatTypeArg& In20, const FloatTypeArg& In21, const FloatTypeArg& In22,
				EMatrixRowMajor InSrcMatrixRowMajor = EMatrixRowMajor::Value
			);

			// Create square matrix of the size specified as the class template argument
			// from the column-major source matrix, where the 3*3 submatrix is filled
			// with the given elements and the other elements are equal to the elements of the
			// identity matrix of the same size.
			// (Arguments indices of element Inij meaning:
			// i - index of column; j - index of row;)		
			// 
			// Usage guidelines;
			//		pass EMatrixColumnMajor::Value to mark the src matrix as the column-major
			SquareMatrix
			(
				const FloatTypeArg& In00, const FloatTypeArg& In01, const FloatTypeArg& In02,
				const FloatTypeArg& In10, const FloatTypeArg& In11, const FloatTypeArg& In12,
				const FloatTypeArg& In20, const FloatTypeArg& In21, const FloatTypeArg& In22,
				EMatrixColumnMajor InSrcMatrixColumnMajor
			);

			// Create square matrix of the size specified as the class template argument
			// from the row-major source matrix, where the 4*4 submatrix is filled
			// with the given elements and the other elements are equal to the elements of the
			// identity matrix of the same size.
			// (Arguments indices of element Inij meaning:
			// i - index of row; j - index of column;)			
			SquareMatrix
			(
				const FloatTypeArg& In00, const FloatTypeArg& In01, const FloatTypeArg& In02, const FloatTypeArg& In03,
				const FloatTypeArg& In10, const FloatTypeArg& In11, const FloatTypeArg& In12, const FloatTypeArg& In13,
				const FloatTypeArg& In20, const FloatTypeArg& In21, const FloatTypeArg& In22, const FloatTypeArg& In23,
				const FloatTypeArg& In30, const FloatTypeArg& In31, const FloatTypeArg& In32, const FloatTypeArg& In33,
				EMatrixRowMajor InSrcMatrixRowMajor = EMatrixRowMajor::Value
			);

			// Create square matrix of the size specified as the class template argument
			// from the column-major source matrix, where the 4*4 submatrix is filled
			// with the given elements and the other elements are equal to the elements of the
			// identity matrix of the same size.
			// (Arguments indices of element Inij meaning:
			// i - index of column; j - index of row;)		
			// 
			// Usage guidelines;
			//		pass EMatrixColumnMajor::Value to mark the src matrix as the column-major
			SquareMatrix
			(
				const FloatTypeArg& In00, const FloatTypeArg& In01, const FloatTypeArg& In02, const FloatTypeArg& In03,
				const FloatTypeArg& In10, const FloatTypeArg& In11, const FloatTypeArg& In12, const FloatTypeArg& In13,
				const FloatTypeArg& In20, const FloatTypeArg& In21, const FloatTypeArg& In22, const FloatTypeArg& In23,
				const FloatTypeArg& In30, const FloatTypeArg& In31, const FloatTypeArg& In32, const FloatTypeArg& In33,
				EMatrixColumnMajor InSrcMatrixColumnMajor
			);

			//
			// Construction of 3*3 matrix from a set of row vectors.
			//
			//		Vectors may have any size and any type convertible to the 
			// component type of the matrix class template instance.
			//
			//		The resulting matrix is an identity matrix with its 3*3 submatrix 
			// is replaced by the matrix specified by the given set of columns.			
			//
			template<class SrcFloatTypeArg, size_t SrcNumComponents>
			SquareMatrix
			(
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow0,
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow1,
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow2,
				EMatrixRowMajor InSrcMatrixRowMajor = EMatrixRowMajor::Value
			);

			//
			// Construction of 2*2 matrix from a set of row vectors.
			//
			//		Vectors may have any size and any type convertible to the 
			// component type of the matrix class template instance.
			//
			//		The resulting matrix is an identity matrix with its 2*2 submatrix 
			// is replaced by the matrix specified by the given set of columns.			
			//
			template<class SrcFloatTypeArg, size_t SrcNumComponents>
			SquareMatrix
			(
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow0,
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow1,
				EMatrixRowMajor InSrcMatrixRowMajor = EMatrixRowMajor::Value
			);


			//
			// Construction of 3*3 matrix from a set of column vectors.
			//
			//		Vectors may have any size and any type convertible to the 
			// component type of the matrix class template instance.
			//
			//		The resulting matrix is an identity matrix with its 3*3 submatrix 
			// is replaced by the matrix specified by the given set of columns.			
			// 
			// Usage guidelines:
			//		Pass the EMatrixColumnMajor::Value to mark the src vectors as columns of the matrix
			template<class SrcFloatTypeArg, size_t SrcNumComponents>
			SquareMatrix
			(
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn0,
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn1,
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn2,
				EMatrixColumnMajor InSrcMatrixColumnMajor
			);

			//
			// Construction of 4*4 matrix from a set of row vectors.
			//		Vectors may have any size and any type convertible to the 
			// component type of the matrix class template instance.
			//
			//		The resulting matrix is an identity matrix with its 4*4 submatrix 
			// replaced by the matrix specified by the given set of columns.		
			template<class SrcFloatTypeArg, size_t SrcNumComponents>
			SquareMatrix
			(
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow0,
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow1,
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow2,				
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow3,
				EMatrixRowMajor InSrcMatrixRowMajor = EMatrixRowMajor::Value
			);

			//
			// Construction of 4*4 matrix from a set of column vectors.
			//		Vectors may have any size and any type convertible to the 
			// component type of the matrix class template instance.
			//
			//		The resulting matrix is an identity matrix with its 4*4 submatrix 
			// replaced by the matrix specified by the given set of columns.	
			//			
			// Usage guidelines:
			//		Pass the EMatrixColumnMajor::Value to mark the src vectors as columns of the matrix
			template<class SrcFloatTypeArg, size_t SrcNumComponents>
			SquareMatrix
			(
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn0,
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn1,
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn2,				
				const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn3,
				EMatrixColumnMajor InSrcMatrixColumnMajor
			);

			//
			// Construction of square matrix of the given size 
			// from the given set of row vectors.
			//
			//		Vectors may have any size and any type convertible to the 
			// component type of the matrix class template instance.
			//		The iterators must point to the range of size equal to the size 
			// provided as the argument.
			//
			//		The resulting matrix is an identity matrix with its submatrix of the given size
			// replaced by the matrix specified by the given set of rows.
			template<class RowVecIterator>
			SquareMatrix
			(			
				size_t InSize,
				RowVecIterator firstRowVecIt, RowVecIterator lastRowVecIt,
				EMatrixRowMajor InSrcMatrixRowMajor = EMatrixRowMajor::Value
			);

			//
			// Construction of square matrix of the given size 
			// from the given set of column vectors.
			//
			//		Vectors may have any size and any type convertible to the 
			// component type of the matrix class template instance.
			//		The iterators must point to the range of size equal to the size 
			// provided as the argument.
			//
			//		The resulting matrix is an identity matrix with its submatrix of the given size
			// replaced by the matrix specified by the given set of columns.
			//			
			// Usage guidelines:
			//		Pass the EMatrixColumnMajor::Value to mark the src vectors as columns of the matrix
			//
			template<class ColumnVecIterator>
			SquareMatrix
			(							
				size_t InSize,
				ColumnVecIterator firstColumnVecIt, ColumnVecIterator lastColumnVecIt,				
				EMatrixColumnMajor InSrcMatrixColumnMajor
			);

			//
			// Construction of square matrix of the size specified as the templ argument 
			// from the given submatrix of the given array of an arbitrary size.
			// If the destintation matrix has a greater size than the specified size argument, 
			// than the extra components are equal to the corresponding components of 
			// the identity matrix.
			template<class SrcFloatTypeArg, size_t InSrcVecArraySize, size_t InSrcVecSize>
			SquareMatrix
			(	
				size_t InSize,
				const std::array<Vec<SrcFloatTypeArg, InSrcVecSize>, InSrcVecArraySize>& InArray,												
				EMatrixRowMajor InSrcMatrixRowMajor = EMatrixRowMajor::Value
			);

			
			//
			// Construction of square matrix of the size specified as the templ argument 
			// from the given submatrix of the given array of an arbitrary size.
			// If the destintation matrix has a greater size than the specified size argument, 
			// than the extra components are equal to the corresponding components of 
			// the identity matrix.
			//			
			// Usage guidelines:
			//		Pass the EMatrixColumnMajor::Value to mark the src vectors as columns of the matrix
			//
			template<class SrcFloatTypeArg, size_t InSrcVecArraySize, size_t InSrcVecSize>
			SquareMatrix
			(				
				size_t InSize,
				const std::array<Vec<SrcFloatTypeArg, InSrcVecSize>, InSrcVecArraySize>& InArray,												
				EMatrixColumnMajor InSrcMatrixColumnMajor
			);

			//
			// Create the square matrix of the size specified as the templ argument.
			// The constructed matrix will have a submatrix of the specified size copied
			// from the given src matrix and the rest elements will be equal
			// to the corresponding elements of the identity matrix.
			// The src matrix is treated as the row-major.
			//
			template<class SrcFloatTypeArg, size_t SrcSize>
			SquareMatrix
			(
				size_t InSize,
				const SquareMatrix<SrcFloatTypeArg, SrcSize>& InSrcMatrix,				
				EMatrixRowMajor InSrcMatrixRowMajor = EMatrixRowMajor::Value
			);

			//
			// Create the square matrix of the size specified as the templ argument.
			// The constructed matrix will have a submatrix of the specified size copied
			// from the given src matrix and the rest elements will be equal
			// to the corresponding elements of the identity matrix.
			// The src matrix is treated as the column-major.			
			//			
			// Usage guidelines:
			//		Pass the EMatrixColumnMajor::Value to mark the src vectors as columns of the matrix
			//
			template<class SrcFloatTypeArg, size_t SrcSize>
			SquareMatrix
			(
				size_t InSize,
				const SquareMatrix<SrcFloatTypeArg, SrcSize>& InSrcMatrix,				
				EMatrixColumnMajor InSrcMatrixColumnMajor
			);

			//
			//	Creates the square matrix from the given memory region.
			//
			//		The memory region is not necessarily square
			// (num src rows and num src columns define the region size).
			//
			//		The resulting matrix is an identity matrix with its 
			// num src rows * num src columns submatrix replaced by the matrix specified 
			// by the given memory region.
			//
			template<class SrcFloatTypeArg>
			SquareMatrix
			(
				const SrcFloatTypeArg			*pSrc, 
				size_t							 rowByteStride, 
				size_t							 componentByteStride		= sizeof(SrcFloatTypeArg),
				size_t							 InNumSrcRows				= pSrc->GetSize(),
				size_t							 InNumSrcColumns			= pSrc->GetSize()								
			);

			/************************************************************************************
			* Copy assignment and ctors
			************************************************************************************/
			SquareMatrix										(const ThisType& InSrc);
			ThisType&						operator=			(const ThisType& InSrc);


			/************************************************************************************
			* Num columns and rows
			************************************************************************************/
			// Get num rows or column (which is the same for square matrix)
			int								GetSize					()						const				{ return SizeArg; }			

			/************************************************************************************
			* Getters
			************************************************************************************/
			const ColumnArrayType&			GetColumnArray			()						const				{ return _columns; }
			ColumnArrayType&			    GetColumnArray			()											{ return _columns; }

		private:		
			ColumnArrayType					_columns;
		};

		template<class FloatTypeArg, int Size>
		bool operator==(const SquareMatrix<FloatTypeArg, Size>& InLeft, const SquareMatrix<FloatTypeArg, Size>& InRight)
		{
			for (int i = 0; i < Size; i++)
			{
				for (int j = 0; j < Size; j++)
				{
					if (*GetComponentPointer(InLeft, i, j) != *GetComponentPointer(InRight, i, j))
					{
						return false;
					}
				}
			}
			return true;
		}

		template<class FloatTypeArg, int Size>
		bool operator!=(const SquareMatrix<FloatTypeArg, Size>& InLeft, const SquareMatrix<FloatTypeArg, Size>& InRight)
		{
			return (false == operator==(InLeft, InRight));
		}

		/************************************************************************************
		* Low-level access
		************************************************************************************/	
		// Get const pointer to the first component of the given matrix
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetPointer				(const SquareMatrix<FloatTypeArg, Size>&			InMatrix);

		// Get pointer to the first component of the given matrix
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetPointer				(SquareMatrix<FloatTypeArg, Size>&					InMatrix);

		// Get const pointer to the given column of the given matrix:
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetColumnPointer		(const SquareMatrix<FloatTypeArg, Size>&			InMatrix, int InColumIndex);

		// Get pointer to the given column of the given matrix:
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetColumnPointer		(SquareMatrix<FloatTypeArg, Size>&			InMatrix, int InColumIndex);

		// Get constant pointer to the X column of the given matrix:
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetXColumnPointer		(const SquareMatrix<FloatTypeArg, Size>&			InMatrix);

		// Get pointer to the X column of the given matrix:
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetXColumnPointer		(SquareMatrix<FloatTypeArg, Size>&			InMatrix);

		// Get constant pointer to the Y column of the given matrix:
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetYYColumnPointer		(const SquareMatrix<FloatTypeArg, Size>&			InMatrix);

		// Get pointer to the Y column of the given matrix:
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetYYColumnPointer		(SquareMatrix<FloatTypeArg, Size>&			InMatrix);


		// Get constant pointer to the Z column of the given matrix:
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetZZZColumnPointer		(const SquareMatrix<FloatTypeArg, Size>&			InMatrix);

		// Get pointer to the Z column of the given matrix:
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetZZZColumnPointer		(SquareMatrix<FloatTypeArg, Size>&			InMatrix);		

		// Get const pointer to the given row start of the matrix
		// (Note: the distance between the two row elements is the columns size)
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetRowStartPointer			(const SquareMatrix<FloatTypeArg, Size>&			InMatrix, int InRowIndex);

		// Get pointer to the given row start of the matrix
		// (Note: the distance between the two row elements is the columns size)
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetRowStartPointer			(SquareMatrix<FloatTypeArg, Size>&			InMatrix, int InRowIndex);

		// Get const pointer to the row X of the matrix
		// (Note: the distance between the two row elements is the columns size)
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetRowXStartPointer			(const SquareMatrix<FloatTypeArg, Size>&			InMatrix);

		// Get pointer to the row X of the matrix
		// (Note: the distance between the two row elements is the columns size)
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetRowXStartPointer			(SquareMatrix<FloatTypeArg, Size>&			InMatrix);

		// Get const pointer to the row Y of the matrix
		// (Note: the distance between the two row elements is the columns size)
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetRowYYStartPointer			(const SquareMatrix<FloatTypeArg, Size>&			InMatrix);

		// Get pointer to the row Y of the matrix
		// (Note: the distance between the two row elements is the columns size)
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetRowYYStartPointer			(SquareMatrix<FloatTypeArg, Size>&			InMatrix);

		// Get const pointer to the row Z of the matrix
		// (Note: the distance between the two row elements is the columns size)
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetRowZZZStartPointer			(const SquareMatrix<FloatTypeArg, Size>&			InMatrix);

		// Get pointer to the row Z of the matrix
		// (Note: the distance between the two row elements is the columns size)
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetRowZZZStartPointer			(SquareMatrix<FloatTypeArg, Size>&			InMatrix);

		// Get const pointer to the component of the matrix
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetComponentPointer			(const SquareMatrix<FloatTypeArg, Size>&	InMatrix, int InRowIndex, int InColumnIndex);

		// Get pointer to the component of the matrix
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetComponentPointer			(SquareMatrix<FloatTypeArg, Size>&			InMatrix, int InRowIndex, int InColumnIndex);

		/************************************************************************************
		* String representation
		************************************************************************************/
		template<class FloatTypeArg, int Size>
		std::string	ToDebugString				(const SquareMatrix<FloatTypeArg, Size>&	InMatrix, int InPrecisionDigs = 6);

		template<class FloatTypeArg, int Size>
		std::string	ToString					(const SquareMatrix<FloatTypeArg, Size>&	InMatrix, int InPrecisionDigs = 6);

		/************************************************************************************
		* Stream output
		************************************************************************************/
		template<class StrmTypeArg, class FloatTypeArg, int SizeArg>
		StrmTypeArg& operator<<(StrmTypeArg& InStrm, const SquareMatrix<FloatTypeArg, SizeArg>& InMatrix)
		{
			InStrm << ToDebugString(InMatrix);
			return InStrm;
		}

		/************************************************************************************
		* Matrix operations
		************************************************************************************/
		// Multiplying on scalar
		template<class DestFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>&	operator*=			
		(
			SquareMatrix<DestFloatTypeArg, Size>&					InDestMatrix,
			const double&											InCoeff
		);

		// Division by scalar
		template<class DestFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>&	operator/=			
		(
			SquareMatrix<DestFloatTypeArg, Size>&					InDestMatrix,
			const double&											InCoeff
		);

		// Get matrix multiplied by the scalar
		template<class DestFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator*			
		(
			const SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix,
			const double&											InCoeff
		);

		template<class DestFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator*			
		(
			const double&											InCoeff,
			const SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix
		);

		// Get matrix divided by the given coeff
		template<class DestFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator/			
		(
			const SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix,
			const double&											InCoeff
		);			

		// Get the negated matrix
		template<class DestFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator-			
		(
			const SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix			
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>&	operator+=			
		(
			SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, Size>&			InSrcMatrix
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>&	operator-=			
		(
			SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, Size>&			InSrcMatrix
		);

		// Multiplying matrix on matrix
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>&	operator*=			
		(
			SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, Size>&			InSrcMatrix
		);

		
		// Get the sum of two equal-sized matrices
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator+
		(
			const SquareMatrix<DestFloatTypeArg, Size>&			InDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, Size>&			InSrcMatrix
		);

		// Get the difference of two equal-sized matrices
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator-
		(
			const SquareMatrix<DestFloatTypeArg, Size>&			InDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, Size>&			InSrcMatrix
		);

		// Get the multiple of two equal-sized matrices
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator*
		(
			const SquareMatrix<DestFloatTypeArg, Size>&			InDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, Size>&			InSrcMatrix
		);

		/************************************************************************************
		* Matrix operations - multiplication matrix by vector with assignment
		************************************************************************************/
		// Same size matrix multiplication
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t SizeArg>
		Vec<DestFloatTypeArg, SizeArg>& operator*=
		(
			Vec<DestFloatTypeArg, SizeArg>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, SizeArg>& InRight
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, 2>& operator*=
		(
			Vec<DestFloatTypeArg, 2>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, 3>& InRight
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, 2>& operator*=
		(
			Vec<DestFloatTypeArg, 2>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, 4>& InRight
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, 3>& operator*=
		(
			Vec<DestFloatTypeArg, 3>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, 4>& InRight
		);


		/************************************************************************************
		* Matrix operations - multiplication matrix by vector
		************************************************************************************/
		// Same size matrix multiplication
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t SizeArg>
		Vec<DestFloatTypeArg, SizeArg> operator*
		(
			const Vec<DestFloatTypeArg, SizeArg>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, SizeArg>& InRight
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, 2> operator*
		(
			const Vec<DestFloatTypeArg, 2>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, 3>& InRight
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, 2> operator*
		(
			const Vec<DestFloatTypeArg, 2>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, 4>& InRight
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, 3> operator*
		(
			const Vec<DestFloatTypeArg, 3>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, 4>& InRight
		);



		/************************************************************************************
		* Matrix operations - adding
		************************************************************************************/
		template<class DestFloatTypeArg, size_t DestSize, class SrcFloatTypeArg, size_t SrcSize>
		void Add_To
		(
			SquareMatrix<DestFloatTypeArg,DestSize>				*pDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, SrcSize>		&srcMatrix,
			int													 InNumRows,
			int													 InNumColumns
		);

		/************************************************************************************
		* Matrix operations (part II)
		************************************************************************************/
		// 1.Description:
		//		Compute determinant of the submatrix of the given matrix
		// 2.Arguments:
		//		InSize - size of the submatrix;
		//		InStartRow - Upper row of the submatrix;
		//		InStartColumn - Left column of the submatrix.			
		// 3. Implementation notes:
		//		This function is implemented using template specializations
		// for all practically useable pairs of types and sizes.				
		// 4. Supported matrix sizes: 2, 3, 4
		// 5. Supported matrix element types:
		// 5.1. float
		// 5.2. double
		// 5.3. int
		// REFACTOR:
		// 1. rename to DetSubmatrix +
		template<class FloatTypeArg, size_t Size>
		FloatTypeArg DetSubmatrix
		(
			const SquareMatrix<FloatTypeArg, Size>&				InMatrix, 
			int													InSize,
			int													InStartRow			= 0,
			int													InStartColumn		= 0
		);

		template<class FloatTypeArg, size_t Size>
		FloatTypeArg Det
		(
			const SquareMatrix<FloatTypeArg, Size>&				InMatrix 
		);
		
		// Compute determinant of the 2*2 matrix
		template<class FloatTypeArg>		
		FloatTypeArg Det2x2
		(
			const SquareMatrix<FloatTypeArg, 2>&				InMatrix
		);

		template<class FloatTypeArg>
		FloatTypeArg DetSubmatrix2x2
		(
			const SquareMatrix<FloatTypeArg, 2>& InMatrix,
			int InRow,
			int InColumn
		);

		// Compute determinant of the 3*3 matrix		
		template<class FloatTypeArg>		
		FloatTypeArg Det3x3
		(
			const SquareMatrix<FloatTypeArg, 3>&				InMatrix
		);

		template<class FloatTypeArg>
		FloatTypeArg DetSubmatrix3x3
		(
			const SquareMatrix<FloatTypeArg, 3>&				InMatrix,
			int													InRow,
			int													InColumn
		);

		// Compute determinant of the 4*4 matrix
		template<class FloatTypeArg>		
		FloatTypeArg Det4x4
		(
			const SquareMatrix<FloatTypeArg, 4>&				InMatrix
		);
		
		template<class FloatTypeArg>
		FloatTypeArg DetSubmatrix4x4
		(
			const SquareMatrix<FloatTypeArg, 4>&				InMatrix,
			int													InRow,
			int													InColumn
		);

		// Inverse the given matrix
		template<class FloatTypeArg>
		void Inverse
		(
			SquareMatrix<FloatTypeArg, 2>					*pInOutMatrix
		);

		template<class FloatTypeArg>
		void Inverse
		(
			SquareMatrix<FloatTypeArg, 3>					*pInOutMatrix
		);

		template<class FloatTypeArg>
		void Inverse
		(
			SquareMatrix<FloatTypeArg, 4>					*pInOutMatrix
		);
		
		// Inverse the given matrix using the precomputed determinant value
		// and place the copy into the given matrix
		template<class FloatTypeArg>
		void InverseCopy_UsePrecomputedDet
		(
			SquareMatrix<FloatTypeArg, 2>					*pDestMatrix,
			const SquareMatrix<FloatTypeArg, 2>				*pSrcMatrix,
			const FloatTypeArg									&InDet
		);		
		template<class FloatTypeArg>
		void InverseCopy_UsePrecomputedDet
		(
			SquareMatrix<FloatTypeArg, 3>					*pDestMatrix,
			const SquareMatrix<FloatTypeArg, 3>				*pSrcMatrix,
			const FloatTypeArg									&InDet
		);
		template<class FloatTypeArg>
		void InverseCopy_UsePrecomputedDet
		(
			SquareMatrix<FloatTypeArg, 4>					*pDestMatrix,
			const SquareMatrix<FloatTypeArg, 4>				*pSrcMatrix,
			const FloatTypeArg									&InDet
		);

		// Make a transposed copy of the given matrix
		// WARNING! The dest matrix may not be the same as the src matrix
		// (from the memory location perspective)
		template<class FloatTypeArg, size_t Size>
		void TransposeCopy
		(
			SquareMatrix<FloatTypeArg, Size>					*pOutMatrix,
			const SquareMatrix<FloatTypeArg, Size>				&InMatrix
		);		

		// Transpose the given matrix
		template<class FloatTypeArg, size_t Size>
		void Transpose
		(
			SquareMatrix<FloatTypeArg, Size>					*pOutMatrix
		);	



		

		/************************************************************************************
		* Copying and settings
		*
		* NOTE:
		*	- Operations named Reset* affect the whole destination matrix always
		*	(ever if the specified destination submatrix is assumed, the rest of 
		*    the matrix	is reset to the values of the corresponding identity
		*    matrix of the size of the destination matrix;
		*   - Operations named SetSubmatrix* affect only the specified submatrix
		*	  of the destination matrix without touching components 
		*	  which indices are beyond the index range of the specified submatrix
		*
		*   EXAMPLE:
		*	A = ( 
		*    2.0, 2.0, 2.0
		*    2.0, 2.0, 2.0
		*    2.0, 2.0, 2.0
		*   )
		*	B = ( 
		*    3.0, 3.0, 3.0
		*    3.0, 3.0, 3.0
		*    3.0, 3.0, 3.0
		*   )
		*   After resettings matrix 3*3 part
		*   of matrix A from matrix B the result will be:		
		*	( 
		*    3.0, 3.0, 0.0
		*    3.0, 3.0, 0.0
		*    0.0, 0.0, 1.0
		*   )
		*
		*)
		************************************************************************************/
		// Make the given matrix zero
		template<class DestFloatTypeArg, size_t DestNumElemsArg>		
		void ResetZero_n
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix			
		);

		// Make the submatrix of the given size of the given matrix 
		// identity
		template<class DestFloatTypeArg, size_t DestNumElemsArg>		
		void SetSubmatrixIdentity_n
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			size_t													 InSize		= pMatrix->GetSize()
		);

		// Set all elements of the given matrix to the given value
		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_n
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			DestFloatTypeArg										 componentValue 			
		);	

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_FromRowMajor
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const DestFloatTypeArg& In00, const DestFloatTypeArg& In01,
			const DestFloatTypeArg& In10, const DestFloatTypeArg& In11
		);

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_FromColumnMajor
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const DestFloatTypeArg& In00, const DestFloatTypeArg& In01,
			const DestFloatTypeArg& In10, const DestFloatTypeArg& In11
		);

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_FromRowMajor
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const DestFloatTypeArg& In00, const DestFloatTypeArg& In01, const DestFloatTypeArg& In02,
			const DestFloatTypeArg& In10, const DestFloatTypeArg& In11, const DestFloatTypeArg& In12,
			const DestFloatTypeArg& In20, const DestFloatTypeArg& In21, const DestFloatTypeArg& In22
		);

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_FromColumnMajor
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const DestFloatTypeArg& In00, const DestFloatTypeArg& In01, const DestFloatTypeArg& In02,
			const DestFloatTypeArg& In10, const DestFloatTypeArg& In11, const DestFloatTypeArg& In12,
			const DestFloatTypeArg& In20, const DestFloatTypeArg& In21, const DestFloatTypeArg& In22
		);

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_FromRowMajor
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const DestFloatTypeArg& In00, const DestFloatTypeArg& In01, const DestFloatTypeArg& In02, const DestFloatTypeArg& In03,
			const DestFloatTypeArg& In10, const DestFloatTypeArg& In11, const DestFloatTypeArg& In12, const DestFloatTypeArg& In13,
			const DestFloatTypeArg& In20, const DestFloatTypeArg& In21, const DestFloatTypeArg& In22, const DestFloatTypeArg& In23,
			const DestFloatTypeArg& In30, const DestFloatTypeArg& In31, const DestFloatTypeArg& In32, const DestFloatTypeArg& In33
		);

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_FromColumnMajor
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const DestFloatTypeArg& In00, const DestFloatTypeArg& In01, const DestFloatTypeArg& In02, const DestFloatTypeArg& In03,
			const DestFloatTypeArg& In10, const DestFloatTypeArg& In11, const DestFloatTypeArg& In12, const DestFloatTypeArg& In13,
			const DestFloatTypeArg& In20, const DestFloatTypeArg& In21, const DestFloatTypeArg& In22, const DestFloatTypeArg& In23,
			const DestFloatTypeArg& In30, const DestFloatTypeArg& In31, const DestFloatTypeArg& In32, const DestFloatTypeArg& In33
		);

		// Reset matrix to the matrix equal to the given column-major matrix
		template<class DestFloatTypeArg, size_t DestNumElemsArg, class SrcFloatTypeArg, size_t SrcNumComponents>
		void Reset_FromRows
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow1
		);

		// Reset matrix to the matrix equal to the given column-major matrix
		template<class DestFloatTypeArg, size_t DestNumElemsArg, class SrcFloatTypeArg, size_t SrcNumComponents>
		void Reset_FromRows
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow1,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow2
		);

		template<class DestFloatTypeArg, size_t DestNumElemsArg, class SrcFloatTypeArg, size_t SrcNumComponents>
		void Reset_FromRows
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow1,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow2,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow3
		);

		// Reset matrix to the matrix equal to the given column-major matrix
		template<class DestFloatTypeArg, size_t DestNumElemsArg, class SrcFloatTypeArg, size_t SrcNumComponents>
		void Reset_FromColumns
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InColumn0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InColumn1,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InColumn2
		);

		// Resetting matrix:
		// 
		// Copying from the given submatrix of the given src matrix			
		// to the submatrix of the destination matrix
		// and fill extra components of the destination matrix
		// as the corresponding components of the identity matrix 
		// of the given destination matrix size.
		// 
		template
		<
			class DestFloatTypeArg, size_t DestNumElemsArg, 
			class SrcFloatTypeArg, size_t SrcNumElemsArg
		>
		void ResetFromMatrix
		(
			size_t													InSrcSize,
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, SrcNumElemsArg>		&InSrcMatrix	
		);	

		template
		<
			class DestFloatTypeArg, size_t DestNumElemsArg, 
			class SrcFloatTypeArg, size_t SrcNumElemsArg
		>

		// Resetting from transposed matrix:
		// 
		// Copying from the given submatrix of the given transposed src matrix			
		// to the submatrix of the destination matrix
		// and fill extra components of the destination matrix
		// as the corresponding components of the identity matrix 
		// of the given destination matrix size.
		// 
		void ResetFromTransposedMatrix
		(
			size_t													InSrcSize,
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, SrcNumElemsArg>		&InSrcMatrix	
		);
		
		//
		// Copying from the given src memory to the submatrix of the given destination matrix	
		//
	    // WARNING: only the given submatrix is updated
		//
		template<class DestFloatTypeArg, size_t DestNumElemsArg, class SrcFloatTypeArg>
		void CopyCastToSubmatrix_n
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,			
			size_t													 rowStrideInBytes,
			size_t													 componentStrideInBytes = sizeof(SrcFloatTypeArg),
			size_t													 InNumRows          = pMatrix->GetSize(),
			size_t													 InNumColumns		= pMatrix->GetSize(),
			size_t													 destFirstRow		= 0,
			size_t													 destFirstColumn	= 0
		);	

		//
		// Copying from the given src memory to the given destination matrix	
		//
	    // Rhe result is like the whole matrix is reset to the identity matrix 
		// and then the submatrix of the given size is updated		
		//
		template<class DestFloatTypeArg, size_t DestNumElemsArg, class SrcFloatTypeArg>
		void ResetMatrixFromMemory
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,			
			size_t													 rowStrideInBytes,
			size_t													 componentStrideInBytes = sizeof(SrcFloatTypeArg),
			size_t												     InNumRows			= pMatrix->GetSize(),
			size_t													 InNumColumns		= pMatrix->GetSize(),
			size_t													 destFirstRow		= 0,
			size_t													 destFirstColumn	= 0
		);	

		/************************************************************************************
		* Component read-only access functions
		*
		* NEW FUNCTIONALITY SUGGESTIONS:
		* 1. Getting row (inefficient but good tool for constructing row
		* from the column-major matrix)
		************************************************************************************/
		// Get column vector object const reference
		template<class FloatTypeArg, int Size>
		const typename SquareMatrix<FloatTypeArg,Size>::ColumnVecType&		GetColumn
		(
			const SquareMatrix<FloatTypeArg, Size>&			InMatrix,	
			size_t											InColumnIndex
		);

		template<class FloatTypeArg, int Size>
		const typename SquareMatrix<FloatTypeArg,Size>::ColumnVecType&		GetXColumn
		(
			const SquareMatrix<FloatTypeArg, Size>&			InMatrix			
		);

		template<class FloatTypeArg, int Size>
		const typename SquareMatrix<FloatTypeArg,Size>::ColumnVecType&		GetYYColumn
		(
			const SquareMatrix<FloatTypeArg, Size>&			InMatrix			
		);

		template<class FloatTypeArg, int Size>
		const typename SquareMatrix<FloatTypeArg,Size>::ColumnVecType&		GetZZZColumn
		(
			const SquareMatrix<FloatTypeArg, Size>&			InMatrix			
		);

		// Get component of the matrix
		template<class FloatTypeArg, int SizeArg>
		const FloatTypeArg&			GetComponent
		(
			const SquareMatrix<FloatTypeArg, SizeArg>&		InMatrix,	
			size_t											InRowIndex, 
			size_t											InColumnIndex
		);


		/************************************************************************************
		* Equality testing
		************************************************************************************/
		// Checking equality of two matrices
		// (both matrices must have equal sizes)
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		bool operator==
		(
			const SquareMatrix<DestFloatTypeArg, Size>&			InFirst,
			const SquareMatrix<DestFloatTypeArg, Size>&			InSecond
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		bool operator!=
		(
			const SquareMatrix<DestFloatTypeArg, Size>&			InFirst,
			const SquareMatrix<DestFloatTypeArg, Size>&			InSecond
		);

		// Checking that two submatrices of the given size are equal
		template
		<
			class DestFloatTypeArg, size_t DestSize,
			class SrcFloatTypeArg, size_t SrcSize
		>
		bool AreSubmatricesEqual
		(
			const SquareMatrix<DestFloatTypeArg, DestSize>&			InFirst,
			const SquareMatrix<DestFloatTypeArg, SrcSize>&			InSecond,
			size_t InSubmatrixNumRows,
			size_t InSubmatrixNumColumns,
			size_t InFirstStartRow = 0,
			size_t InFirstStartColumn = 0,
			size_t InSecondStartRow = 0,
			size_t InSecondStartColumn = 0
		);

		
		/************************************************************************************
		* Component set functions
		************************************************************************************/
		template<class FloatTypeArg, int Size>
		void SetComponent
		(
			SquareMatrix<FloatTypeArg, Size>			*pOutMatrix,
			int											 InRowIndex,
			int											 InColumnIndex,
			const FloatTypeArg&							 InNewValue
		);	

		//
		// Set two sequential components of the given column
		//
		template
		<
			class FloatTypeArg, size_t SizeArg		
		>
		void SetColumnPart_TwoComponents
		(
			SquareMatrix<FloatTypeArg, SizeArg>				*pOutMatrix,
			size_t											 InColumnIndex,
			const FloatTypeArg							    &InFirst,
			const FloatTypeArg								&InSecond,
			size_t											 InDestPartStart			= 0
		);		

		//
		// Set three sequential components of the given column
	    //
		// Arguments:
		//		InDestPartStart - start index of the column to be updated
		//
		template
		<
			class FloatTypeArg, size_t SizeArg		
		>
		void SetColumnPart_ThreeComponents
		(
			SquareMatrix<FloatTypeArg, SizeArg>				*pOutMatrix,
			size_t											 InColumnIndex,
			const FloatTypeArg							    &InFirst,
			const FloatTypeArg								&InSecond,
			const FloatTypeArg								&InThird,
			size_t											 InDestPartStart			= 0
		);	

		// Set four sequential components of the given column
	    //
		// Arguments:
		//		InDestPartStart - start index of the column to be updated
		//
		template
		<
			class FloatTypeArg, size_t SizeArg		
		>
		void SetColumnPart_ThreeComponents
		(
			SquareMatrix<FloatTypeArg, SizeArg>				*pOutMatrix,
			size_t											 InColumnIndex,
			const FloatTypeArg							    &InFirst,
			const FloatTypeArg								&InSecond,
			const FloatTypeArg								&InThird,
			const FloatTypeArg								&InFourth,
			size_t											 InDestPartStart			= 0
		);	

		//
		// Set part of the column of the given matrix to the given vector	
		// Only the specified part of the matrix column is updated.
		//
		// Arguments:
		//		InDestPartStart - start index of the column to be updated
		//
		template
		<
			class MatrixFloatTypeArg, size_t MatrixSize,
			class VecFloatTypeArg, size_t VecSize
		>
		void SetColumnPart
		(
			SquareMatrix<MatrixFloatTypeArg, MatrixSize>   *pOutMatrix,
			size_t											InColumnIndex,
			const Vec<VecFloatTypeArg, VecSize>&			InColumnVec,
			size_t											InColumnVecSize			= InColumnVec.GetSize(),
			size_t											InDestPartStart			= 0
		);

		// 
		// Update part of the column of the given matrix from memory.		
		// Only the specified part of the matrix column is updated.
		//
		template
		<
			class MatrixFloatTypeArg, size_t MatrixSize,
			class SrcFloatTypeArg
		>
		void SetColumnPart_FromMemory
		(
			SquareMatrix<MatrixFloatTypeArg, MatrixSize>   *pOutMatrix,
			size_t											InColumnIndex,
			const SrcFloatTypeArg						   *pSrcColumn,
			size_t											InNumComponentsToUpdate,
			size_t											srcComponentStrideInBytes = sizeof(SrcFloatTypeArg),
			size_t											InDestPartStart			= 0
		);


		/************************************************************************************
		* Special matrix operations
		************************************************************************************/
		// Perform the bias 3D transformation on the given 4x4  square matrix;				
		// - It's assumed that the matrix is normalized (w component is nearly equal to 1.0)
		// - 3D vector is used as the bias vector;
		// - Matrix size must have size 4;
		template<class FloatTypeArg>
		void BiasNormalizedMatrix44(SquareMatrix<FloatTypeArg, 4> *pMatrix, const Vec<FloatTypeArg, 3>& InBiasVector);


		/************************************************************************************
		*
		* IMPLEMENATION
		*
		************************************************************************************/	





		/************************************************************************************
		* String representation functions IMPL
		************************************************************************************/
		template<class FloatTypeArg, int Size>
		std::string	ToDebugString				(const SquareMatrix<FloatTypeArg, Size>&	InMatrix, int InPrecisionDigs)
		{			
			return MatrixToDebugString
			(
				GetPointer(InMatrix),
				InMatrix.GetSize(),
				InMatrix.GetComponentSize(),
				InMatrix.GetColumnSize()
			);			
		}

		template<class FloatTypeArg, int Size>
		std::string	ToString					(const SquareMatrix<FloatTypeArg, Size>&	InMatrix, int InPrecisionDigs)
		{			
			return MatrixToString
			(
				GetPointer(InMatrix),
				InMatrix.GetSize(),
				InMatrix.GetComponentSize(),
				InMatrix.GetColumnSize()
			);
		}

		/************************************************************************************
		* Component set functions IMPL
		************************************************************************************/
		template<class FloatTypeArg, int Size>
		void SetComponent
		(
			SquareMatrix<FloatTypeArg, Size>			*pInMatrix,
			int											 InRowIndex,
			int											 InColumnIndex,
			const FloatTypeArg&							 InNewValue
		)
		{
			assert(pInMatrix != nullptr);
			assert(InRowIndex >= 0);
			assert(InRowIndex < pInMatrix->GetSize());
			assert(InColumnIndex >= 0);
			assert(InColumnIndex < pInMatrix->GetSize());
			SetComponent(&(pInMatrix->GetColumnArray()[InColumnIndex]),InRowIndex, InNewValue);
		}


		/************************************************************************************
		* Component read-only access functions IMPL
		************************************************************************************/
		// Get column vector object const reference
		template<class FloatTypeArg, int Size>
		const typename SquareMatrix<FloatTypeArg,Size>::ColumnVecType&		GetColumn
		(
			const SquareMatrix<FloatTypeArg, Size>&			InMatrix,	
			size_t											InColumnIndex
		)
		{
			assert(&InMatrix != nullptr);
			assert(InColumnIndex >= 0);
			assert(InColumnIndex < InMatrix.GetSize());
			return InMatrix.GetColumnArray()[InColumnIndex];
		}

		template<class FloatTypeArg, int Size>
		const typename SquareMatrix<FloatTypeArg,Size>::ColumnVecType&		GetXColumn
		(
			const SquareMatrix<FloatTypeArg, Size>&			InMatrix			
		)
		{
			return GetColumn(InMatrix, 0);
		}

		template<class FloatTypeArg, int Size>
		const typename SquareMatrix<FloatTypeArg,Size>::ColumnVecType&		GetYYColumn
		(
			const SquareMatrix<FloatTypeArg, Size>&			InMatrix			
		)
		{
			return GetColumn(InMatrix, 1)
		}

		template<class FloatTypeArg, int Size>
		const typename SquareMatrix<FloatTypeArg,Size>::ColumnVecType&		GetZZZColumn
		(
			const SquareMatrix<FloatTypeArg, Size>&			InMatrix			
		)
		{
			return GetColumn(InMatrix, 3);
		}

		// Get component of the matrix
		template<class FloatTypeArg, int SizeArg>
		const FloatTypeArg&			GetComponent
		(
			const SquareMatrix<FloatTypeArg, SizeArg>&		InMatrix,	
			size_t											InRowIndex, 
			size_t											InColumnIndex
		)
		{
			assert(InRowIndex >= 0);
			assert(InRowIndex < InMatrix.GetSize());
			assert(InColumnIndex >= 0);
			assert(InColumnIndex < InMatrix.GetSize());
			return GetComponent(InMatrix.GetColumnArray()[InColumnIndex], InRowIndex);
		}

		/************************************************************************************
		* Low-level access operations IMPL
		************************************************************************************/	
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetPointer				(const SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			assert(&InMatrix != nullptr);
			return GetPointer(&(InMatrix.GetColumnArray()[0]));	
		}

		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetPointer				(SquareMatrix<FloatTypeArg, Size>&					InMatrix)
		{
			assert(&InMatrix != nullptr);
			return GetPointer(&(InMatrix.GetColumnArray()[0]));	
		}

		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetColumnPointer		(const SquareMatrix<FloatTypeArg, Size>&			InMatrix, int InColumnIndex)
		{
			assert(&InMatrix != nullptr);
			assert(InColumnIndex >= 0);
			return GetPointer(&(InMatrix.GetColumnArray()[InColumnIndex]));
		}

		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetColumnPointer		(SquareMatrix<FloatTypeArg, Size>&			InMatrix, int InColumnIndex)
		{
			assert(&InMatrix != nullptr);			
			assert(InColumnIndex >= 0);
			return GetPointer(&(InMatrix.GetColumnArray()[InColumnIndex]));
		}

		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetXColumnPointer		(const SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			return GetColumnPointer(InMatrix, 0);
		}

		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetXColumnPointer		(SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			return GetColumnPointer(InMatrix, 0);
		}

		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetYYColumnPointer		(const SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			return GetColumnPointer(InMatrix, 1);
		}

		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetYYColumnPointer		(SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			return GetColumnPointer(InMatrix, 1);
		}

		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetZZZColumnPointer		(const SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			return GetColumnPointer(InMatrix, 2);
		}

		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetZZZColumnPointer		(SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			return GetColumnPointer(InMatrix, 2);
		}

		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetRowStartPointer			(const SquareMatrix<FloatTypeArg, Size>&			InMatrix, int InRowIndex)
		{
			assert(&InMatrix != nullptr);
			return &(InMatrix.GetColumnArray()[0][InRowIndex]);
		}
		
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetRowStartPointer			(SquareMatrix<FloatTypeArg, Size>&			InMatrix, int InRowIndex)
		{
			assert(&InMatrix != nullptr);
			return &(InMatrix.GetColumnArray()[0][InRowIndex]);
		}
		
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetRowXStartPointer			(const SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			return GetRowStartPointer(InMatrix, 0);
		}
		
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetRowXStartPointer			(SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			return GetRowStartPointer(InMatrix, 0);
		}
		
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetRowYYStartPointer			(const SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			return GetRowStartPointer(InMatrix, 1);
		}
		
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetRowYYStartPointer			(SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			return GetRowStartPointer(InMatrix, 1);
		}
		
		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetRowZZZStartPointer			(const SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			return GetRowStartPointer(InMatrix, 2);
		}
		
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetRowZZZStartPointer			(SquareMatrix<FloatTypeArg, Size>&			InMatrix)
		{
			return GetRowStartPointer(InMatrix, 2);
		}

		template<class FloatTypeArg, int Size>
		const FloatTypeArg		*GetComponentPointer			(const SquareMatrix<FloatTypeArg, Size>&	InMatrix, int InRowIndex, int InColumnIndex)
		{
			assert(&InMatrix != nullptr);
			const FloatTypeArg *pColumn =  GetColumnPointer(InMatrix, InColumnIndex);
			const FloatTypeArg *pResComponent = Util::AddPtr(pColumn, InRowIndex * InMatrix.GetComponentSize());
			return pResComponent;
		}
		
		template<class FloatTypeArg, int Size>
		FloatTypeArg			*GetComponentPointer			(SquareMatrix<FloatTypeArg, Size>&			InMatrix, int InRowIndex, int InColumnIndex)
		{
			assert(&InMatrix != nullptr);		
			FloatTypeArg *pColumn =  GetColumnPointer(InMatrix, InColumnIndex);
			FloatTypeArg *pResComponent = Util::AddPtr(pColumn, InRowIndex * InMatrix.GetComponentSize());
			return pResComponent;
		}


		/************************************************************************************
		* Equality testing
		************************************************************************************/
		// Checking equality of two matrices
		// (both matrices must have equal sizes)
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		bool operator==
		(
			const SquareMatrix<DestFloatTypeArg, Size>&			InFirst,
			const SquareMatrix<DestFloatTypeArg, Size>&			InSecond
		)
		{
			return AreSubmatricesEqual
			(
				InFirst, 
				InSecond, 
				InFirst.GetSize(), 
				InFirst.GetSize()
			);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		bool operator!=
		(
			const SquareMatrix<DestFloatTypeArg, Size>&			InFirst,
			const SquareMatrix<DestFloatTypeArg, Size>&			InSecond
		)
		{
			return !(InFirst == InSecond);
		}

		// Checking that two submatrices of the given size are equal
		template
		<
			class DestFloatTypeArg, size_t DestSize,
			class SrcFloatTypeArg, size_t SrcSize
		>
		bool AreSubmatricesEqual
		(
			const SquareMatrix<DestFloatTypeArg, DestSize>&			InFirst,
			const SquareMatrix<DestFloatTypeArg, SrcSize>&			InSecond,
			size_t InSubmatrixNumRows,
			size_t InSubmatrixNumColumns,
			size_t InFirstStartRow,
			size_t InFirstStartColumn,
			size_t InSecondStartRow,
			size_t InSecondStartColumn
		)
		{
			assert (&InFirst != nullptr);
			assert (&InSecond != nullptr);
			assert (InSubmatrixNumRows > 0);
			assert (InSubmatrixNumRows < InFirst.GetSize());
			assert (InSubmatrixNumRows < InSecond.GetSize());
			assert (InSubmatrixNumColumns > 0);
			assert (InSubmatrixNumColumns < InFirst.GetSize());
			assert (InSubmatrixNumColumns < InSecond.GetSize())
			assert (InFirstStartRow >= 0);
			assert (InFirstStartRow < InFirst.GetSize());
			assert (InFirstStartColumn >= 0);
			assert (InFirstStartColumn < InFirst.GetSize());
			assert (InSecondStartRow >= 0);
			assert (InSecondStartRow < InSecond.GetSize());
			assert (InSecondStartColumn >= 0);
			assert (InSecondStartColumn < InSecond.GetSize());
			assert (!"AreSubmatricesEqual for SquareMatrix not yet implemented");
			return false;
		}

		/************************************************************************************
		* Matrix operations
		************************************************************************************/
		// Multiplying on scalar
		template<class DestFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>&	operator*=			
		(
			SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix,
			const double&										InCoeff
		)
		{
			DestFloatTypeArg *pDestMatrixPointer = GetPointer(InDestMatrix);
			MultScalar
			(
				pDestMatrixPointer, 
				InCoeff, 
				InDestMatrix.GetSize(),				 				
				InDestMatrix.GetColumnSize(), 				
				InDestMatrix.GetComponentSize()
			);
			return InDestMatrix;
		}

		// Division by scalar
		template<class DestFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>&	operator/=			
		(
			SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix,
			const double&										InCoeff
		)
		{
			DestFloatTypeArg *pDestMatrixPointer = GetPointer(InDestMatrix);
			DivScalar
			(
				pDestMatrixPointer, 
				InCoeff, 
				InDestMatrix.GetSize(),				
				InDestMatrix.GetColumnSize(), 
				InDestMatrix.GetComponentSize()
			);
			return InDestMatrix;
		}		

		// Get matrix multiplied by the scalar
		template<class DestFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator*			
		(
			const SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix,
			const double&											InCoeff
		)
		{
			SquareMatrix<DestFloatTypeArg, Size>	res_matrix(InDestMatrix);
			res_matrix *= InCoeff;
			return									res_matrix;
		}

		template<class DestFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator*			
		(
			const double&											InCoeff,
			const SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix
		)
		{
			return InDestMatrix * InCoeff;
		}

		// Get matrix divided by the given coeff
		template<class DestFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator/			
		(
			const SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix,
			const double&											InCoeff
		)
		{
			SquareMatrix<DestFloatTypeArg, Size>	res_matrix(InDestMatrix);
			res_matrix /= InCoeff;
			return									res_matrix;
		}

		// Get the negated matrix
		template<class DestFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator-			
		(
			const SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix			
		)
		{
			SquareMatrix<DestFloatTypeArg, Size>	res_matrix;
			DestFloatTypeArg					   *pMatrixPointer = GetPointer(res_matrix);
			Negate
			(
				pMatrixPointer, 
				InDestMatrix.GetSize(), 
				InDestMatrix.GetColumnSize(), 
				InDestMatrix.GetComponentSize()
			);
			return res_matrix;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>&	operator+=			
		(
			SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, Size>&			InSrcMatrix
		)
		{
			AddMatrix_To
			(
				GetPointer(InDestMatrix), 
				GetPointer(InSrcMatrix), 
				InDestMatrix.GetSize(), 
				InDestMatrix.GetSize(),
				InDestMatrix.GetColumnSize(),
				InSrcMatrix.GetColumnSize(),
				InDestMatrix.GetComponentSize(),
				InSrcMatrix.GetComponentSize()
			);
			return InDestMatrix;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>&	operator-=			
		(
			SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, Size>&			InSrcMatrix
		)
		{
			SubtractMatrix_From
			(
				GetPointer(InDestMatrix), 
				GetPointer(InSrcMatrix), 
				InDestMatrix.GetSize(), 
				InDestMatrix.GetSize(),
				InDestMatrix.GetColumnSize(),
				InSrcMatrix.GetColumnSize(),
				InDestMatrix.GetComponentSize(),
				InSrcMatrix.GetComponentSize()
			);
			return InDestMatrix;
		}

		// Multiplying matrix on matrix
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>&	operator*=			
		(
			SquareMatrix<DestFloatTypeArg, Size>&				InDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, Size>&			InSrcMatrix
		)
		{
			// NOT YET IMPL:
			//SquareMatrix<DestFloatTypeArg, Size> res_matrix(InDestMatrix);
			//res_matrix *= InSrcMatrix;
			//InDestMatrix = res_matrix;
			return InDestMatrix;
		}

		
		// Get the sum of two equal-sized matrices
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator+
		(
			const SquareMatrix<DestFloatTypeArg, Size>&			InDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, Size>&			InSrcMatrix
		)
		{
			SquareMatrix<DestFloatTypeArg, Size> res_matrix(InDestMatrix);
			res_matrix += InSrcMatrix;
			return res_matrix;
		}

		// Get the difference of two equal-sized matrices
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator-
		(
			const SquareMatrix<DestFloatTypeArg, Size>&			InDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, Size>&			InSrcMatrix
		)
		{
			SquareMatrix<DestFloatTypeArg, Size> res_matrix (InDestMatrix);
			res_matrix -= InSrcMatrix;
			return res_matrix;
		}

		// Get the multiple of two equal-sized matrices
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t Size>
		SquareMatrix<DestFloatTypeArg, Size>	operator*
		(
			const SquareMatrix<DestFloatTypeArg, Size>&			InSrcMatrix,
			const SquareMatrix<SrcFloatTypeArg, Size>&			InSrcOneMatrix
		)
		{
			assert(&InSrcMatrix != nullptr);
			assert(&InSrcOneMatrix != nullptr);
			SquareMatrix<DestFloatTypeArg, Size> res_matrix;
			// WARNING: The row byte stride and column byte stride are swapped delibarately
			// (the reason is that the matrices are column-major)
			MatrixCopyMult
			(
				GetPointer(res_matrix), 
				GetPointer(InSrcMatrix),
				GetPointer(InSrcOneMatrix),
				InSrcMatrix.GetSize(), InSrcMatrix.GetSize(),
				res_matrix.GetComponentSize(), 
				InSrcMatrix.GetComponentSize(), 			
				InSrcOneMatrix.GetComponentSize(), 	
				res_matrix.GetColumnSize(), 			
				InSrcMatrix.GetColumnSize(),
				InSrcOneMatrix.GetColumnSize()		
			);
			return res_matrix;
		}


		template<class DestFloatTypeArg, size_t DestSize, class SrcFloatTypeArg, size_t SrcSize>
		void Add_To
		(
			SquareMatrix<DestFloatTypeArg,DestSize>				*pDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, SrcSize>		&srcMatrix,
			int													 InNumRows,
			int													 InNumColumns
		)
		{
			assert(pDestMatrix != nullptr);
			assert(&srcMatrix != nullptr);
			assert(InNumRows > 1);
			assert(InNumRows < pDestMatrix->GetSize());
			assert(InNumRows < srcMatrix.GetSize());
			assert(InNumColumns > 1);
			assert(InNumColumns < pDestMatrix->GetSize());
			assert(InNumColumns < srcMatrix.GetSize());
			AddMatrix_To
			(
				GetPointer(*pDestMatrix),
				GetPointer(srcMatrix),
				InNumRows, InNumColumns,
				pDestMatrix->GetColumnSize(),
				srcMatrix.GetColumnSize(),
				pDestMatrix->GetComponentSize(),
				srcMatrix.GetComponentSize()
			);
		}

		/************************************************************************************
		* Matrix operations implementation - multiplication matrix by vector with assignment
		************************************************************************************/
		// Same size matrix multiplication
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t SizeArg>
		Vec<DestFloatTypeArg, SizeArg>& operator*=
		(
			Vec<DestFloatTypeArg, SizeArg>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, SizeArg>& InRight
		)
		{
			for(int i = 0; i < SizeArg; i++)
			{
				const auto& rightColumn         = GetColumn(InRight, i);
				auto new_component_value        = Dot(InLeft, rightColumn);
				SetComponent(&InLeft, i, new_component_value);
			}
			return InLeft;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, 2>& operator*=
		(
			Vec<DestFloatTypeArg, 2>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, 3>& InRight
		)
		{
			auto new_X            = Dot2(InLeft, GetColumn(InRight, 0));
			auto new_YY           = Dot2(InLeft, GetColumn(InRight, 1));

			SetX(&InLeft, new_X);
			SetYY(&InLeft, new_YY);
			return InLeft;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, 2>& operator*=
		(
			Vec<DestFloatTypeArg, 2>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, 4>& InRight
		)
		{
			auto new_X            = Dot2(InLeft, GetColumn(InRight, 0));
			auto new_YY           = Dot2(InLeft, GetColumn(InRight, 1));

			SetX(&InLeft, new_X);
			SetYY(&InLeft, new_YY);
			return InLeft;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, 3>& operator*=
		(
			Vec<DestFloatTypeArg, 3>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, 4>& InRight
		)
		{
			auto new_X            = Dot4_wOne(InLeft, GetColumn(InRight, 0));
			auto new_YY           = Dot4_wOne(InLeft, GetColumn(InRight, 1));
			auto new_ZZZ          = Dot4_wOne(InLeft, GetColumn(InRight, 2));

			SetX(&InLeft, new_X);
			SetYY(&InLeft, new_YY);
			SetZZZ(&InLeft, new_ZZZ);
			return InLeft;
		}


		/************************************************************************************
		* Matrix operations implementation - multiplication matrix by vector
		************************************************************************************/
		// Same size matrix multiplication
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t SizeArg>
		Vec<DestFloatTypeArg, SizeArg> operator*
		(
			const Vec<DestFloatTypeArg, SizeArg>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, SizeArg>& InRight
		)
		{
			Vec<DestFloatTypeArg, SizeArg> res { InLeft };
			res *= InRight;
			return res;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, 2> operator*
		(
			const Vec<DestFloatTypeArg, 2>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, 3>& InRight
		)	
		{
			Vec<DestFloatTypeArg, 2> res { InLeft };
			res *= InRight;
			return res;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, 2> operator*
		(
			const Vec<DestFloatTypeArg, 2>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, 4>& InRight
		)
		{
			Vec<DestFloatTypeArg, 2> res { InLeft };
			res *= InRight;
			return res;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, 3> operator*
		(
			const Vec<DestFloatTypeArg, 3>& InLeft,
			const SquareMatrix<SrcFloatTypeArg, 4>& InRight
		)
		{
			Vec<DestFloatTypeArg, 3> res { InLeft };
			res *= InRight;
			return res;
		}

		/************************************************************************************
		* Matrix operations part II IMPLEMENTATION
		************************************************************************************/
		/*
		// Description:
		//		Compute determinant of the submatrix of the given matrix
		// Arguments:
		//		InSize - size of the submatrix;
		//		InStartRow - Upper row of the submatrix;
		//		InStartColumn - Left column of the submatrix.
		template<class FloatTypeArg, size_t Size>
		FloatTypeArg Det
		(
			const SquareMatrix<FloatTypeArg, Size>&				InMatrix, 
			int													InSize,
			int													InStartRow,
			int													InStartColumn
		)
		{
		}
		*/

		template<class FloatTypeArg, size_t Size>
		FloatTypeArg Det
		(
			const SquareMatrix<FloatTypeArg, Size>&				InMatrix 
		)
		{
			return DetSubmatrix(InMatrix, Size, 0, 0);
		}

		// Implementing the functions for calculating the determinant of the given submatrix
		// using the template specialization
		template<>
		inline float DetSubmatrix<float, 2>
		(
			const SquareMatrix<float, 2>&						InMatrix, 
			int													InSize,
			int													InStartRow,
			int													InStartColumn
		)
		{
			return DetSubmatrix2x2(InMatrix, InStartRow, InStartColumn);
		}

		template<>
		inline float DetSubmatrix<float, 3>
		(
			const SquareMatrix<float, 3>&						InMatrix, 
			int													InSize,
			int													InStartRow,
			int													InStartColumn
		)
		{
			return DetSubmatrix3x3(InMatrix, InStartRow, InStartColumn);
		}

		template<>
		inline float DetSubmatrix<float, 4>
		(
			const SquareMatrix<float, 4>&						InMatrix, 
			int													InSize,
			int													InStartRow,
			int													InStartColumn
		)
		{
			return DetSubmatrix4x4(InMatrix, InStartRow, InStartColumn);
		}

		template<>
		inline double DetSubmatrix<double, 2>
		(
			const SquareMatrix<double, 2>&						InMatrix, 
			int													InSize,
			int													InStartRow,
			int													InStartColumn
		)
		{
			return DetSubmatrix2x2(InMatrix, InStartRow, InStartColumn);
		}

		template<>
		inline double DetSubmatrix<double, 3>
		(
			const SquareMatrix<double, 3>&						InMatrix, 
			int													InSize,
			int													InStartRow,
			int													InStartColumn
		)
		{
			return DetSubmatrix3x3(InMatrix, InStartRow, InStartColumn);
		}

		template<>
		inline double DetSubmatrix<double, 4>
		(
			const SquareMatrix<double, 4>&						InMatrix, 
			int													InSize,
			int													InStartRow,
			int													InStartColumn
		)
		{
			return DetSubmatrix4x4(InMatrix, InStartRow, InStartColumn);
		}

		// Inverse the given matrix
		template<class FloatTypeArg>
		void Inverse
		(
			SquareMatrix<FloatTypeArg, 2>					*pInOutMatrix
		)
		{
			SquareMatrix<FloatTypeArg, 2> result_matrix;
			auto det = Det2x2(*pInOutMatrix);
			InverseCopy_UsePrecomputedDet(&result_matrix, pInOutMatrix, det);
			*pInOutMatrix = result_matrix;
		}

		template<class FloatTypeArg>
		void Inverse
		(
			SquareMatrix<FloatTypeArg, 3>					*pInOutMatrix
		)
		{
			SquareMatrix<FloatTypeArg, 3> result_matrix;
			auto det = Det3x3(*pInOutMatrix);
			InverseCopy_UsePrecomputedDet(&result_matrix, pInOutMatrix, det);
			*pInOutMatrix = result_matrix;
		}

		template<class FloatTypeArg>
		void Inverse
		(
			SquareMatrix<FloatTypeArg, 4>					*pInOutMatrix
		)
		{
			SquareMatrix<FloatTypeArg, 4> result_matrix;
			auto det = Det4x4(*pInOutMatrix);
			InverseCopy_UsePrecomputedDet(&result_matrix, pInOutMatrix, det);
			*pInOutMatrix = result_matrix;
		}

		// Inverse the given matrix using the precomputed determinant value
		template<class FloatTypeArg>
		void Inverse_UsePrecomputedDet
		(
			SquareMatrix<FloatTypeArg, 2>					*pInOutMatrix,
			const FloatTypeArg									&InDet
		)
		{
			SquareMatrix<FloatTypeArg, 2> result_matrix;
			InverseCopy_UsePrecomputedDet(&result_matrix, pInOutMatrix, InDet);
			*pInOutMatrix = result_matrix;
		}
		template<class FloatTypeArg>
		void Inverse_UsePrecomputedDet
		(
			SquareMatrix<FloatTypeArg, 3>					*pInOutMatrix,
			const FloatTypeArg									&InDet
		)
		{
			SquareMatrix<FloatTypeArg, 3> result_matrix;
			InverseCopy_UsePrecomputedDet(&result_matrix, pInOutMatrix, InDet);
			*pInOutMatrix = result_matrix;
			return pInOutMatrix;
		}
		template<class FloatTypeArg>
		void Inverse_UsePrecomputedDet
		(
			SquareMatrix<FloatTypeArg, 4>					*pInOutMatrix,
			const FloatTypeArg									&InDet
		)
		{
			SquareMatrix<FloatTypeArg, 4> result_matrix;
			InverseCopy_UsePrecomputedDet(&result_matrix, pInOutMatrix, InDet);
			*pInOutMatrix = result_matrix;
			return pInOutMatrix;
		}

		// Inverse the given matrix using the precomputed determinant value
		// and place the copy into the given matrix
		template<class FloatTypeArg>
		void InverseCopy_UsePrecomputedDet
		(
			SquareMatrix<FloatTypeArg, 2>					*pDestMatrix,
			const SquareMatrix<FloatTypeArg, 2>				*pSrcMatrix,
			const FloatTypeArg									&InDet
		)
		{
			auto destRowStride = pDestMatrix->GetComponentSize();
			auto srcRowStride = pSrcMatrix->GetComponentSize();
			auto destComponentStride = pDestMatrix->GetColumnSize();
			auto srcComponentStride = pSrcMatrix->GetColumnSize();
			MatrixCopyInverse_UsePrecomputedDet22(GetPointer(*pDestMatrix), GetPointer(*pSrcMatrix), InDet, destRowStride, srcRowStride, destComponentStride, srcComponentStride);
		}

		template<class FloatTypeArg>
		void InverseCopy_UsePrecomputedDet
		(
			SquareMatrix<FloatTypeArg, 3>					*pDestMatrix,
			const SquareMatrix<FloatTypeArg, 3>				*pSrcMatrix,
			const FloatTypeArg									&InDet
		)
		{
			auto destRowStride = pDestMatrix->GetComponentSize();
			auto srcRowStride = pSrcMatrix->GetComponentSize();
			auto destComponentStride = pDestMatrix->GetColumnSize();
			auto srcComponentStride = pSrcMatrix->GetColumnSize();
			MatrixCopyInverse_UsePrecomputedDet33(GetPointer(*pDestMatrix), GetPointer(*pSrcMatrix), InDet, destRowStride, srcRowStride, destComponentStride, srcComponentStride);
		}

		template<class FloatTypeArg>
		void InverseCopy_UsePrecomputedDet
		(
			SquareMatrix<FloatTypeArg, 4>					*pDestMatrix,
			const SquareMatrix<FloatTypeArg, 4>				*pSrcMatrix,
			const FloatTypeArg									&InDet
		)
		{
			auto destRowStride = pDestMatrix->GetComponentSize();
			auto srcRowStride = pSrcMatrix->GetComponentSize();
			auto destComponentStride = pDestMatrix->GetColumnSize();
			auto srcComponentStride = pSrcMatrix->GetColumnSize();
			MatrixCopyInverse_UsePrecomputedDet44(GetPointer(*pDestMatrix), GetPointer(*pSrcMatrix), InDet, destRowStride, srcRowStride, destComponentStride, srcComponentStride);
		}
		
		// Compute determinant of the 2*2 matrix
		template<class FloatTypeArg>
		FloatTypeArg Det2x2
		(
			const SquareMatrix<FloatTypeArg, 2>& InMatrix
		)
		{						
			return Det2x2
			(
				GetComponent(InMatrix, 0, 0), GetComponent(InMatrix, 0, 1),
				GetComponent(InMatrix, 1, 0), GetComponent(InMatrix, 1, 1)
			);
		}

		template<class FloatTypeArg>
		FloatTypeArg DetSubmatrix2x2
		(
			const SquareMatrix<FloatTypeArg, 2>& InMatrix,
			int InRow,
			int InColumn
		)
		{						
			return Det2x2
			(
				GetComponent(InMatrix, InRow + 0, InColumn + 0), GetComponent(InMatrix, InRow + 0, InColumn + 1),
				GetComponent(InMatrix, InRow + 1, InColumn + 0), GetComponent(InMatrix, InRow + 1, InColumn + 1)
			);
		}

		// Compute determinant of the 3*3 matrix
		template<class FloatTypeArg>
		FloatTypeArg Det3x3
		(
			const SquareMatrix<FloatTypeArg, 3>&				InMatrix
		)
		{
			return Det3x3
			(
				GetComponent(InMatrix, 0, 0), GetComponent(InMatrix, 0, 1), GetComponent(InMatrix, 0, 2),
				GetComponent(InMatrix, 1, 0), GetComponent(InMatrix, 1, 1), GetComponent(InMatrix, 1, 2),
				GetComponent(InMatrix, 2, 0), GetComponent(InMatrix, 2, 1), GetComponent(InMatrix, 2, 2)				
			);
		}

		template<class FloatTypeArg>
		FloatTypeArg DetSubmatrix3x3
		(
			const SquareMatrix<FloatTypeArg, 3>&				InMatrix,
			int													InRow,
			int													InColumn
		)
		{
			return Det3x3
			(
				GetComponent(InMatrix, InRow + 0, InColumn + 0), GetComponent(InMatrix, InRow + 0, InColumn + 1), GetComponent(InMatrix, InRow + 0, InColumn + 2),
				GetComponent(InMatrix, InRow + 1, InColumn + 0), GetComponent(InMatrix, InRow + 1, InColumn + 1), GetComponent(InMatrix, InRow + 1, InColumn + 2),
				GetComponent(InMatrix, InRow + 2, InColumn + 0), GetComponent(InMatrix, InRow + 2, InColumn + 1), GetComponent(InMatrix, InRow + 2, InColumn + 2)				
			);
		}
		

		// Compute determinant of the 4*4 matrix
		template<class FloatTypeArg>
		FloatTypeArg Det4x4
		(
			const SquareMatrix<FloatTypeArg, 4>&				InMatrix
		)
		{
			return Det4x4
			(
				GetComponent(InMatrix, 0, 0), GetComponent(InMatrix, 0, 1), GetComponent(InMatrix, 0, 2), GetComponent(InMatrix, 0, 3),
				GetComponent(InMatrix, 1, 0), GetComponent(InMatrix, 1, 1), GetComponent(InMatrix, 1, 2), GetComponent(InMatrix, 1, 3),
				GetComponent(InMatrix, 2, 0), GetComponent(InMatrix, 2, 1), GetComponent(InMatrix, 2, 2), GetComponent(InMatrix, 2, 3),
				GetComponent(InMatrix, 3, 0), GetComponent(InMatrix, 3, 1), GetComponent(InMatrix, 3, 2), GetComponent(InMatrix, 3, 3)
			);
		}

		template<class FloatTypeArg>
		FloatTypeArg DetSubmatrix4x4
		(
			const SquareMatrix<FloatTypeArg, 4>&				InMatrix,
			int													InRow,
			int													InColumn
		)
		{
			return Det4x4
			(
				GetComponent(InMatrix, InRow + 0, InColumn + 0), GetComponent(InMatrix, InRow + 0, InColumn + 1), GetComponent(InMatrix, InRow + 0, InColumn + 2), GetComponent(InMatrix, InRow + 0, InColumn + 3),
				GetComponent(InMatrix, InRow + 1, InColumn + 0), GetComponent(InMatrix, InRow + 1, InColumn + 1), GetComponent(InMatrix, InRow + 1, InColumn + 2), GetComponent(InMatrix, InRow + 1, InColumn + 3),
				GetComponent(InMatrix, InRow + 2, InColumn + 0), GetComponent(InMatrix, InRow + 2, InColumn + 1), GetComponent(InMatrix, InRow + 2, InColumn + 2), GetComponent(InMatrix, InRow + 2, InColumn + 3),
				GetComponent(InMatrix, InRow + 3, InColumn + 0), GetComponent(InMatrix, InRow + 3, InColumn + 1), GetComponent(InMatrix, InRow + 3, InColumn + 2), GetComponent(InMatrix, InRow + 3, InColumn + 3)
			);
		}

		template<class FloatTypeArg, size_t Size>
		void TransposeCopy
		(
			SquareMatrix<FloatTypeArg, Size>					*pOutMatrix,
			const SquareMatrix<FloatTypeArg, Size>				&InMatrix
		)
		{
			assert(pOutMatrix != nullptr);
			assert(&InMatrix != nullptr);

			// NOTE: 
			// We used the fact that tranposing columns is the same as transposing rows		
			MatrixTransposeCopy
			(
				GetPointer(*pOutMatrix),
				GetPointer(InMatrix),
				Size,
				pOutMatrix->GetColumnSize(),
				InMatrix.GetColumnSize(),
				pOutMatrix->GetComponentSize(),
				InMatrix.GetComponentSize()
			);
		}

		template<class FloatTypeArg, size_t Size>
		void Transpose
		(
			SquareMatrix<FloatTypeArg, Size>					*pOutMatrix
		)
		{
			assert(pOutMatrix != nullptr);
			// NOTE: 
			// We used the fact that tranposing columns is the same as transposing rows			
			MatrixTranspose
			(
				GetPointer(*pOutMatrix), 
				Size, 
				pOutMatrix->GetColumnSize(), 
				pOutMatrix->GetComponentSize()
			);
		}

		template<class DestFloatTypeArg, size_t DestNumElemsArg>		
		void ResetZero_n
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix			
		)
		{
			assert(pMatrix != nullptr);
			ZeroMatrix
			(
				GetPointer(*pMatrix), 
				pMatrix->GetSize(), pMatrix->GetSize(), 
				pMatrix->GetColumnSize(), 
				pMatrix->GetComponentSize()
			);
		}

		template<class DestFloatTypeArg, size_t DestNumElemsArg>		
		void SetSubmatrixIdentity_n
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			size_t													 InSize
		)
		{
			assert(pMatrix != nullptr);
			SetIdentityMatrix
			(
				GetPointer(*pMatrix),
				InSize,
				pMatrix->GetColumnSize(),
				pMatrix->GetComponentSize()
			);
		}

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_n
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			DestFloatTypeArg										 componentValue 			
		)
		{
			assert(pMatrix != nullptr);
			FillMatrix
			(
				GetPointer(*pMatrix), 
				pMatrix->GetSize(), 
				componentValue, 
				pMatrix->GetColumnSize(), 
				pMatrix->GetComponentSize()
			);
		}

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_FromColumnMajor
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>	*pMatrix,
			const DestFloatTypeArg& In00, const DestFloatTypeArg& In01,
			const DestFloatTypeArg& In10, const DestFloatTypeArg& In11
		)
		{
			assert(pMatrix != nullptr);	
			ResetMatrix2x2
			(
				GetPointer(*pMatrix),
				pMatrix->GetSize(),
				In00, In01,
				In10, In11,
				pMatrix->GetColumnSize(),
				pMatrix->GetComponentSize()
			);
		}

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_FromRowMajor
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const DestFloatTypeArg& In00, const DestFloatTypeArg& In01, const DestFloatTypeArg& In02,
			const DestFloatTypeArg& In10, const DestFloatTypeArg& In11, const DestFloatTypeArg& In12,
			const DestFloatTypeArg& In20, const DestFloatTypeArg& In21, const DestFloatTypeArg& In22
		)
		{
			assert(pMatrix != nullptr);	
			ResetMatrix3x3
			(
				GetPointer(*pMatrix),
				pMatrix->GetSize(),
				In00, In10, In20,
				In01, In11, In21,
				In02, In12, In22,
				pMatrix->GetColumnSize(),
				pMatrix->GetComponentSize()
			);
		}

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_FromColumnMajor
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const DestFloatTypeArg& In00, const DestFloatTypeArg& In01, const DestFloatTypeArg& In02,
			const DestFloatTypeArg& In10, const DestFloatTypeArg& In11, const DestFloatTypeArg& In12,
			const DestFloatTypeArg& In20, const DestFloatTypeArg& In21, const DestFloatTypeArg& In22
		)
		{
			assert(pMatrix != nullptr);
			ResetMatrix3x3
			(
				GetPointer(*pMatrix),
				pMatrix->GetSize(),
				In00, In01, In02,
				In10, In11, In12,
				In20, In21, In22,
				pMatrix->GetColumnSize(),
				pMatrix->GetComponentSize()
			);
		}

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_FromRowMajor
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const DestFloatTypeArg& In00, const DestFloatTypeArg& In01, const DestFloatTypeArg& In02, const DestFloatTypeArg& In03,
			const DestFloatTypeArg& In10, const DestFloatTypeArg& In11, const DestFloatTypeArg& In12, const DestFloatTypeArg& In13,
			const DestFloatTypeArg& In20, const DestFloatTypeArg& In21, const DestFloatTypeArg& In22, const DestFloatTypeArg& In23,
			const DestFloatTypeArg& In30, const DestFloatTypeArg& In31, const DestFloatTypeArg& In32, const DestFloatTypeArg& In33
		)
		{
			assert(pMatrix != nullptr);
			ResetMatrix4x4
			(
				GetPointer(*pMatrix),
				pMatrix->GetSize(),
				In00, In10, In20, In30,
				In01, In11, In21, In31,
				In02, In12, In22, In32,
				In03, In13, In23, In33,
				pMatrix->GetColumnSize(),
				pMatrix->GetComponentSize()
			);
		}

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_FromColumnMajor
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const DestFloatTypeArg& In00, const DestFloatTypeArg& In01, const DestFloatTypeArg& In02, const DestFloatTypeArg& In03,
			const DestFloatTypeArg& In10, const DestFloatTypeArg& In11, const DestFloatTypeArg& In12, const DestFloatTypeArg& In13,
			const DestFloatTypeArg& In20, const DestFloatTypeArg& In21, const DestFloatTypeArg& In22, const DestFloatTypeArg& In23,
			const DestFloatTypeArg& In30, const DestFloatTypeArg& In31, const DestFloatTypeArg& In32, const DestFloatTypeArg& In33
		)
		{
			assert(pMatrix != nullptr);
			ResetMatrix4x4
			(
				GetPointer(*pMatrix),
				pMatrix->GetSize(),
				In00, In01, In02, In03,
				In10, In11, In12, In13,
				In20, In21, In22, In23,
				In30, In31, In32, In33,
				pMatrix->GetColumnSize(),
				pMatrix->GetComponentSize()
			);
		}

		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Reset_FromRowMajor
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const DestFloatTypeArg& In00, const DestFloatTypeArg& In01,
			const DestFloatTypeArg& In10, const DestFloatTypeArg& In11
		)
		{
			assert(pMatrix != nullptr);	
			ResetMatrix2x2
			(
				GetPointer(*pMatrix),
				pMatrix->GetSize(),
				In00, In10,
				In01, In11,
				pMatrix->GetColumnSize(),
				pMatrix->GetComponentSize()
			);
		}

		template
		<
			class DestFloatTypeArg, size_t DestNumElemsArg, 
			class SrcFloatTypeArg, size_t SrcNumElemsArg
		>
		void ResetFromMatrix
		(
			size_t													InSrcSize,
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pDestMatrix,
			const SquareMatrix<SrcFloatTypeArg, SrcNumElemsArg>		&InSrcMatrix	
		)
		{
			assert(InSrcSize > 0);
			assert(pDestMatrix != nullptr);
			assert(&InSrcMatrix != nullptr);
			ResetMatrixFromMatrix_n
			(
				GetPointer(*pDestMatrix),
				GetPointer(InSrcMatrix),
				pDestMatrix->GetSize(),
				InSrcSize,
				pDestMatrix->GetColumnSize(),
				InSrcMatrix.GetColumnSize(),
				pDestMatrix->GetComponentSize(),
				InSrcMatrix.GetComponentSize()
			);
		}

		template<class DestFloatTypeArg, size_t DestNumElemsArg, class SrcFloatTypeArg, size_t SrcNumComponents>
		void Reset_FromRows
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow1
		)
		{
			assert(DestNumElemsArg >= 2);
			assert(pMatrix != nullptr);
			ResetMatrix2x2_FromRows
			(
				GetPointer(*pMatrix),
				pMatrix->GetSize(),
				GetPointer(&InRow0),
				GetPointer(&InRow1),
				pMatrix->GetComponentSize(),	// OLD VALUE: pMatrix->GetColumnSize(),				
				pMatrix->GetColumnSize(),		// OLD VALUE: pMatrix->GetComponentSize(),
				InRow0.GetComponentSize(),
				InRow1.GetComponentSize()
			);		
		}
		
		template<class DestFloatTypeArg, size_t DestNumElemsArg, class SrcFloatTypeArg, size_t SrcNumComponents>
		void Reset_FromRows
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow1,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow2
		)
		{	
			// REPAIRED BUGS: T
			//  Matrix transposition (REPAIRED: 12/08/2016, 10:34)
			assert(DestNumElemsArg >= 3);
			assert(pMatrix != nullptr);						
			ResetMatrix3x3_FromRows
			(
				GetPointer(*pMatrix), 
				pMatrix->GetSize(),
				GetPointer(&InRow0),
				GetPointer(&InRow1),
				GetPointer(&InRow2),
				pMatrix->GetComponentSize(),	// OLD VALUE: pMatrix->GetColumnSize(),				
				pMatrix->GetColumnSize(),		// OLD VALUE: pMatrix->GetComponentSize(),
				InRow0.GetComponentSize(),
				InRow1.GetComponentSize(),
				InRow2.GetComponentSize()
			);			

			//ResetMatrix3x3_FromRows
			//(
			//	GetPointer(*pMatrix), 
			//	pMatrix->GetSize(),
			//	GetPointer(&InRow0),
			//	GetPointer(&InRow1),
			//	GetPointer(&InRow2),
			//	pMatrix->GetColumnSize(),				
			//	pMatrix->GetComponentSize(),
			//	InRow0.GetComponentSize(),
			//	InRow1.GetComponentSize(),
			//	InRow2.GetComponentSize()
			//);			
		}

		template<class DestFloatTypeArg, size_t DestNumElemsArg, class SrcFloatTypeArg, size_t SrcNumComponents>
		void Reset_FromRows
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow1,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow2,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InRow3
		)
		{
			assert(pMatrix != nullptr);
			// BUG: REPAIRED
			ResetMatrix4x4_FromRows
			(
				GetPointer(*pMatrix), 
				pMatrix->GetSize(),
				GetPointer(&InRow0),
				GetPointer(&InRow1),
				GetPointer(&InRow2),
				GetPointer(&InRow3),
				pMatrix->GetComponentSize(), // OLD VALUE: pMatrix->GetColumnSize(),	
				pMatrix->GetColumnSize(),	// OLD VALUE: pMatrix->GetComponentSize(),				
				InRow0.GetComponentSize(),
				InRow1.GetComponentSize(),
				InRow2.GetComponentSize(),
				InRow3.GetComponentSize()
			);	
		}

		template<class DestFloatTypeArg, size_t DestNumElemsArg, class SrcFloatTypeArg, size_t SrcNumComponents>
		void Reset_FromColumns
		(
			SquareMatrix<DestFloatTypeArg, DestNumElemsArg>			*pMatrix,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InColumn0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InColumn1,
			const Vec<SrcFloatTypeArg, SrcNumComponents>&			 InColumn2
		)
		{
			assert(pMatrix != nullptr);
			ResetMatrix3x3_FromRows
			(
				GetPointer(*pMatrix), 
				pMatrix->GetSize(),
				GetPointer(&InColumn0),
				GetPointer(&InColumn1),
				GetPointer(&InColumn2),
				pMatrix->GetColumnSize(),				
				pMatrix->GetComponentSize(),
				InColumn0.GetComponentSize(),
				InColumn1.GetComponentSize(),
				InColumn2.GetComponentSize()
			);
		}

		/************************************************************************************
		* Special matrix operations IMPLEMENTATIONS
		************************************************************************************/		
		template<class FloatTypeArg>
		void BiasNormalizedMatrix44(SquareMatrix<FloatTypeArg, 4> *pMatrix, const Vec<FloatTypeArg, 3>& InBiasVector)
		{
			assert(pMatrix != nullptr);
			assert(&InBiasVector != nullptr);			
			const size_t row_index = 3; // index of row to update
			FloatTypeArg *pDestX = GetComponentPointer(*pMatrix, row_index, 0);
			FloatTypeArg *pDestYY = GetComponentPointer(*pMatrix, row_index, 1);
			FloatTypeArg *pDestZZZ = GetComponentPointer(*pMatrix, row_index, 2);
			*pDestX += GetX(InBiasVector);
			*pDestYY += GetYY(InBiasVector);
			*pDestZZZ += GetZZZ(InBiasVector);
		}


		/************************************************************************************
		* Member-function IMPLEMENTATION
		************************************************************************************/
		template<class FloatTypeArg, size_t SizeArg>		
		size_t SquareMatrix<FloatTypeArg, SizeArg>::GetComponentSize()
		{
			return sizeof(FloatTypeArg);
		}

		template<class FloatTypeArg, size_t SizeArg>
		size_t SquareMatrix<FloatTypeArg, SizeArg>::GetColumnSize()
		{
			return sizeof(ColumnComponentArrayType);
		}
				
		template<class FloatTypeArg, size_t SizeArg>
		SquareMatrix< FloatTypeArg, SizeArg>::SquareMatrix()
		{
			SetSubmatrixIdentity_n(this, GetSize());
		}		

		template<class FloatTypeArg, size_t SizeArg>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			const FloatTypeArg& In00, const FloatTypeArg& In01,
			const FloatTypeArg& In10, const FloatTypeArg& In11,						
			EMatrixRowMajor InSrcMatrixRowMajor
		)
		{
			Reset_FromRowMajor
			(
				this, 
				In00, In01,
				In10, In11
			);
		}

		template<class FloatTypeArg, size_t SizeArg>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			const FloatTypeArg& In00, const FloatTypeArg& In01,
			const FloatTypeArg& In10, const FloatTypeArg& In11,				
			EMatrixColumnMajor InSrcMatrixColumnMajor
		)
		{
			Reset_FromColumnMajor
			(
				this, 
				In00, In01,
				In10, In11
			);			
		}

		template<class FloatTypeArg, size_t SizeArg>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			const FloatTypeArg& In00, const FloatTypeArg& In01, const FloatTypeArg& In02,
			const FloatTypeArg& In10, const FloatTypeArg& In11, const FloatTypeArg& In12,
			const FloatTypeArg& In20, const FloatTypeArg& In21, const FloatTypeArg& In22,
			EMatrixRowMajor InSrcMatrixRowMajor
		)
		{
			Reset_FromRowMajor
			(
				this, 
				In00, In01, In02,
				In10, In11, In12,
				In20, In21, In22
			);	
		}

		template<class FloatTypeArg, size_t SizeArg>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			const FloatTypeArg& In00, const FloatTypeArg& In01, const FloatTypeArg& In02,
			const FloatTypeArg& In10, const FloatTypeArg& In11, const FloatTypeArg& In12,
			const FloatTypeArg& In20, const FloatTypeArg& In21, const FloatTypeArg& In22,
			EMatrixColumnMajor InSrcMatrixColumnMajor
		)
		{
			Reset_FromColumnMajor
			(
				this, 
				In00, In01, In02,
				In10, In11, In12,
				In20, In21, In22
			);	
		}

		template<class FloatTypeArg, size_t SizeArg>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			const FloatTypeArg& In00, const FloatTypeArg& In01, const FloatTypeArg& In02, const FloatTypeArg& In03,
			const FloatTypeArg& In10, const FloatTypeArg& In11, const FloatTypeArg& In12, const FloatTypeArg& In13,
			const FloatTypeArg& In20, const FloatTypeArg& In21, const FloatTypeArg& In22, const FloatTypeArg& In23,
			const FloatTypeArg& In30, const FloatTypeArg& In31, const FloatTypeArg& In32, const FloatTypeArg& In33,
			EMatrixRowMajor InSrcMatrixRowMajor
		)
		{
			Reset_FromRowMajor
			(
				this,
				In00, In01, In02, In03,
				In10, In11, In12, In13,
				In20, In21, In22, In23,
				In30, In31, In32, In33
			);
		}

		template<class FloatTypeArg, size_t SizeArg>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			const FloatTypeArg& In00, const FloatTypeArg& In01, const FloatTypeArg& In02, const FloatTypeArg& In03,
			const FloatTypeArg& In10, const FloatTypeArg& In11, const FloatTypeArg& In12, const FloatTypeArg& In13,
			const FloatTypeArg& In20, const FloatTypeArg& In21, const FloatTypeArg& In22, const FloatTypeArg& In23,
			const FloatTypeArg& In30, const FloatTypeArg& In31, const FloatTypeArg& In32, const FloatTypeArg& In33,
			EMatrixColumnMajor InSrcMatrixColumnMajor
		)
		{
			Reset_FromColumnMajor
			(
				this,
				In00, In01, In02, In03,
				In10, In11, In12, In13,
				In20, In21, In22, In23,
				In30, In31, In32, In33
			);
		}

		template<class FloatTypeArg, size_t SizeArg>
		template<class SrcFloatTypeArg, size_t SrcNumComponents>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow1,
			EMatrixRowMajor InSrcMatrixRowMajor
		)
		{
			Reset_FromRows
			(
				this, InRow0, InRow1
			);
		}
				
		template<class FloatTypeArg, size_t SizeArg>
		template<class SrcFloatTypeArg, size_t SrcNumComponents>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow1,
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow2,
			EMatrixRowMajor InSrcMatrixRowMajor
		)
		{
			Reset_FromRows
			(
				this, InRow0, InRow1, InRow2
			);
		}

		template<class FloatTypeArg, size_t SizeArg>
		template<class SrcFloatTypeArg, size_t SrcNumComponents>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn1,
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn2,
			EMatrixColumnMajor InSrcMatrixColumnMajor
		)
		{
			Reset_FromColumns
			(
				this, InRow0, InRow1, InRow2
			);
		}

		template<class FloatTypeArg, size_t SizeArg>
		template<class SrcFloatTypeArg, size_t SrcNumComponents>		
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow1,
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow2,				
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InRow3,
			EMatrixRowMajor InSrcMatrixRowMajor
		)
		{
			Reset_FromRows
			(
				this, InRow0, InRow1, InRow2, InRow3
			);
		}

		template<class FloatTypeArg, size_t SizeArg>
		template<class SrcFloatTypeArg, size_t SrcNumComponents>		
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn0,
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn1,
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn2,				
			const Vec<SrcFloatTypeArg, SrcNumComponents>& InColumn3,
			EMatrixColumnMajor InSrcMatrixColumnMajor
		)
		{	
			Reset_FromColumns
			(
				this, InColumn0, InColumn1, InColumn2, InColumn3
			);
		}
		
		template<class FloatTypeArg, size_t SizeArg>
		template<class RowVecIterator>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(			
			size_t InSize,
			RowVecIterator firstRowVecIt, RowVecIterator lastRowVecIt,
			EMatrixRowMajor InSrcMatrixRowMajor
		)
		{
			assert(InSize <= SizeArg);

			assert(!"Not yet impl");
			// 
			// IMPLEMENTATION:
			// 
			int row_index = 0;
			for(RowVecIterator row_iterator = firstRowVecIt; row_iterator != lastRowVecIt; ++row_iterator)
			{
				const auto& row_vector = *row_iterator;
				// Set row with index row-index so, that
				//		1) all components in range [0;InSize) are taken from the row vector
				//		2) all components in range [InSize; SizeArg] are equal to the components of the row of SizeArg identity matrix
				// CALL FUNCTION HERE
				row_index++;
			}
			// FILL EXTRA ROWS:			
			while(row_index < SizeArg)
			{
				// Every extra row is filled as vector of the identiy matrix of size SizeArg
				// CALL FUNCTION HERE
				row_index++;
			}
		}		

		template<class FloatTypeArg, size_t SizeArg>
		template<class ColumnVecIterator>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(							
			size_t InSize,
			ColumnVecIterator firstColumnVecIt, ColumnVecIterator lastColumnVecIt,			
			EMatrixColumnMajor InSrcMatrixColumnMajor
		)
		{
			assert(InSize <= SizeArg);

			assert(!"Not yet impl");
			// IMPLEMENTATION:
			// See the SquareMatrix from row range implementation
		}

		//
		// Construction of square matrix of the size specified as the templ argument 
		// from the given submatrix of the given array of an arbitrary size.
		// If the destintation matrix has a greater size than the specified size argument, 
		// than the extra components are equal to the corresponding components of 
		// the identity matrix.
		template<class FloatTypeArg, size_t SizeArg>
		template<class SrcFloatTypeArg, size_t InSrcVecArraySize, size_t InSrcVecSize>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(				
			size_t InSize,
			const std::array<Vec<SrcFloatTypeArg, InSrcVecSize>, InSrcVecArraySize>& InArray,											
			EMatrixRowMajor InSrcMatrixRowMajor
		)
		{
			assert(!"Not yet impl");
			// IMPLEMENTATION
			// Implement the construction from vector range functions before
			// implementing this function
		}

		template<class FloatTypeArg, size_t SizeArg>
		template<class SrcFloatTypeArg, size_t SrcSize>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			size_t InSize,
			const SquareMatrix<SrcFloatTypeArg, SrcSize>& InSrcMatrix,				
			EMatrixRowMajor InSrcMatrixRowMajor
		)
		{
			ResetFromMatrix
			(
				InSize,
				this,
				InSrcMatrix	
			);	
		}

		//
		// Create the square matrix of the size specified as the templ argument.
		// The constructed matrix will have a submatrix of the specified size copied
		// from the given src matrix and the rest elements will be equal
		// to the corresponding elements of the identity matrix.
		// The src matrix is treated as the row-major.
		//
		template<class FloatTypeArg, size_t SizeArg>
		template<class SrcFloatTypeArg, size_t SrcSize>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			size_t InSize,
			const SquareMatrix<SrcFloatTypeArg, SrcSize>& InSrcMatrix,				
			EMatrixColumnMajor InSrcMatrixColumnMajor
		)
		{
			ResetFromTransposedMatrix
			(
				InSize,
				this,
				InSrcMatrix	
			);	
		}

		template<class FloatTypeArg, size_t SizeArg>
		template<class SrcFloatTypeArg>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix
		(
			const SrcFloatTypeArg			*pSrc, 			
			size_t							 rowByteStride, 
			size_t							 componentByteStride,
			size_t							 InNumSrcRows,
			size_t							 InNumSrcColumns
		)
		{		
			ResetMatrixFromMemory
			(
				this,				
				rowByteStride,
				componentByteStride,
				InNumSrcRows,
				InNumSrcColumns,
				0, 0
			);				
		}

		template<class FloatTypeArg, size_t SizeArg>
		SquareMatrix<FloatTypeArg, SizeArg>::SquareMatrix (const ThisType& InSrc)
		{
			*this = InSrc;
		}

		template<class FloatTypeArg, size_t SizeArg>
		typename SquareMatrix<FloatTypeArg, SizeArg>::ThisType& SquareMatrix<FloatTypeArg, SizeArg>::operator=(const ThisType& InSrc)
		{
			ResetFromMatrix
			(
				GetSize(),
				this,
				InSrc
			);	
			return *this;
		}
	} // Math
} // DvUtils