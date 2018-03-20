#ifndef _MathUtil_Core_LinearAlgebraUtils_h_
#define _MathUtil_Core_LinearAlgebraUtils_h_

#include <DirectXMath.h>
#include <string>
#include "CoreUtil\CoreUtil_Ptr.h"
#include "MathUtil_Core_Util.h"
#include "../3DMath/MathUtil_3DMath_Utils.h"
#include "BLAS/MathUtil_Core_BLAS.h"
using namespace Dv;

/************************************************************
* REFACTOR TODO:
* 1. Move vectors operations to BLAS/MathUtil_Core_BLAS_Vector.h
* 2. Move quaternion operations to BLAS/MathUtil_Core_BLAS_Quaternion.h
* 3. Move matrix operations to BLAS/MathUtil_Core_BLAS_Matrix.h
* 4. Move common operation to BLAS/MathUtil_Core_BLAS.h
************************************************************/

/************************************************************
// QUICK TODO:
// 1. MatrixToString, MatrixToDebugString (DONE: 16.08.2016)
//
// QUICK TODO LOG 1:
// 1. Implement matrix operations
// 1.1 mult
// 1.2 transpose
// 1.2.1. MatrixTranspose				(DONE: 03.08.2016)
// 1.2.2. MatrixTransposeCopy			(DONE: 03.08.2016)
// 1.3 inverse
// 2. Setting matrix
// 2.1. SetMatrix2x2, SetMatrix3x3, SetMatrix4x4 (DONE: 04.08.2016)
// 2.2. SetRow2, SetRow3, SetRow4 +
// 2.2.1. Find or implement operations for setting all components of the vector +
// 2.3. SetTransposedMatrix2x2, SetTransposedMatrix3x3, SetTransposedMatrix4x4 +
// 2.4. ResetMatrix2x2, ResetMatrix3x3, ResetMatrix4x4 +
// 2.5. Reset from columns:
//		2.5.1. ResetMatrix3x3_FromColumns +
//			2.5.1.1. Implement functions for resetting column of the matrix
//				2.5.1.1.1. Implement function for setting the part of the matrix row
//						   to the elements corresponding the elements of the identity matrix.
//				2.5.1.1.1.1. Implement or found function SetVectorComponent +
// 2.6. Implement:
//			ResetMatrixFromMatrix_n +
// 2.7. Reset from rows:
// 2.7.0. functions to implement:
// 2.7.0.1. Reset 2x2 submatrix from rows +
// 2.7.0.2. Reset 3x3 submatrix from rows +
// 2.7.0.3. Reset 4x4 submatrix from rows +
// 2.7.1. Implement functions
// 2.7.1.1. ResetMatrixRow +
************************************************************/

/************************************************************
* Matrix operations:
* 1. Using matrix operation functions:
* 1.1. When matrix is declared as the array [m][n],
*      always pass the pointer as &(arr[0][0])
*      (the reason is to avoid the wrong template argument deduction)
************************************************************/

/************************************************************
// 2. Implement vector operations
// 2.1. type casting
//		2.1. n +
// 2.2. floating to to int vector type casting
// 2.3. to string and to debug string +
// 2.4. cross product +
// 2.5. normalize
// 2.6. vector-by-scalar division
// 2.7. generalized operations:
// 2.7.1. add_n_to
// 2.8.2. mult +
// 2.9.3. Vec_n_length+
// 2.9.3. SetAdd_n +
// 2.9.4. Subtract_n_from +
// 2.9.5. Dot_n +
// 2.9.6. Vec_n_Lerp
// 2.9.7. SetSubtract_n +
// 3. zero vectors
// 3.1. squared distance between two points in n-dimensional space: SqrDist_n +
// 3.2. negate_n +
// 3.3. absolutize_n +
// 4. add signatures with template argument <numComponents>
// 5. MinIndex, MaximIndex +
// 6. linear combination calculation
// 7. 3d Math functions
// 7.1. spherical space calculation
// 7.1.1. 2d radius vector calculation +
// 7.1.2. 3d radius vector calculation +
// 8. Coordinate transformations
// 8.1. XYZ -> XZY +
// 8.2.
*************************************************************/

/*************************************************************
* 9. Quaternions
*		9.0. Quaternion representation
*			9.0.1. w - scalar component
*			9.0.2. vec(x,y,z) - vector part
*		9.1. Set quaternion:
*			9.1.1. from axis and angle
*				9.1.1.1 Arguments
*					9.1.1.1.1. axis 
*					9.1.1.1.2. angle (in radians)
*				9.1.1.2 Algo:
*					9.1.1.2.1. res_w = cos(angle/2.0)
*					8.1.1.2.2. res_vec = sin(angle/2.0) * vec
*			9.1.2. Set as identity
*				9.1.2.1. No Arguments
*				9.1.2.2. Algo:
*					9.1.2.2.1. res_w = 1.0
*					9.1.2.2.2. res_vec = (0.0, 0.0, 0.0)
*		9.2. Comparison
*			9.2.1. Is near identity quaternion
*				9.2.1.0. Description:
*					9.2.1.0.1. Checks whether quaternion is identity with w==1 or w==-1
*							   (because two quaternions always exist)
*				9.2.1.1. Arguments:
*					9.2.2.0.1. Eps
*				9.2.1.2. Algo:
*					9.2.1.2.1. return AreQuaternionsNearEqual(pQuat, 1.0, 0.0, 0.0, 0.0)
*			9.2.2. Are quaternions near equal
*				9.2.2.0. Description:
*					9.2.2.0.1. Checks whether two quaternions are near equal using given Eps
*				9.2.2.1. Arguments:
*					9.2.2.0.0. Eps
*					9.2.2.1.1. w2 
*					9.2.2.1.2. vec2
*		9.3. Magnitude
*		9.4. Negate
*		9.5. Conjugate and inverse
*			9.5.1. Conjugate:
*				9.5.1.0. Description
*					9.5.1.0.1. Negates the axis vector of rotation of the given quaternion
*			9.5.2. Inverse:
*				9.5.2.0. Description
*					9.5.2.0.1. Conjugate divided by magnitude
*					9.5.2.0.2. For identity quaternions inverse is equal to the conjugate
*			9.5.3. Angle negate
*				9.5.3.1. Description
*					9.5.3.0.1. Negates only the angle
*					9.5.3.0.2. Geometrically the resulting angle is equal to the conjugated
*		9.6. Quaternion multiplication
*			9.6.0. Description
*				9.6.0.1. The quaternion multiplication is useful for concatenating two rotations
*				9.6.0.2. The quaternion multiplication is NOT very useful to
*						 rotate the point by the rotation defined by a quaternions,
*						 because it's AS FAST to convert the quaternion to the matrix
*						 and then use the matrix to transform the point.
*			9.6.1. Standart multiplication
*				9.6.1.0. Description
*					9.6.1.0.0. Conforms to the official quaternion definition 
*					9.6.1.0.1. Concation of quaternions occurs in opposite order:
*							   when we want to rotate point p by quaternion a, then by quaternion b:
*							   res_quat = qpq^-1
*							   res_quat=b(apa^-1)b^-1
*							   res_quat=(ba)p(ba)^-1
*					9.6.1.0.1. formula: 
*						9.6.1.0.1.a. res_w = w1*w2-dot(vec1,vec2); 
*						9.6.1.0.1.b. res_vec = w1*v2+w2*v1+cross(v2,v1);
*			9.7.1. "Forward" multiplication
*				9.7.1.0. Description
*					9.7.1.0.0. Doest NOT conform to the official quaternion definition 
*					9.7.1.0.1. formula (only the v1, v2 order in the cross is affected!): 
*						9.7.1.0.1.a. res_w = w1*w2-dot(vec1,vec2); 
*						9.7.1.0.1.b. res_vec = w1*v2+w2*v1+cross(v1,v2);
*		9.7. Quaternion "difference"
*			9.7.1. Result quaternion d represents the angular displacement
*			between the given quaternions a and b.
*		9.8. Quaternion dot product
*			8.9.0. For unit quaternion result is always within range [-1;1],
*				   but we mostly interested in the absolute value
*			8.9.1. Dot product can be used as a measure of how similar the two rotations are.
*		9.9. Quaternion log
*			9.9.0. log of quaternion			q	  = [(v_x sin a, v_y sin a, v_z sin a), cos a]
*				   returnes the following:		log q = [a * vec_x, a * vec_y, a * vec_z, 0]
*				   As you see, there result quaternion is not necessarily a unit quaternion
*		9.10. Quaternion exponent
**************************************************************************/

/************************************************************************
* Euler angles
* 1. Systems:
*		1.1. Heading-pitch-bank
*			1.1.0. Named in the order of transformation from interial to object space
*			1.1.1. Heading is clockwise rotation around the object space Y (restricted to range +-180)
*			1.1.2. Pitch is "downward" rotation around object space X (restricted to range +-90)
*			1.1.3. Roll is counterclockwise in the object Z axis (restricted in ragne +-180)
************************************************************************/

/************************************************************************
* 1. Converting quaternions to matrices:
* 2. Converting quaternions to euler angles
************************************************************************/

/************************************************************************
* Euler angles representation transformations
* 1. Converting a matrix to euler angles (needed to convert the quaternion to euler)
*		1.1. Intertial (translated-world) space to object rotation matrix to euler +
* 2. Converting euler angles to matrix:
*		2.1. Intertial (translated-world) space to object rotation matrix +
*		2.2. Object space to inertial(translated-world) rotation matrix
************************************************************************/

namespace DvUtils
{
	namespace Math
	{	
		/*********************************************************************************************
		* Euler angles representation transformations
		*********************************************************************************************/
		// See help an the top
		template<class FloatTypeArg>
		FloatTypeArg *Euler_HeadingPitchBank_To_Matrix33_InertialToObjectRotation
		(
			FloatTypeArg			*pMatrix33,
			const FloatTypeArg		*pHeading,
			const FloatTypeArg		*pPitch,
			const FloatTypeArg		*pRoll,
			size_t					 destColumnStrideInBytes			= sizeof(FloatTypeArg),
			size_t					 destRowStrideInBytes				= 3 * sizeof(FloatTypeArg)
		);		

		template<class FloatTypeArg>
		FloatTypeArg *Euler_HeadingPitchBank_To_Matrix33_InertialToObjectRotation
		(
			FloatTypeArg			*pMatrixRowOne,
			FloatTypeArg			*pMatrixRowTwo,
			FloatTypeArg			*pMatrixRowThree,
			const FloatTypeArg		*pHeading,
			const FloatTypeArg		*pPitch,
			const FloatTypeArg		*pBank,
			size_t					 destColumnStrideInBytes			= sizeof(FloatTypeArg)			
		);

		// ----------------------------------------------------------------------------------------------
		// Matrix33_InertialToObjectRotation_To_Euler_HeadingPitchBank
		// This function reverse engineers matrices generated
		// by the Euler_HeadingPitchBank_To_Matrix33_InertialToObjectRotation functions.
		//  
		// Considers:
		//		- Tries to consider the malformity of the input matrix
		//		(e.g. some component may be out of [-1.0;1.0] range).
		//		- When the gimbal lock detected, the result is the matrix with bank equal to zero.
		// ----------------------------------------------------------------------------------------------
		template<class FloatTypeArg>
		FloatTypeArg *Matrix33_InertialToObjectRotation_To_Euler_HeadingPitchBank
		(
			FloatTypeArg				*pHeading,
			FloatTypeArg				*pPitch,
			FloatTypeArg				*pBank,
			const FloatTypeArg			*pMatrix33,
			size_t						 destColumnStrideInBytes			= sizeof(FloatTypeArg),
			size_t						 destRowStrideInBytes				= 3 * sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *Matrix33_InertialToObjectRotation_To_Euler_HeadingPitchBank
		(
			FloatTypeArg				*pHeading,
			FloatTypeArg				*pPitch,
			FloatTypeArg				*pBank,
			const FloatTypeArg			*pMatrixRowOne,
			const FloatTypeArg			*pMatrixRowTwo,
			const FloatTypeArg			*pMatrixRowThree,
			size_t						 destColumnStrideInBytes			= sizeof(FloatTypeArg)			
		);

		/*********************************************************************************************
		* Quaternions
		*********************************************************************************************/
		template<class FloatTypeArg>
		FloatTypeArg *SetQuaternion_FromAxisAngle
		(
			FloatTypeArg			*pDestQuat,
			const FloatTypeArg		*pAxis,
			const FloatTypeArg		*pAngle,
			size_t					 DestStrideInBytes = sizeof(FloatTypeArg),
			size_t					 AxisStridInBytes = sizeof(FloatTypeArg)			
		);

		template<class FloatTypeArg>
		FloatTypeArg *SetQuaternion_FromAxisAngle
		(
			FloatTypeArg			*pDestQuat,
			const FloatTypeArg		*pAxisX,
			const FloatTypeArg		*pAxisY,
			const FloatTypeArg		*pAxisZ,
			const FloatTypeArg		*pAngle,
			size_t					 DestStrideInBytes = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *SetQuaternion_AsIdentity
		(
			FloatTypeArg			*pDestQuat,
			size_t					 DestStrideInBytes = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		bool Quaternion_IsNearIdentity
		(
			const FloatTypeArg			*pQuat,
			FloatTypeArg				 Eps,
			size_t						 quatStrideInBytes		= sizeof(FloatTypeArg)
		);	

		template<class FloatTypeArg>
		bool Quaternion_AreNearEqual
		(
			const FloatTypeArg			*pQuat,
			const FloatTypeArg			*pQuatTwo,
			FloatTypeArg				 Eps,
			size_t						 quatStrideInBytes		= sizeof(FloatTypeArg),
			size_t						 quatTwoStrideInBytes	= sizeof(FloatTypeArg)
		);		

		template<class FloatTypeArg>
		bool Quaternion_AreNearEqual
		(
			const FloatTypeArg			*pQuat,			
			const FloatTypeArg			*pQuatTwo_W,
			const FloatTypeArg			*pQuatTwo_X,
			const FloatTypeArg			*pQuatTwo_Y,
			const FloatTypeArg			*pQuatTwo_Z,
			FloatTypeArg				 Eps,
			size_t						 quatStrideInBytes		= sizeof(FloatTypeArg)			
		);		

		template<class FloatTypeArg>
		bool Quaternion_AreNearEqual
		(
			const FloatTypeArg			*pQuat_W,
			const FloatTypeArg			*pQuat_X,
			const FloatTypeArg			*pQuat_Y,
			const FloatTypeArg			*pQuat_Z,
			const FloatTypeArg			*pQuatTwo_W,
			const FloatTypeArg			*pQuatTwo_X,
			const FloatTypeArg			*pQuatTwo_Y,
			const FloatTypeArg			*pQuatTwo_Z,
			FloatTypeArg				 Eps
		);

		template<class FloatTypeArg>
		bool Quaternion_AreStrictEqual
		(
			const FloatTypeArg			*pQuat,
			const FloatTypeArg			*pQuatTwo,			
			size_t						 quatStrideInBytes		= sizeof(FloatTypeArg),
			size_t						 quatTwoStrideInBytes	= sizeof(FloatTypeArg)
		);	

		template<class FloatTypeArg>
		bool Quaternion_AreStrictEqual
		(
			const FloatTypeArg			*pQuat,			
			const FloatTypeArg			*pQuatTwo_W,
			const FloatTypeArg			*pQuatTwo_X,
			const FloatTypeArg			*pQuatTwo_Y,
			const FloatTypeArg			*pQuatTwo_Z,			
			size_t						 quatStrideInBytes		= sizeof(FloatTypeArg)			
		);	

		template<class FloatTypeArg>
		bool Quaternion_AreStrictEqual
		(
			const FloatTypeArg			*pQuat_W,
			const FloatTypeArg			*pQuat_X,
			const FloatTypeArg			*pQuat_Y,
			const FloatTypeArg			*pQuat_Z,
			const FloatTypeArg			*pQuatTwo_W,
			const FloatTypeArg			*pQuatTwo_X,
			const FloatTypeArg			*pQuatTwo_Y,
			const FloatTypeArg			*pQuatTwo_Z			
		);

		template<class FloatTypeArg>
		FloatTypeArg Quaternion_CalcMagnitude
		(
			const FloatTypeArg			*pQuat,
			size_t						 DestStrideInBytes = sizeof(FloatTypeArg)
		);
		
		template<class FloatTypeArg>
		FloatTypeArg Quaternion_CalcSquaredMagnitude
		(
			const FloatTypeArg			*pQuat,
			size_t						 DestStrideInBytes
		);

		template<class FloatTypeArg>
		FloatTypeArg *Quaterion_Conjugate
		(
			FloatTypeArg				*pQuat,
			size_t						 DestStrideInBytes = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *Quaternion_NegateAngle
		(
			FloatTypeArg				*pQuat,
			size_t						 DestStrideInBytes = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *Quaternion_Inverse
		(
			FloatTypeArg				*pQuat,
			size_t						DestStrideInBytes = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *Quaternion_Mult
		(
			FloatTypeArg				*pDestQuat,
			const FloatTypeArg			*pQuat,
			const FloatTypeArg			*pQuatTwo,
			size_t						 DestStrideInBytes = sizeof(FloatTypeArg),
			size_t						 quatStrideInBytes = sizeof(FloatTypeArg),
			size_t						 quatTwoStrideInBytes = sizeof(FloatTypeArg)
		);

		// See help at the top of this header file
		template<class FloatTypeArg>
		FloatTypeArg *Quaternion_Mult_Forward
		(
			FloatTypeArg				*pDestQuat,
			const FloatTypeArg			*pQuat,
			const FloatTypeArg			*pQuatTwo,
			size_t						 DestStrideInBytes = sizeof(FloatTypeArg),
			size_t						 quatStrideInBytes = sizeof(FloatTypeArg),
			size_t						 quatTwoStrideInBytes = sizeof(FloatTypeArg)
		);

		// See help at the top of this header file
		template<class FloatTypeArg>
		FloatTypeArg *Quaterntion_Difference
		(
			FloatTypeArg						*pDestQuat,
			const FloatTypeArg					*pEndQuat,
			const FloatTypeArg					*pStartQuat,
			size_t								 DestStrideInBytes				= sizeof(FloatTypeArg),
			size_t								 EndQuatStrideInBytes			= sizeof(FloatTypeArg),
			size_t								 StartQuatTwoStrideInBytes		= sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg Quaternion_Dot
		(
			const FloatTypeArg			*pQuat,
			const FloatTypeArg			*pQuatTwo,			
			size_t						 quatStrideInBytes = sizeof(FloatTypeArg),
			size_t						 quatTwoStrideInBytes = sizeof(FloatTypeArg)
		);

		// Computes the absolute value of the quaternion dot product
		template<class FloatTypeArg>
		FloatTypeArg Quaternion_Abs_Dot
		(
			const FloatTypeArg			*pQuat,
			const FloatTypeArg			*pQuatTwo,			
			size_t						 quatStrideInBytes = sizeof(FloatTypeArg),
			size_t						 quatTwoStrideInBytes = sizeof(FloatTypeArg)
		);

		// See help at the top of this header file
		template<class FloatTypeArg>
		FloatTypeArg *Quaternion_Exp
		(
			FloatTypeArg						*pDestQuat,
			size_t								 DestStrideInBytes				= sizeof(FloatTypeArg)
		);

		/*********************************************************************************************
		* Converting quaternions to other representations
		*********************************************************************************************/
		// Converts the given quaternion to a 3x3 rotation matrix
		// Matrix is specified with the stride between rows and stride between columns
		template<class FloatTypeArg>
		void Quaternion_To_Matrix33
		(
			FloatTypeArg			*pDestMatrix,
			const FloatTypeArg		*pQuat,
			size_t					 destColumnStrideInBytes			= sizeof(FloatTypeArg),
			size_t					 destRowStrideInBytes				= 3 * sizeof(FloatTypeArg),
			size_t					 quatStrideInBytes					= sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		void Quaternion_To_Matrix33
		(
			FloatTypeArg			*pDestMatrix_RowOne,
			FloatTypeArg			*pDestMatrix_RowTwo,
			FloatTypeArg			*pDestMatrix_RowThree,
			const FloatTypeArg		*pQuad,
			size_t					 destColumnStrideInBytes			= sizeof(FloatTypeArg),			
			size_t					 quatStrideInBytes					= sizeof(FloatTypeArg)
		);
	
		template<class FloatTypeArg>
		void Quatenion_IntertialToObject_To_Euler
		(
			FloatTypeArg			*pHeading,
			FloatTypeArg			*pPitch,
			FloatTypeArg			*pBank,			
			const FloatTypeArg		*pQuat,			
			size_t					 quatStrideInBytes					= sizeof(FloatTypeArg)
		);

		// -----------------------------------------------------------------------------------------------------
		// Matrix utils
		// -----------------------------------------------------------------------------------------------------		
		template<class SrcFloatTypeArg>
		std::string MatrixToDebugString
		(
			const SrcFloatTypeArg			*InMatrix,
			int								 InSize,
			size_t							 InRowStride,
			size_t							 InComponentStide
		);	

		template<class SrcFloatTypeArg>
		std::string MatrixToString
		(
			const SrcFloatTypeArg			*InMatrix,
			int								 InSize,
			size_t							 InRowStride,
			size_t							 InComponentStide
		);	

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix2x2_FromColumns
		(
			DestFloatTypeArg		   *pDest,
			size_t						InMatrixSize,
			const SrcFloatTypeArg	   *pColumn0,
			const SrcFloatTypeArg	   *pColumn1,			
			size_t						InDestRowStride,
			size_t						InDestComponentStride,
			size_t						InColumnOneComponentStride,
			size_t						InColumnTwoComponentStride		
		);	

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix3x3_FromColumns
		(
			DestFloatTypeArg		   *pDest,
			size_t						InMatrixSize,
			const SrcFloatTypeArg	   *pColumn0,
			const SrcFloatTypeArg	   *pColumn1,
			const SrcFloatTypeArg	   *pColumn2,
			size_t						InDestRowStride,
			size_t						InDestComponentStride,
			size_t						InColumnOneComponentStride,
			size_t						InColumnTwoComponentStride,
			size_t						InColumnThreeComponentStride
		);		

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix4x4_FromColumns
		(
			DestFloatTypeArg		   *pDest,
			size_t						InMatrixSize,
			const SrcFloatTypeArg	   *pColumn0,
			const SrcFloatTypeArg	   *pColumn1,
			const SrcFloatTypeArg	   *pColumn2,
			const SrcFloatTypeArg	   *pColumn3,
			size_t						InDestRowStride,
			size_t						InDestComponentStride,
			size_t						InColumnOneComponentStride,
			size_t						InColumnTwoComponentStride,
			size_t						InColumnThreeComponentStride,
			size_t						InColumnFourComponentStride
		);	

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix2x2_FromRows
		(
			DestFloatTypeArg		   *pDest,
			size_t						InMatrixSize,
			const SrcFloatTypeArg	   *pRow0,
			const SrcFloatTypeArg	   *pRow1,			
			size_t						InDestRowStride,
			size_t						InDestComponentStride,
			size_t						InRowOneComponentStride,
			size_t						InRowTwoComponentStride		
		);	

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix3x3_FromRows
		(
			DestFloatTypeArg		   *pDest,
			size_t						InMatrixSize,
			const SrcFloatTypeArg	   *pRow0,
			const SrcFloatTypeArg	   *pRow1,
			const SrcFloatTypeArg	   *pRow2,
			size_t						InDestRowStride,
			size_t						InDestComponentStride,
			size_t						InRowOneComponentStride,
			size_t						InRowTwoComponentStride,
			size_t						InRowThreeComponentStride
		);		

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix4x4_FromRows
		(
			DestFloatTypeArg		   *pDest,
			size_t						InMatrixSize,
			const SrcFloatTypeArg	   *pRow0,
			const SrcFloatTypeArg	   *pRow1,
			const SrcFloatTypeArg	   *pRow2,
			const SrcFloatTypeArg	   *pRow3,
			size_t						InDestRowStride,
			size_t						InDestComponentStride,
			size_t						InRowOneComponentStride,
			size_t						InRowTwoComponentStride,
			size_t						InRowThreeComponentStride,
			size_t						InRowFourComponentStride
		);	

		// Reset matrix:
		//		the given submatrix will be filled with the given values,
		// and the rest components will be filled with the corresponding 
		// components of the square matrix.
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix2x2
		(
			DestFloatTypeArg			*pDest,
			size_t						InMatrixSize,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix3x3
		(
			DestFloatTypeArg			*pDest,
			size_t						InMatrixSize,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01, SrcFloatTypeArg In02,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11, SrcFloatTypeArg In12,
			SrcFloatTypeArg	In20, SrcFloatTypeArg In21, SrcFloatTypeArg In22,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix4x4
		(
			DestFloatTypeArg			*pDest,
			size_t						InMatrixSize,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01, SrcFloatTypeArg In02, SrcFloatTypeArg In03,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11, SrcFloatTypeArg In12, SrcFloatTypeArg In13,
			SrcFloatTypeArg	In20, SrcFloatTypeArg In21, SrcFloatTypeArg In22, SrcFloatTypeArg In23,
			SrcFloatTypeArg	In30, SrcFloatTypeArg In31, SrcFloatTypeArg In32, SrcFloatTypeArg In33,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetMatrix2x2
		(
			DestFloatTypeArg			*pDest,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes				= sizeof(FloatTypeArg)
		);		

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetMatrix3x3
		(
			DestFloatTypeArg			*pDest,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01, SrcFloatTypeArg In02,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11, SrcFloatTypeArg In12,
			SrcFloatTypeArg	In20, SrcFloatTypeArg In21, SrcFloatTypeArg In22,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes				= sizeof(FloatTypeArg)
		);		

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetMatrix4x4
		(
			DestFloatTypeArg			*pDest,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01, SrcFloatTypeArg In02, SrcFloatTypeArg In03,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11, SrcFloatTypeArg In12, SrcFloatTypeArg In13,
			SrcFloatTypeArg	In20, SrcFloatTypeArg In21, SrcFloatTypeArg In22, SrcFloatTypeArg In23,
			SrcFloatTypeArg	In30, SrcFloatTypeArg In31, SrcFloatTypeArg In32, SrcFloatTypeArg In33,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes				= sizeof(FloatTypeArg)
		);		

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetTransposedMatrix2x2
		(
			DestFloatTypeArg			*pDest,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes				= sizeof(FloatTypeArg)
		);		

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetTransposedMatrix3x3
		(
			DestFloatTypeArg			*pDest,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01, SrcFloatTypeArg In02,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11, SrcFloatTypeArg In12,
			SrcFloatTypeArg	In20, SrcFloatTypeArg In21, SrcFloatTypeArg In22,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes				= sizeof(FloatTypeArg)
		);		

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetTransposedMatrix4x4
		(
			DestFloatTypeArg			*pDest,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01, SrcFloatTypeArg In02, SrcFloatTypeArg In03,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11, SrcFloatTypeArg In12, SrcFloatTypeArg In13,
			SrcFloatTypeArg	In20, SrcFloatTypeArg In21, SrcFloatTypeArg In22, SrcFloatTypeArg In23,
			SrcFloatTypeArg	In30, SrcFloatTypeArg In31, SrcFloatTypeArg In32, SrcFloatTypeArg In33,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes				= sizeof(FloatTypeArg)
		);	

		// Set the given memory to the identity matrix
		template<class FloatTypeArg>			
		void SetIdentityMatrix
		(
			FloatTypeArg			*pDest, 
			int						 InNumRows,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes				= sizeof(FloatTypeArg)
		);		

		template<class FloatTypeArg>
		void ZeroMatrix
		(
			FloatTypeArg			*pDest, 			
			int						 InNumRows,
			int						 InNumColumns,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes				= sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *FillMatrix
		(
			FloatTypeArg		*pDestMatrix,
			size_t				InSize,
			FloatTypeArg		InValue,
			size_t				rowStrideInBytes,
			size_t				strideInBytes				= sizeof(FloatTypeArg)
		);		

		// Set the given element of the matrix
		template<class FloatTypeArg>		
		FloatTypeArg SetMatrixElement
		(
			FloatTypeArg			*pDest, 			
			int						 InColumnIndex,
			const FloatTypeArg&		 InValue,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes			= sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>			
		void SetMainDiagElements
		(
			FloatTypeArg			*pDest, 
			int						 InNumRows,
			const FloatTypeArg&		 InValue,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes			= sizeof(FloatTypeArg)
		);		

		template<class FloatTypeArg> 
		FloatTypeArg *ZeroRow
		(	
			FloatTypeArg			*pDest, 		
			int						 InNumColumns,
			int						 InRowIndex,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetRow2
		(
			DestFloatTypeArg		*pDestMatrix,
			int						 InRowIndex,
			SrcFloatTypeArg			 InComponentFirst, 
			SrcFloatTypeArg			 InComponentSecond,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetRow3
		(
			DestFloatTypeArg		*pDestMatrix,
			int						 InRowIndex,
			SrcFloatTypeArg			 InComponentFirst, 
			SrcFloatTypeArg			 InComponentSecond,
			SrcFloatTypeArg			 InComponentThird,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetRow4
		(
			DestFloatTypeArg		*pDestMatrix,
			int						 InRowIndex,
			SrcFloatTypeArg			 InComponentFirst, 
			SrcFloatTypeArg			 InComponentSecond,
			SrcFloatTypeArg			 InComponentThird,
			SrcFloatTypeArg			 InComponentFourth,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes
		);

		// Resets the given column of the matrix.
		// So that the part of the destination matrix column of the given size
		// is copied from the given src column, and the rest of
		// the destination column elemenents are filled with the corresponding
		// elements of the identity matrix equal to the size of the destination matrix.		
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrixColumn
		(
				DestFloatTypeArg		*pDestMatrix,
				size_t					 InMatrixSize, // size of the matrix
				int						 InNumElemsToCopy, // number of elements to copy
				int						 InDestColumnIndex, // destination column index
				const SrcFloatTypeArg	*pSrcColumn,
				size_t					 InDestRowStride,
				size_t					 InDestComponentStride,
				size_t					 InSrcComponentStride				
		);

		// Resets the given row of the matrix.
		// So that the part of the destination matrix row of the given size
		// is copied from the given src row, and the rest of
		// the destination column elemenents are filled with the corresponding
		// elements of the identity matrix equal to the size of the destination matrix.	
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrixRow
		(
				DestFloatTypeArg		*pDestMatrix,
				size_t					 InMatrixSize,			// size of the matrix
				int						 InNumElemsToCopy,		// number of elements to copy
				int						 InDestRowIndex,		// destination column index
				const SrcFloatTypeArg	*pSrcRow,
				size_t					 InDestRowStride,
				size_t					 InDestComponentStride,
				size_t					 InSrcComponentStride				
		);

		// Matrix addition
		template
		<
			class DestFloatTypeArg, 
			class SrcFloatTypeArg
		>
		DestFloatTypeArg *AddMatrix_To
		(
			DestFloatTypeArg *pDestMatrix, 
			const SrcFloatTypeArg *pSrcMatrix, 
			int InNumRows, int InNumColumns,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep		= sizeof(DestFloatTypeArg), 			
			uint8_t srcComponentByteStep		= sizeof(SrcFloatTypeArg)
		);

		template
		<
			class DestFloatTypeArg, 
			class SrcFloatTypeArg
		>
		DestFloatTypeArg *SubtractMatrix_From
		(
			DestFloatTypeArg *pDestMatrix, 
			const SrcFloatTypeArg *pSrcMatrix, 
			int InNumRows, int InNumColumns,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep		= sizeof(DestFloatTypeArg), 			
			uint8_t srcComponentByteStep		= sizeof(SrcFloatTypeArg)
		);

		template
		<
			class DestFloatTypeArg, 
			class SrcFloatTypeArg
		>
		DestFloatTypeArg *AddMatrixRow_To
		(
			DestFloatTypeArg *pDestMatrix, const SrcFloatTypeArg *pSrcMatrix, 
			int row_index, 
			int InNumColumns, 
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep, 			
			uint8_t srcComponentByteStep
		);		

		template
		<
			class DestFloatTypeArg, 
			class SrcFloatTypeArg
		>
		DestFloatTypeArg *RemoveMatrixRow_From
		(
			DestFloatTypeArg *pDestMatrix, const SrcFloatTypeArg *pSrcMatrix, 
			int row_index, 
			int InNumColumns, 
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep, 			
			uint8_t srcComponentByteStep
		);		

		// Mult two matrices and save result to the given memory location
		// WARNING: The result memory region may not overlap
		// with the region of the src matrices
		template
		<
			class DestFloatTypeArg, 
			class SrcFloatTypeArg,
			class SrcTwoFloatTypeArg
		>
		void MatrixCopyMult
		(
			DestFloatTypeArg									*pDestMatrix, 
			const						 SrcFloatTypeArg		*pSrcMatrix,
			const						 SrcTwoFloatTypeArg		*pSrcTwoMatrix,
			int InDestNumRows, int InDestNumColumns,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t srcTwoComponentRowByteStep, 	
			uint8_t destComponentByteStep		= sizeof(DestFloatTypeArg), 			
			uint8_t srcComponentByteStep		= sizeof(SrcFloatTypeArg),
			uint8_t srcTwoComponentByteStep		= sizeof(SrcFloatTypeArg)		
		);

		// Matrix multiplication and division
		template<class FloatTypeArg>
		FloatTypeArg *Negate
		(
			FloatTypeArg		*pDestMatrix,			
			int					InNumComponents,
			size_t				InRowByteStride,		
			size_t				InComponentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg, class CoeffType>
		FloatTypeArg *MultScalar
		(
			FloatTypeArg		*pDestMatrix,
			CoeffType			InCoeff,
			int					InNumComponents,
			size_t				InRowByteStride,		
			size_t				InComponentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg, class CoeffType>
		FloatTypeArg *DivScalar
		(
			FloatTypeArg		*pDestMatrix,
			CoeffType			InCoeff,
			int					InNumComponents,
			size_t				InRowByteStride,		
			size_t				InComponentByteStride = sizeof(FloatTypeArg)
		);		

		template<class FloatTypeArg>	
		FloatTypeArg *Matrix22Mult(FloatTypeArg *pDestVec, const FloatTypeArg *pMatrix1, const FloatTypeArg *pMatrix2);

		template<class FloatTypeArg>	
		FloatTypeArg *Matrix33Mult(FloatTypeArg *pDestVec, const FloatTypeArg *pMatrix1, const FloatTypeArg *pMatrix2);

		template<class FloatTypeArg>
		FloatTypeArg *Matrix44Mult(FloatTypeArg *pDestVec, const FloatTypeArg *pMatrix1, const FloatTypeArg *pMatrix2);

		template<class FloatTypeArg>
		FloatTypeArg *Matrix22Inverse(FloatTypeArg *pDestVec, const FloatTypeArg *pMatrix);

		template<class FloatTypeArg>
		FloatTypeArg *Matrix33Inverse(FloatTypeArg *pDestVec, const FloatTypeArg *pMatrix);

		template<class FloatTypeArg>
		FloatTypeArg *Matrix44Inverse(FloatTypeArg *pDestVec, const FloatTypeArg *pMatrix);

		template<class DestFloatTypeArg>
		DestFloatTypeArg *MatrixTranspose
		(
			DestFloatTypeArg			*pMatrix,
			size_t						 InSize,
			size_t						 InRowByteStride,
			size_t						 InComponentByteStride = sizeof(FloatTypeArg)
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *MatrixTransposeCopy
		(
			DestFloatTypeArg			*pDestMatrix,
			const SrcFloatTypeArg		*pSrcMatrix,
			size_t						 InSize,
			size_t						 InDestRowByteStride,
			size_t						 InSrcRowByteStride,
			size_t						 InDestComponentByteStride = sizeof(DestFloatTypeArg),
			size_t						 InSrcComponentByteStride  = sizeof(SrcFloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *Matrix22Transpose(FloatTypeArg *pDestVec, const FloatTypeArg *pMatrix);

		template<class FloatTypeArg>
		FloatTypeArg *Matrix33Transpose(FloatTypeArg *pDestVec, const FloatTypeArg *pMatrix);

		template<class FloatTypeArg>
		FloatTypeArg *Matrix44Transpose(FloatTypeArg *pDestVec, const FloatTypeArg *pMatrix);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *MatrixCopyInverse_UsePrecomputedDet44
		(
			DestFloatTypeArg			*pDestMatrix,
			const SrcFloatTypeArg		*pSrcMatrix,
			SrcFloatTypeArg				 InPrecomputedDet,			
			size_t						 InDestRowByteStride,	
			size_t						 InSrcRowByteStride,
			size_t						 InDestComponentByteStride,
			size_t						 InSrcComponentByteStride
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *MatrixCopyInverse_UsePrecomputedDet33
		(
			DestFloatTypeArg			*pDestMatrix,
			const SrcFloatTypeArg		*pSrcMatrix,
			SrcFloatTypeArg				 InPrecomputedDet,
			size_t						 InDestRowByteStride,
			size_t						 InSrcRowByteStride,
			size_t						 InDestComponentByteStride,
			size_t						 InSrcComponentByteStride
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *MatrixCopyInverse_UsePrecomputedDet22
		(
			DestFloatTypeArg			*pDestMatrix,
			const SrcFloatTypeArg		*pSrcMatrix,
			SrcFloatTypeArg				 InPrecomputedDet,
			size_t						 InDestRowByteStride,
			size_t						 InSrcRowByteStride,
			size_t						 InDestComponentByteStride,
			size_t						 InSrcComponentByteStride
		);
		
		// Cofactor is a signed minor determinant
		template<class SrcFloatTypeArg>
		SrcFloatTypeArg ComputeSubmatrixCofactor44
		(
			const SrcFloatTypeArg *pSrcMatrix,
			size_t InRowIndex, size_t InColumnIndex,
			size_t InSrcRowByteStride,
			size_t InSrcComponentByteStride
		);

		template<class SrcFloatTypeArg>
		SrcFloatTypeArg ComputeSubmatrixCofactor33
		(
			const SrcFloatTypeArg *pSrcMatrix,
			size_t InRowIndex, size_t InColumnIndex,
			size_t InSrcRowByteStride,
			size_t InSrcComponentByteStride
		);

		// Minor 33 determinant is the 33 determinant of the given 44 matrix
		// where the i-th row and j-th column are crossed out
		// InRowIndex - index to cross out, 
		// InColumnIndex - index to cross out
		template<class SrcFloatTypeArg>
		SrcFloatTypeArg Minor_Det33
		(
			const SrcFloatTypeArg *pSrcMatrix, 
			size_t InRowIndex, size_t InColumnIndex,
			size_t InSrcRowByteStride,
			size_t InSrcComponentByteStride
		);

		// Minor 11 determinant is the 11 determinant of the given 44 matrix
		// where the i-th row and j-th column are crossed out
		// InRowIndex - index to cross out, 
		// InColumnIndex - index to cross out
		template<class SrcFloatTypeArg>
		SrcFloatTypeArg Minor_Det22
		(
			const SrcFloatTypeArg *pSrcMatrix,
			size_t InRowIndex, size_t InColumnIndex,
			size_t InSrcRowByteStride,
			size_t InSrcComponentByteStride
		);


		// Matrix copying
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCastMatrix
		(
			DestFloatTypeArg *pDestMatrix, const SrcFloatTypeArg *pSrcMatrix, 
			int InNumRows, int InNumColumns,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep		= sizeof(DestFloatTypeArg), 			
			uint8_t srcComponentByteStep		= sizeof(SrcFloatTypeArg)			
		);

		// Reset the destination matrix so that
		// the given submatrix is copied from the given src submatrix,
		// and the rest of the components are filled with
		// the corresponding components of the identity matrix
		// of the equal size.
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrixFromMatrix_n
		(
			DestFloatTypeArg					*pDestMatrix,
			const SrcFloatTypeArg				*pSrcMatrix,
			size_t								 InDestSize,
			size_t								 InSubmatrixSize,
			size_t								 InDestRowStride,
			size_t								 InSrcRowStride,
			size_t								 InDestComponentStride,
			size_t								 InSrcComponentStride
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCastRow
		(
			DestFloatTypeArg *pDestMatrix, const SrcFloatTypeArg *pSrcMatrix, 
			int InNumColumns,
			int InSrcRowIndex, int InDestRowIndex,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep		= sizeof(DestFloatTypeArg), 			
			uint8_t srcComponentByteStep		= sizeof(SrcFloatTypeArg)
		);

		// ---------------------------------------------------------------------------------------------
		// Description:
		//		Copies the given row of the src matrix to the column of the destination matrix
		// ---------------------------------------------------------------------------------------------
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCastRow_TransposedToDestColumn
		(
			DestFloatTypeArg *pDestMatrix, const SrcFloatTypeArg *pSrcMatrix, 
			int InNumSrcColumns,
			int InSrcRowIndex, int InDestColumnIndex,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep 		= sizeof(DestFloatTypeArg), 			
			uint8_t srcComponentByteStep		= sizeof(SrcFloatTypeArg)	
		);		

		// -----------------------------------------------------------------------------------------------
		//		Copies the given source matrix 
		// with transposing before copying to the dest value
		// Arguments:
		//		InNumSrcRows    - Number of rows in the src matrix (number of columns in the dest matrix)
		//		InNumSrcColumns - Number of columns in the src matrix (number of rows in the dest matrix)
		// ----------------------------------------------------------------------------------------------
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCastMatrix_Transposed
		(
			DestFloatTypeArg *pDestMatrix, const SrcFloatTypeArg *pSrcMatrix, 
			int InNumSrcRows, int InNumSrcColumns,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep		= sizeof(DestFloatTypeArg), 			
			uint8_t srcComponentByteStep		= sizeof(SrcFloatTypeArg)			
		);

		// --------------------------------------------------------------------------------------------
		// Spherical space calculation
		// -------------------------------------------------------------------------------------------		
		// x,y = horz axis
		// z = vertical axis
		template<class FloatTypeArg>
		FloatTypeArg *CalcRadiusVector3D
		(
			FloatTypeArg			*pOutRadiusVecto3D,
			FloatTypeArg			 angleInRadians_horz, 			
			FloatTypeArg			 angleInRadians_vertical, 			
			size_t					 DestStrideInBytes = sizeof(FloatTypeArg)
		);	

		template<class FloatTypeArg>
		void CalcRadiusVector3D
		(
			FloatTypeArg			*pOutX, 
			FloatTypeArg			*pOutY,
			FloatTypeArg			*pOutZ,
			FloatTypeArg			 angleInRadians_horz, 			
			FloatTypeArg			 angleInRadians_vertical			
		);	
		
		// x,y = plane
		template<class FloatTypeArg>
		FloatTypeArg *CalcRadiusVector2D
		(
			FloatTypeArg			*pOutRadiusVector2D,
			FloatTypeArg			 angleInRadians, 			
			size_t					 DestStrideInBytes = sizeof(FloatTypeArg)
		);		

		template<class FloatTypeArg>
		void CalcRadiusVector2D
		(
			FloatTypeArg			*pOutX,
			FloatTypeArg			*pOutY,
			FloatTypeArg			 angleInRadians			
		);		

		// ---------------------------------------------------------------------------------------------
		// Coordinate space transformations
		// ---------------------------------------------------------------------------------------------
		template<class FloatTypeArg>
		void Transform_XYZ_to_XZY
		(
			FloatTypeArg				*pOutX, 
			FloatTypeArg				*pOutY, 
			FloatTypeArg				*pOutZ
		);		

		template<class FloatTypeArg>
		FloatTypeArg *Transform_XYZ_to_XZY
		(
			FloatTypeArg				*pOut,
			size_t						 DestStrideInBytes = sizeof(FloatTypeArg)
		);

		// ---------------------------------------------------------------------------------------------
		// Vector utils
		// ---------------------------------------------------------------------------------------------	
		// Copy a vector from src to destination with type casting
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCast2
		(
			DestFloatTypeArg *pDestVec, const SrcFloatTypeArg *pSrcVec, 
			uint8_t destComponentByteStep = sizeof(DestFloatTypeArg), 
			uint8_t srcComponentByteStep = sizeof(SrcFloatTypeArg)
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCast3
		(
			DestFloatTypeArg *pDestVec, const SrcFloatTypeArg *pSrcVec, 
			uint8_t destComponentByteStep = sizeof(DestFloatTypeArg), 
			uint8_t srcComponentByteStep = sizeof(SrcFloatTypeArg)
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCast4
		(
			DestFloatTypeArg *pDestVec, const SrcFloatTypeArg *pSrcVec, 
			uint8_t destComponentByteStep = sizeof(DestFloatTypeArg), 
			uint8_t srcComponentByteStep = sizeof(SrcFloatTypeArg)
		);

		// Perform copying with a casting to the dest type
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCast_n
		(
			DestFloatTypeArg *pDestVec, const SrcFloatTypeArg *pSrcVec, 
			int numComponents,
			uint8_t destComponentByteStep = sizeof(DestFloatTypeArg), 
			uint8_t srcComponentByteStep = sizeof(SrcFloatTypeArg)
		);

		// Perform copying with a casting to the dest type
		// and zeroing the rest components
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCastZeroed_n
		(
			DestFloatTypeArg *pDestVec, const SrcFloatTypeArg *pSrcVec,
			int numDestComponents,
			int numSrcComponents,
			uint8_t destComponentByteStep = sizeof(DestFloatTypeArg), 
			uint8_t srcComponentByteStep = sizeof(SrcFloatTypeArg)
		);

		// Convert a given vector to a string
		template<class FloatTypeArg> 
		std::string VectorToDebugString
		(
			const FloatTypeArg *pVec, 
			int numComponents, 
			uint8_t componentByteStep = sizeof(FloatTypeArg),
			int precision_digs = 6
		);		

		template<class FloatTypeArg> 
		std::string VectorToString
		(
			const FloatTypeArg *pVec, 
			int numComponents, 
			uint8_t componentByteStep = sizeof(FloatTypeArg),
			int precision_digs = 6
		);		
		
		// Multiplies a 2d-vector by a 2x2 matrix rows
		template<class FloatTypeArg>
		FloatTypeArg *Vec2Matrix22RowDot(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec, const FloatTypeArg *pMatrix);

		template<class FloatTypeArg>
		FloatTypeArg *Vec2Matrix22RowDot
		(
			FloatTypeArg *pDestVec, 
			const FloatTypeArg *pSrcVec, 
			const FloatTypeArg *pMatrixRow0,
			const FloatTypeArg *pMatrixRow1
		);

		// Multiplies a 3d-vector by a 3x3 matrix memory rows		
		// (The [0] elements of the dest vector to be obtained by performing dot product
		//  on the first three elements ([0] [1] [2]) of the matrix in memory)
		// WARNING: The destination and the source vector pointers must NOT coincide!
		template<class FloatTypeArg>		
		FloatTypeArg *Vec3Matrix33RowDot(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec, const FloatTypeArg *pMatrix);

		template<class FloatTypeArg>
		FloatTypeArg *Vec3Matrix33RowDot
		(
			FloatTypeArg *pDestVec, 
			const FloatTypeArg *pSrcVec, 
			const FloatTypeArg *pMatrixRow0,
			const FloatTypeArg *pMatrixRow1,
			const FloatTypeArg *pMatrixRow2
		);

		// Multiplies a 4d-vector by a 4x4 matrix rows
		template<class FloatTypeArg>		
		FloatTypeArg *Vec4Matrix44RowDot(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec, const FloatTypeArg *pMatrix);

		template<class FloatTypeArg>		
		FloatTypeArg *Vec4Matrix44RowDot
		(
			FloatTypeArg *pDestVec, 
			const FloatTypeArg *pSrcVec, 
			const FloatTypeArg *pMatrixRow0,
			const FloatTypeArg *pMatrixRow1,
			const FloatTypeArg *pMatrixRow2,
			const FloatTypeArg *pMatrixRow3
		);

		// Perform swizzle on a src vector (of up to 4 components) according to a given mask
		// (Zero bit = don't include; one bit = include)
		// The result vector (returned using the destination vector point)
		// will contain number of components corresponding number of components of the mask 1 bits.
		template<class FloatTypeArg>
		FloatTypeArg *SwizzleMask(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec, int InMask);				

		// Negates compoents of a dest vector according to a given mask
		// (1 bit = negate)
		template<class FloatTypeArg>
		FloatTypeArg *NegateMask(FloatTypeArg *pDestVec, int InMask);	

		// Negate sign of all components of the dest vector
		template<class FloatTypeArg>
		FloatTypeArg *Negate_n
		(
			FloatTypeArg *pDestVec,
			int numComponents,
			uint8_t destComponentByteStride = sizeof(FloatTypeArg)
		);

		// Make absolute sign of all components of the dest vector
		template<class FloatTypeArg>
		FloatTypeArg *Absolutize_n
		(
			FloatTypeArg *pDestVec,
			int numComponents,
			uint8_t destComponentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *Absolutize2
		(
			FloatTypeArg *pDestVec,			
			uint8_t destComponentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *Absolutize3
		(
			FloatTypeArg *pDestVec,			
			uint8_t destComponentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *CopyAbsolutize_n
		(
			FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec,
			int numComponents,
			uint8_t destComponentByteStride = sizeof(FloatTypeArg),
			uint8_t srcComponentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *CopyAbsolutize2
		(
			FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec,
			uint8_t destComponentByteStride = sizeof(FloatTypeArg),
			uint8_t srcComponentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *CopyAbsolutize3
		(
			FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec,
			uint8_t destComponentByteStride = sizeof(FloatTypeArg),
			uint8_t srcComponentByteStride = sizeof(FloatTypeArg)
		);

		// Find index of element with minimum value
		template<class FloatTypeArg>
		int MinIndex_n
		(
			const FloatTypeArg *pVec,
			int numComponents,
			uint8_t componentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		int MaximIndex_n
		(
			const FloatTypeArg *pVec,
			int numComponents,
			uint8_t componentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		int Min2Index
		(
			const FloatTypeArg *pVec,
			uint8_t componentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		int Maxim2Index
		(
			const FloatTypeArg *pVec,
			uint8_t componentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		int Min3Index
		(
			const FloatTypeArg *pVec,
			uint8_t componentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		int Maxim3Index
		(
			const FloatTypeArg *pVec,
			uint8_t componentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *CopyVec2(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec);

		template<class FloatTypeArg>
		FloatTypeArg *CopyVec2_n
		(
			FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec,
			size_t DestStrideInBytes = sizeof(FloatTypeArg),
			size_t SrcStrideInBytes = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *CopyVec3(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec);

		template<class FloatTypeArg>
		FloatTypeArg *CopyVec3_n
		(
			FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec,
			size_t DestStrideInBytes = sizeof(FloatTypeArg),
			size_t SrcStrideInBytes = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *CopyVec4(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec);

		template<class FloatTypeArg>
		FloatTypeArg *CopyVec4_n
		(
			FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec,
			size_t DestStrideInBytes = sizeof(FloatTypeArg),
			size_t SrcStrideInBytes = sizeof(FloatTypeArg)
		);		

		template<class FloatTypeArg>
		FloatTypeArg *ResetZeroed2
		(
			FloatTypeArg		*pVec, 
			FloatTypeArg		 X,
			FloatTypeArg		 Y, 
			size_t				 InVecNumComponents, 
			size_t				 InComponentSize = sizeof(FloatTypeArg), 
			size_t				 InFirstElem = 0
		);

		template<class FloatTypeArg>
		FloatTypeArg *ResetZeroed3
		(
			FloatTypeArg		*pVec, 
			FloatTypeArg		 X,
			FloatTypeArg		 Y, 
			FloatTypeArg		 Z, 
			size_t				 InVecNumComponents, 
			size_t				 InComponentSize = sizeof(FloatTypeArg), 
			size_t				 InFirstElem = 0
		);

		template<class FloatTypeArg>
		FloatTypeArg *ResetZeroed4
		(
			FloatTypeArg		*pVec, 
			FloatTypeArg		 X,
			FloatTypeArg		 Y, 
			FloatTypeArg		 Z, 
			FloatTypeArg		 W, 
			size_t				 InVecNumComponents, 
			size_t				 InComponentSize = sizeof(FloatTypeArg), 
			size_t				 InFirstElem = 0
		);

		template<class FloatTypeArg> 
		FloatTypeArg *Set2(FloatTypeArg *pVec, FloatTypeArg X, FloatTypeArg Y);		

		template<class FloatTypeArg> 
		FloatTypeArg *Set3(FloatTypeArg *pVec, FloatTypeArg X, FloatTypeArg Y, FloatTypeArg Z);		

		template<class FloatTypeArg> 
		FloatTypeArg *Set4(FloatTypeArg *pVec, FloatTypeArg X, FloatTypeArg Y, FloatTypeArg Z, FloatTypeArg W);		

		template<class FloatTypeArg> 
		FloatTypeArg *Set2
		(
			FloatTypeArg *pVec, 
			FloatTypeArg X, FloatTypeArg Y,
			size_t DestByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg> 
		FloatTypeArg *Set3
		(
			FloatTypeArg *pVec, 
			FloatTypeArg X, FloatTypeArg Y, FloatTypeArg Z,
			size_t DestByteStride = sizeof(FloatTypeArg)
		);		

		template<class FloatTypeArg> 
		FloatTypeArg *Set4
		(
			FloatTypeArg *pVec, 
			FloatTypeArg X, FloatTypeArg Y, FloatTypeArg Z, FloatTypeArg W,
			size_t DestByteStride = sizeof(FloatTypeArg)
		);		

		// Set all components of the given vector to a given value
		template<class FloatTypeArg> 
		FloatTypeArg *Set_all_n
		(
			FloatTypeArg *pVec,
			FloatTypeArg componentValue, 
			int numComponents,
			uint8_t componentByteStride = sizeof(FloatTypeArg)
		);	

		// Make for the given destination vector
		// components with indices ranging from the given start index to the end of the vector
		// equal to zero.
		template<class DestFloatTypeArg>
		DestFloatTypeArg *MkVecPartZero
		(
			DestFloatTypeArg				*pDestVector,
			size_t							 InSize,
			size_t							 InStartIndex,
			size_t							 InDestComponentStride
		);

		// Zero 2d vector
		template<class FloatTypeArg> 
		FloatTypeArg *MkZero2(FloatTypeArg *pVec);		

		// Zero 3d vector
		template<class FloatTypeArg> 
		FloatTypeArg *MkZero3(FloatTypeArg *pVec);		

		// Make a vector of form (0.0, 0.0, 0.0, ..., 0.0, componentValue, 0.0, 0.0 .... 0.0)
		// where index of the component to set the component value is valueComponentIndex
		template<class FloatTypeArg>
		FloatTypeArg *MkComponent_n
		(
			FloatTypeArg *pVec, 
			FloatTypeArg componentValue, int valueComponentIndex, 
			int numComponents,
			uint8_t componentByteStride = sizeof(FloatTypeArg)
		);		

		// 1. Description:
		//		Set part of the given vector to the values
		// equal to the corresponding values of the vector of form (0.0, 0.0, ... 1.0F, 0.0, .. 0.0)
		// without touching the other vectors components.
		//		The part of the vector to be updated is the part from the given start index
		// to the end of the vector.
		//		if the one component index is not in the range of the indices of part
		// to update, then the corresponding component will not be touched.
		template<class DestFloatTypeArg>
		DestFloatTypeArg *SetIdentityVectorPart
		(
			DestFloatTypeArg		*pDestColumn,
			size_t					 InSize,				// size of the vector
			size_t					 InStartIndex,			// start index 
			size_t					 InOneComponentIndex,	// index of the component that have value of one
			size_t					 InDestComponentStride
		);

		// Zero 4d vector
		template<class FloatTypeArg> 
		FloatTypeArg *MkZero4(FloatTypeArg *pVec);

		template<class FloatTypeArg>
		FloatTypeArg Vec2Length(const FloatTypeArg *pVec);
		
		template<class FloatTypeArg>
		FloatTypeArg Vec3Length(const FloatTypeArg *pVec);
		
		template<class FloatTypeArg>
		FloatTypeArg Vec4Length(const FloatTypeArg *pVec);
		
		template<class FloatTypeArg>
		FloatTypeArg VecLength_n(const FloatTypeArg *pVec, int numComponent, int componentByteStride);

		// WARNING: This is the SQUARED length
		template<class FloatTypeArg>
		FloatTypeArg Vec2SqrLength(const FloatTypeArg *pVec);

		// WARNING: This is the SQUARED length
		template<class FloatTypeArg>
		FloatTypeArg Vec3SqrLength(const FloatTypeArg *pVec);

		// WARNING: This is the SQUARED length
		template<class FloatTypeArg>
		FloatTypeArg Vec4SqrLength(const FloatTypeArg *pVec);

		// WARNING: This is the SQUARED length
		template<class FloatTypeArg>
		FloatTypeArg VecSqrLength_n(const FloatTypeArg *pVec, int numComponent, int componentByteStride);

		// Compoute squared distance between two points in the n-dimensional space
		template<class FloatTypeArg>
		FloatTypeArg SqrDist_n
		(
			const FloatTypeArg *pVec1,
			const FloatTypeArg *pVec2,
			int numComponents,
			uint8_t vec1ComponentByteStride = sizeof(FloatTypeArg),
			uint8_t vec2ComponentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *Mult2Scalar(FloatTypeArg *pDestVec, const FloatTypeArg& scalar);

		template<class FloatTypeArg>
		FloatTypeArg *Mult3Scalar(FloatTypeArg *pDestVec, const FloatTypeArg& scalar);

		template<class FloatTypeArg>
		FloatTypeArg *Mult4Scalar(FloatTypeArg *pDestVec, const FloatTypeArg& scalar);

		template<class FloatTypeArg, class CoeffFloatTypeArg>
		FloatTypeArg *MultScalar_n
		(
			FloatTypeArg		*pDestVec, 
			CoeffFloatTypeArg	 scalar, 
			int					 numComponents,
			uint8_t				 componentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg, class CoeffFloatTypeArg>
		FloatTypeArg *Div2Scalar
		(
			FloatTypeArg		*pDestVec, 
			CoeffFloatTypeArg    scalar, 
			uint8_t				 componentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg, class CoeffFloatTypeArg>
		FloatTypeArg *Div3Scalar
		(
			FloatTypeArg				 *pDestVec,
			CoeffFloatTypeArg			  scalar, 
			uint8_t						  componentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg, class CoeffFloatTypeArg>
		FloatTypeArg *Div4Scalar
		(
			FloatTypeArg				*pDestVec, 
			CoeffFloatTypeArg			 scalar, 
			uint8_t						 componentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg, class CoeffFloatTypeArg>
		FloatTypeArg *DivScalar_n
		(
			FloatTypeArg				*pDestVec, 
			CoeffFloatTypeArg			 scalar, 
			int							 numComponents,
			uint8_t						 componentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *Add2To(FloatTypeArg *pDestVec, const FloatTypeArg *pVec);

		template<class FloatTypeArg>
		FloatTypeArg *Add3To(FloatTypeArg *pDestVec, const FloatTypeArg *pVec);

		template<class FloatTypeArg>
		FloatTypeArg *Add4To(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *Add_n_To
		(
			DestFloatTypeArg *pDestVec, 
			const SrcFloatTypeArg *pVec1, 
			int numComponents,
			uint8_t destComponentByteStride = sizeof(FloatTypeArg),
			uint8_t srcComponentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *SetAdd2(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1, const FloatTypeArg *pVec2);

		template<class FloatTypeArg>
		FloatTypeArg *SetAdd3(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1, const FloatTypeArg *pVec2);

		template<class FloatTypeArg>
		FloatTypeArg *SetAdd4(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1, const FloatTypeArg *pVec2);

		template<class DestFloatTypeArg, class SrcFloatTypeArg, class SrcFloatTypeArgTwo>
		DestFloatTypeArg *SetAdd_n
		(
			DestFloatTypeArg *pDestVec,
			const SrcFloatTypeArg *pVec1,
			const SrcFloatTypeArgTwo *pVec2,
			int numComponents,
			uint8_t destComponentBytesStride = sizeof(DestFloatTypeArg),
			uint8_t vec1ComponentBytesStride = sizeof(SrcFloatTypeArg),
			uint8_t vec2ComponentBytesStride = sizeof(SrcFloatTypeArgTwo)
		);

		template<class FloatTypeArg>
		FloatTypeArg *SetSubtract2(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1, const FloatTypeArg *pVec2);

		template<class FloatTypeArg>
		FloatTypeArg *SetSubtract3(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1, const FloatTypeArg *pVec2);

		template<class FloatTypeArg>
		FloatTypeArg *SetSubtract4(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1, const FloatTypeArg *pVec2);


				template<class DestFloatTypeArg, class SrcFloatTypeArg, class SrcFloatTypeArgOne>
		DestFloatTypeArg *SetSubtract_n
		(
			DestFloatTypeArg				*pDestVec, 
			const SrcFloatTypeArg			*pVec1, 
			const SrcFloatTypeArgOne		*pVec2,
			int								 numComponents,
			uint8_t							 destComponentByteStride = sizeof(DestFloatTypeArg),
			uint8_t							 vec1ComponentByteStride = sizeof(SrcFloatTypeArg),
			uint8_t							 vec2ComponentByteStride = sizeof(SrcFloatTypeArgOne)
		);

		template<class FloatTypeArg>
		FloatTypeArg *Subtract2From(FloatTypeArg *pDestVec, const FloatTypeArg *pVec);

		template<class FloatTypeArg>
		FloatTypeArg *Subtract3From(FloatTypeArg *pDestVec, const FloatTypeArg *pVec);

		template<class FloatTypeArg>
		FloatTypeArg *Subtract4From(FloatTypeArg *pDestVec, const FloatTypeArg *pVec);
		
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *Subtract_n_from
		(
			DestFloatTypeArg			*pDestVec, 
			const SrcFloatTypeArg		*pVec,
			int							 numComponents,	
			uint8_t destComponentByteStride = sizeof(FloatTypeArg),
			uint8_t vecComponentByteStride = sizeof(FloatTypeArg)
		);		

		template<class FloatTypeArg>
		FloatTypeArg Dot2(const FloatTypeArg *pVec1, const FloatTypeArg *pVec2);

		template<class FloatTypeArg>
		FloatTypeArg Dot3(const FloatTypeArg *pVec1, const FloatTypeArg *pVec2);

		template<class FloatTypeArg>
		FloatTypeArg Dot4(const FloatTypeArg *pVec1, const FloatTypeArg *pVec2);

		// Perform a dot product on a 2D and 3D vectors
		// by assuming the third (inspecified) component of the 2D vector is 1
		template<class FloatTypeArg>
		FloatTypeArg Dot2_3(const FloatTypeArg *pVec2D_1, const FloatTypeArg *pVec3D_2);

		template<class FloatTypeArg>
		FloatTypeArg Dot3_4(const FloatTypeArg *pVec3D_1, const FloatTypeArg *pVec4D_2);

		// Sum components of a given vector
		template<class FloatTypeArg>
		FloatTypeArg Sum_n
		(
			const FloatTypeArg *pVec,			
			int numComponents,
			uint8_t componentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg Sum_component_range
		(
			const FloatTypeArg *pVec,			
			int firstComponentIndex, int endComponentIndex,
			uint8_t componentByteStride = sizeof(FloatTypeArg)
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot_n
		(
			const DestFloatTypeArg *pVec1,
			const SrcFloatTypeArg *pVec2,
			int numComponents,
			uint8_t vec1ComponentByteStride,
			uint8_t vec2ComponentByteStride
		);

		// Compute the dot product of vectors 1 and 2
		// (the vector 1 must have less components than vector 2)
		template<class FloatTypeArg>
		FloatTypeArg Dot_m_n
		(
			const FloatTypeArg *pVec1, int numComponents_vec1,
			const FloatTypeArg *pVec2, int numComponents_vec2,			
			uint8_t vec1ComponentByteStride = sizeof(FloatTypeArg),
			uint8_t vec2ComponentByteStride = sizeof(FloatTypeArg)
		);

		// Perform a cross product of two 3d vectors

		template<class DestFloatTypeArg, class SrcFloatTypeArg, class SrcFloatTypeArgOne>
		DestFloatTypeArg *Cross3
		(
			DestFloatTypeArg				*pDestVector,
			const SrcFloatTypeArg			*pVec1,
			const SrcFloatTypeArgOne		*pVec2,
			uint8_t destComponentByteStep = sizeof(DestFloatTypeArg),
			uint8_t vec1ComponentByteStep = sizeof(SrcFloatTypeArg),
			uint8_t vec2ComponentByteStep = sizeof(SrcFloatTypeArgOne)
		);

		template<class FloatTypeArg>
		FloatTypeArg *Cross3
		(
			FloatTypeArg *pDestVector,
			const FloatTypeArg *pVec1,
			const FloatTypeArg *pVec2
		);		
		

		// Advanced functions
		// Lerp knowing length of the interval and start
		template<class FloatTypeArg>
		FloatTypeArg *Vec2LengthLerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pStartVec, const FloatTypeArg *pLengthVec, FloatTypeArg coeff
		);

		template<class FloatTypeArg>
		FloatTypeArg *Vec3LengthLerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pStartVec, const FloatTypeArg *pLengthVec, FloatTypeArg coeff
		);

		template<class FloatTypeArg>
		FloatTypeArg *Vec4LengthLerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pStartVec, const FloatTypeArg *pLengthVec, FloatTypeArg coeff
		);

		// Lerp knowing ends of the interval
		template<class FloatTypeArg>
		FloatTypeArg *Vec2Lerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pVec1, const FloatTypeArg *pVec2, FloatTypeArg coeff
		);

		template<class FloatTypeArg>
		FloatTypeArg *Vec3Lerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pVec1, const FloatTypeArg *pVec2, FloatTypeArg coeff
		);

		template<class FloatTypeArg>
		FloatTypeArg *Vec4Lerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pVec1, const FloatTypeArg *pVec2, FloatTypeArg coeff
		);

		template<class FloatTypeArg>
		FloatTypeArg *Vec_n_Lerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pVec1, const FloatTypeArg *pVec2, 
			FloatTypeArg coeff,
			int numComponents,
			uint8_t destComponentByteStride = sizeof(FloatTypeArg),
			uint8_t vec1ComponentByteStride = sizeof(FloatTypeArg),
			uint8_t vec2ComponentByteStride = sizeof(FloatTypeArg)
		);

		template<class FloatTypeArg>
		FloatTypeArg *Normalize2(FloatTypeArg *pDestVec);

		template<class FloatTypeArg>
		FloatTypeArg *Normalize3(FloatTypeArg *pDestVec);

		template<class FloatTypeArg>
		FloatTypeArg *Normalize4(FloatTypeArg *pDestVec);

		template<class FloatTypeArg>
		FloatTypeArg *Normalize_n
		(
			FloatTypeArg *pDestVec,
			int numComponents,
			uint8_t destComponentStride = sizeof(FloatTypeArg)
		);







		// impl vector utils
		// Vector utils
		template<class FloatTypeArg>
		FloatTypeArg *Vec2Matrix22RowDot(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec, const FloatTypeArg *pMatrix)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			assert(pMatrix != nullptr);
			assert(pDestVec != pSrcVec);
			pDestVec[0] = Dot2(pSrcVec, pMatrix);
			pDestVec[1] = Dot2(pSrcVec, pMatrix + 2);			
			return pDestVec;
		}		

		template<class FloatTypeArg>
		FloatTypeArg *Vec2Matrix22RowDot
		(
			FloatTypeArg *pDestVec, 
			const FloatTypeArg *pSrcVec, 
			const FloatTypeArg *pMatrixRow0,
			const FloatTypeArg *pMatrixRow1
		)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			assert(pMatrixRow0 != nullptr);
			assert(pMatrixRow1 != nullptr);
			assert(pDestVec != pSrcVec);
			pDestVec[0] = Dot2(pSrcVec, pMatrixRow0);
			pDestVec[1] = Dot2(pSrcVec, pMatrixRow1);			
			return pDestVec;		
		}

		template<class FloatTypeArg>
		FloatTypeArg *Vec3Matrix33RowDot(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec, const FloatTypeArg *pMatrix)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			assert(pMatrix != nullptr);
			assert(pDestVec != pSrcVec);
			pDestVec[0] = Dot3(pSrcVec, pMatrix);
			pDestVec[1] = Dot3(pSrcVec, pMatrix + 3);
			pDestVec[2] = Dot3(pSrcVec, pMatrix + 3 * 2);
			return pDestVec;
		}

		template<class FloatTypeArg>		
		FloatTypeArg *Vec4Matrix44RowDot(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec, const FloatTypeArg *pMatrix)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			assert(pMatrix != nullptr);
			assert(pDestVec != pSrcVec);
			pDestVec[0] = Dot4(pSrcVec, pMatrix);
			pDestVec[1] = Dot4(pSrcVec, pMatrix + 4);
			pDestVec[2] = Dot4(pSrcVec, pMatrix + 4 * 2);
			pDestVec[3] = Dot4(pSrcVec, pMatrix + 4 * 3);
			return pDestVec;
		}

		template<class FloatTypeArg>		
		FloatTypeArg *Vec4Matrix44RowDot
		(
			FloatTypeArg *pDestVec, 
			const FloatTypeArg *pSrcVec, 
			const FloatTypeArg *pMatrixRow0,
			const FloatTypeArg *pMatrixRow1,
			const FloatTypeArg *pMatrixRow2,
			const FloatTypeArg *pMatrixRow3
		)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			assert(pMatrixRow0 != nullptr);
			assert(pMatrixRow1 != nullptr);
			assert(pMatrixRow2 != nullptr);
			assert(pMatrixRow3 != nullptr);
			assert(pDestVec != pSrcVec);
			pDestVec[0] = Dot4(pSrcVec, pMatrixRow0);
			pDestVec[1] = Dot4(pSrcVec, pMatrixRow1);
			pDestVec[2] = Dot4(pSrcVec, pMatrixRow2);
			pDestVec[3] = Dot4(pSrcVec, pMatrixRow3);
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Vec3Matrix33RowDot
		(
			FloatTypeArg *pDestVec, 
			const FloatTypeArg *pSrcVec, 
			const FloatTypeArg *pMatrixRow0,
			const FloatTypeArg *pMatrixRow1,
			const FloatTypeArg *pMatrixRow2
		)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			assert(pMatrixRow0 != nullptr);
			assert(pMatrixRow1 != nullptr);
			assert(pMatrixRow2 != nullptr);
			assert(pDestVec != pSrcVec);
			pDestVec[0] = Dot3(pSrcVec, pMatrixRow0);
			pDestVec[1] = Dot3(pSrcVec, pMatrixRow1);
			pDestVec[2] = Dot3(pSrcVec, pMatrixRow2);
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Normalize_n
		(
			FloatTypeArg			*pDestVec,
			int						 numComponents,
			uint8_t					 destComponentStride
		)
		{
			assert(pDestVec != nullptr);
			FloatTypeArg dest_length = VecLength_n(pDestVec, numComponents, destComponentStride);
			for(int i = 0; i < numComponents; i++)
			{
				FloatTypeArg *pComponent = Util::AddPtr(pDestVec, i * destComponentStride);
				*pComponent /= dest_length;
			}
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg Vec2Length(const FloatTypeArg *pVec)
		{
			FloatTypeArg sqr_length = Vec2Length(pVec);
			return Sqrt(sqr_length);
		}
		
		template<class FloatTypeArg>
		FloatTypeArg Vec3Length(const FloatTypeArg *pVec)
		{
			FloatTypeArg sqr_length = Vec3Length(pVec);
			return Sqrt(sqr_length);
		}
		
		template<class FloatTypeArg>
		FloatTypeArg Vec4Length(const FloatTypeArg *pVec)
		{
			FloatTypeArg sqr_length = Vec4Length(pVec);
			return Sqrt(sqr_length);
		}
		
		template<class FloatTypeArg>
		FloatTypeArg VecLength_n(const FloatTypeArg *pVec, int numComponents, int componentByteStride)
		{
			FloatTypeArg sqr_length = VecSqrLength_n(pVec, numComponents, componentByteStride);
			return Sqrt(sqr_length);
		}

		template<class FloatTypeArg>
		FloatTypeArg Vec2SqrLength(const FloatTypeArg *pVec)
		{
			return Dot2(pVec, pVec);
		}

		template<class FloatTypeArg>
		FloatTypeArg Vec3SqrLength(const FloatTypeArg *pVec)
		{
			return Dot3(pVec, pVec);
		}

		template<class FloatTypeArg>
		FloatTypeArg Vec4SqrLength(const FloatTypeArg *pVec)
		{
			return Dot4(pVec, pVec);
		}

		template<class FloatTypeArg>
		FloatTypeArg VecSqrLength_n(const FloatTypeArg *pVec, int numComponents, int componentByteStride)
		{
			return Dot_n(pVec, pVec, numComponents, componentByteStride, componentByteStride);
		}



		template<class FloatTypeArg>
		FloatTypeArg *Mult2Scalar(FloatTypeArg *pDestVec, const FloatTypeArg& scalar)
		{
			pDestVec[0] *= scalar;
			pDestVec[1] *= scalar;
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Mult3Scalar(FloatTypeArg *pDestVec, const FloatTypeArg& scalar)
		{
			pDestVec[0] *= scalar;
			pDestVec[1] *= scalar;
			pDestVec[2] *= scalar;
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Mult4Scalar(FloatTypeArg *pDestVec, const FloatTypeArg& scalar)
		{
			pDestVec[0] *= scalar;
			pDestVec[1] *= scalar;
			pDestVec[2] *= scalar;
			pDestVec[3] *= scalar;
			return pDestVec;
		}

		template<class FloatTypeArg, class CoeffFloatTypeArg>
		FloatTypeArg *MultScalar_n
		(
			FloatTypeArg		*pDestVec, 
			CoeffFloatTypeArg	 scalar, 
			int numComponents,
			uint8_t componentByteStride
		)
		{
			assert(pDestVec != nullptr);
			for(int i = 0; i < numComponents; i++)
			{
				typename FloatTypeArg *pComponent = Util::AddPtr(pDestVec, i *componentByteStride);
				*pComponent = *pComponent * scalar;
			}
			return pDestVec;
		}

		template<class FloatTypeArg, class CoeffFloatTypeArg>
		FloatTypeArg *Div2Scalar
		(
			FloatTypeArg			*pDestVec, 
			CoeffFloatTypeArg		 scalar, 
			uint8_t					 componentByteStride
		)
		{
			return DivScalar_n(pDestVec, scalar, 2, componentByteStride);
		}

		template<class FloatTypeArg, class CoeffFloatTypeArg>
		FloatTypeArg *Div3Scalar
		(
			FloatTypeArg					*pDestVec,
			CoeffFloatTypeArg				 scalar, 
			uint8_t							 componentByteStride
		)
		{
			return DivScalar_n(pDestVec, scalar, 3, componentByteStride);
		}

		template<class FloatTypeArg, class CoeffFloatTypeArg>
		FloatTypeArg *Div4Scalar
		(
			FloatTypeArg					*pDestVec, 
			CoeffFloatTypeArg				 scalar, 
			uint8_t							 componentByteStride
		)
		{
			return DivScalar_n(pDestVec, scalar, 4, componentByteStride);
		}

		template<class FloatTypeArg, class CoeffFloatTypeArg>
		FloatTypeArg *DivScalar_n
		(
			FloatTypeArg			*pDestVec, 
			CoeffFloatTypeArg		 scalar, 
			int						 numComponents,
			uint8_t					 componentByteStride
		)
		{			
			assert(pDestVec != nullptr);
			assert(scalar > 0.0);
			for(int i = 0; i < numComponents; i++)
			{
				typename FloatTypeArg *pComponent = Util::AddPtr(pDestVec, i *componentByteStride);
				*pComponent = *pComponent / scalar;
			}
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Add2To(FloatTypeArg *pDestVec, const FloatTypeArg *pVec)
		{
			pDestVec[0] += pVec[0];
			pDestVec[1] += pVec[1];
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Add3To(FloatTypeArg *pDestVec, const FloatTypeArg *pVec)
		{
			pDestVec[0] += pVec[0];
			pDestVec[1] += pVec[1];
			pDestVec[2] += pVec[2];
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Add4To(FloatTypeArg *pDestVec, const FloatTypeArg *pVec)
		{
			pDestVec[0] += pVec[0];
			pDestVec[1] += pVec[1];
			pDestVec[2] += pVec[2];
			pDestVec[3] += pVec[3];
			return pDestVec;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *Add_n_To
		(
			DestFloatTypeArg				*pDestVec, 
			const SrcFloatTypeArg			*pVec1, 
			int								 numComponents,
			uint8_t							 destComponentByteStride,
			uint8_t							 srcComponentByteStride
		)
		{
			assert(pDestVec != nullptr);
			assert(pVec1 != nullptr);
			assert(numComponents >= 0);
			for(int index = 0; index < numComponents; index++)
			{
				DestFloatTypeArg *pDestComponent = Util::AddPtr(pDestVec, index * destComponentByteStride);
				const SrcFloatTypeArg *pSrcComponent = Util::AddPtr(pVec1, index * srcComponentByteStride);
				*pDestComponent += *pSrcComponent;
			}
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *SetAdd2(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1, const FloatTypeArg *pVec2)
		{
			assert(pDestVec);
			assert(pVec1);
			assert(pVec2);
			CopyVec2To(pDestVec, pVec1);
			return Add2To(pDestVec, pVec2);
		}

		template<class FloatTypeArg>
		FloatTypeArg *SetAdd3(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1, const FloatTypeArg *pVec2)
		{
			assert(pDestVec);
			assert(pVec1);
			assert(pVec2);
			CopyVec3To(pDestVec, pVec1);
			return Add3To(pDestVec, pVec2);
		}

		template<class FloatTypeArg>
		FloatTypeArg *SetAdd4(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1, const FloatTypeArg *pVec2)
		{
			assert(pDestVec);
			assert(pVec1);
			assert(pVec2);
			CopyVec4To(pDestVec, pVec1);
			return Add4To(pDestVec, pVec2);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg, class SrcFloatTypeArgTwo>
		DestFloatTypeArg *SetAdd_n
		(
			DestFloatTypeArg *pDestVec,
			const SrcFloatTypeArg *pVec1,
			const SrcFloatTypeArgTwo *pVec2,
			int numComponents,
			uint8_t destComponentBytesStride,
			uint8_t vec1ComponentBytesStride,
			uint8_t vec2ComponentBytesStride
		)
		{
			assert(pDestVec != nullptr);
			assert(pVec1 != nullptr);
			assert(pVec2 != nullptr);
			for(int i = 0; i < numComponents; i++)
			{
				typename DestFloatTypeArg *pDestComponent					= Util::AddPtr(pDestVec, i * destComponentBytesStride);
				typename const SrcFloatTypeArg *pFirstComponent				= Util::AddPtr(pVec1, i * vec1ComponentBytesStride);
				typename const SrcFloatTypeArgTwo *pSecondComponent			= Util::AddPtr(pVec2, i * vec2ComponentBytesStride);
				*pDestComponent = *pFirstComponent + *pSecondComponent;
			}
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *SetSubtract2(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1, const FloatTypeArg *pVec2)
		{
			assert(pDestVec);
			assert(pVec1);
			assert(pVec2);
			CopyVec2To(pDestVec, pVec1);
			return Subtract2From(pDestVec, pVec2);
		}

		template<class FloatTypeArg>
		FloatTypeArg *SetSubtract3(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1, const FloatTypeArg *pVec2)
		{
			assert(pDestVec);
			assert(pVec1);
			assert(pVec2);
			CopyVec3To(pDestVec, pVec1);
			return Subtract3From(pDestVec, pVec2);
		}

		template<class FloatTypeArg>
		FloatTypeArg *SetSubtract4(FloatTypeArg *pDestVec, const FloatTypeArg *pVec1, const FloatTypeArg *pVec2)
		{
			assert(pDestVec);
			assert(pVec1);
			assert(pVec2);
			CopyVec4To(pDestVec, pVec1);
			return Subtract4From(pDestVec, pVec2);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg, class SrcFloatTypeArgOne>
		DestFloatTypeArg *SetSubtract_n
		(
			DestFloatTypeArg				*pDestVec, 
			const SrcFloatTypeArg			*pVec1, 
			const SrcFloatTypeArgOne		*pVec2,
			int								 numComponents,
			uint8_t							 destComponentByteStride,
			uint8_t							 vec1ComponentByteStride,
			uint8_t							 vec2ComponentByteStride
		)
		{
			assert(pDestVec != nullptr);
			assert(pVec1 != nullptr);
			assert(pVec2 != nullptr);
			for(int i = 0; i < numComponents; i++)
			{
				typename DestFloatTypeArg *pDestComponent = Util::AddPtr(pDestVec, destComponentByteStride * i);
				typename const SrcFloatTypeArg *pFirstComponent = Util::AddPtr(pVec1, vec1ComponentByteStride * i);
				typename const SrcFloatTypeArgOne *pSecondComponent = Util::AddPtr(pVec2, vec2ComponentByteStride * i);
				*pDestComponent = *pFirstComponent - *pSecondComponent;
			}
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Subtract2From(FloatTypeArg *pDestVec, const FloatTypeArg *pVec)
		{
			pDestVec[0] -= pVec[0];
			pDestVec[1] -= pVec[1];
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Subtract3From(FloatTypeArg *pDestVec, const FloatTypeArg *pVec)
		{
			pDestVec[0] -= pVec[0];
			pDestVec[1] -= pVec[1];
			pDestVec[2] -= pVec[2];
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Subtract4From(FloatTypeArg *pDestVec, const FloatTypeArg *pVec)
		{
			pDestVec[0] -= pVec[0];
			pDestVec[1] -= pVec[1];
			pDestVec[2] -= pVec[2];
			pDestVec[3] -= pVec[3];
			return pDestVec;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *Subtract_n_from
		(
			DestFloatTypeArg			*pDestVec, 
			const SrcFloatTypeArg		*pVec,
			int							 numComponents,
			uint8_t						 destComponentByteStride,
			uint8_t						 vecComponentByteStride
		)
		{
			assert(pDestVec != nullptr);
			assert(pVec != nullptr);
			for(int i = 0; i < numComponents; i++)
			{
				typename DestFloatTypeArg *pDestComponent		= Util::AddPtr(pDestVec, i * destComponentByteStride);
				typename const SrcFloatTypeArg *pComponent		= Util::AddPtr(pVec, i * vecComponentByteStride);
				*pDestComponent -= *pComponent;
			}
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg Dot2(const FloatTypeArg *pVec1, const FloatTypeArg *pVec2)
		{
			return pVec1[0] * pVec2[0] + pVec1[1] * pVec2[1];
		}

		template<class FloatTypeArg>
		FloatTypeArg Dot3(const FloatTypeArg *pVec1, const FloatTypeArg *pVec2)
		{
			return pVec1[0] * pVec2[0] + pVec1[1] * pVec2[1] + pVec1[2] * pVec2[2];
		}

		template<class FloatTypeArg>
		FloatTypeArg Dot4(const FloatTypeArg *pVec1, const FloatTypeArg *pVec2)
		{
			return pVec1[0] * pVec2[0] + pVec1[1] * pVec2[1] + pVec1[2] * pVec2[2] + pVec1[3] * pVec2[3];
		}

		template<class FloatTypeArg>
		FloatTypeArg Sum_component_range
		(
			const FloatTypeArg *pVec,			
			int firstComponentIndex, int endComponentIndex,
			uint8_t componentByteStride
		)
		{
			typename FloatTypeArg res_sum = 0;
			for(int i = firstComponentIndex; i < endComponentIndex; i++)
			{
				res_sum += *Util::AddPtr(pVec, i * componentByteStride);
			}
			return res_sum;
		}

		template<class FloatTypeArg>
		FloatTypeArg Sum_n
		(
			const FloatTypeArg *pVec,			
			int numComponents,
			uint8_t componentByteStride	
		)
		{
			typename FloatTypeArg res_sum = 0;
			for(int i = 0; i < numComponents; i++)
			{
				res_sum += *Util::AddPtr(pVec, i * componentByteStride);
			}
			return res_sum;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot_n
		(
			const DestFloatTypeArg *pVec1,
			const SrcFloatTypeArg *pVec2,
			int numComponents,
			uint8_t vec1ComponentByteStride,
			uint8_t vec2ComponentByteStride
		)
		{
			typename DestFloatTypeArg res_dot = 0;
			for(int i = 0; i < numComponents; i++)
			{
				DestFloatTypeArg dest_component = *Util::AddPtr(pVec1, vec1ComponentByteStride * i);
				DestFloatTypeArg src_component = *Util::AddPtr(pVec2, vec2ComponentByteStride * i);
				res_dot += dest_component * src_component;
			}
			return res_dot;
		}

		template<class FloatTypeArg>
		FloatTypeArg Dot_m_n
		(
			const FloatTypeArg *pVec1, int numComponents_vec1,
			const FloatTypeArg *pVec2, int numComponents_vec2,			
			uint8_t vec1ComponentByteStride,
			uint8_t vec2ComponentByteStride
		)
		{
			assert(numComponents_vec1 < numComponents_vec2);
			typename FloatTypeArg res_dot = Dot_n(pVec1, pVec2, numComponents_vec1, vec1ComponentByteStride, vec2ComponentByteStride);
			typename FloatTypeArg residue_sum = Sum_component_range(pVec2, numComponents_vec1, numComponents_vec2, vec2ComponentByteStride);
			return res_dot + residue_sum;
		}

		template<class FloatTypeArg>
		FloatTypeArg *ResetZeroed2
		(
			FloatTypeArg		*pVec, 
			FloatTypeArg		 X,
			FloatTypeArg		 Y, 
			size_t				 InVecNumComponents, 
			size_t				 InComponentSize,
			size_t				 InFirstElem
		)
		{
			assert(pVec != nullptr);
			assert(InFirstElem >= 0);
			assert(InFirstElem < InVecNumComponents);
			Set_all_n(pVec, (FloatTypeArg)0, InVecNumComponents, InComponentSize);
			Set2(Util::AddPtr(pVec, InFirstElem * InComponentSize), X, Y, InComponentSize);
			return pVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *ResetZeroed3
		(
			FloatTypeArg		*pVec, 
			FloatTypeArg		 X,
			FloatTypeArg		 Y, 
			FloatTypeArg		 Z, 
			size_t				 InVecNumComponents, 
			size_t				 InComponentSize, 
			size_t				 InFirstElem
		)
		{
			assert(pVec != nullptr);
			assert(InFirstElem >= 0);
			assert(InFirstElem < InVecNumComponents);
			Set_all_n(pVec, (FloatTypeArg)0, InVecNumComponents, InComponentSize);
			Set3(Util::AddPtr(pVec, InFirstElem * InComponentSize), X, Y, Z, InComponentSize);
			return pVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *ResetZeroed4
		(
			FloatTypeArg		*pVec, 
			FloatTypeArg		 X,
			FloatTypeArg		 Y, 
			FloatTypeArg		 Z, 
			FloatTypeArg		 W, 
			size_t				 InVecNumComponents, 
			size_t				 InComponentSize, 
			size_t				 InFirstElem
		)		
		{
			assert(pVec != nullptr);
			assert(InFirstElem >= 0);
			assert(InFirstElem < InVecNumComponents);
			Set_all_n(pVec, (FloatTypeArg)0, InVecNumComponents, InComponentSize);
			Set4(Util::AddPtr(pVec, InFirstElem * InComponentSize), X, Y, Z, W, InComponentSize);
			return pVec;
		}

		template<class FloatTypeArg> 
		FloatTypeArg *Set2(const FloatTypeArg *pVec, FloatTypeArg X, FloatTypeArg Y)
		{
			assert(pVec != nullptr);
			pVec[0] = X;
			pVec[1] = Y;
		}

		template<class FloatTypeArg> 
		FloatTypeArg *Set3(FloatTypeArg *pVec, FloatTypeArg X, FloatTypeArg Y, FloatTypeArg Z)
		{
			assert(pVec != nullptr);
			pVec[0] = X;
			pVec[1] = Y;
			pVec[2] = Z;
			return pVec;
		}

		template<class FloatTypeArg> 
		FloatTypeArg *Set4(FloatTypeArg *pVec, FloatTypeArg X, FloatTypeArg Y, FloatTypeArg Z, FloatTypeArg W)
		{
			assert(pVec != nullptr);
			pVec[0] = X;
			pVec[1] = Y;
			pVec[2] = Z;
			pVec[3] = W;
			return pVec;
		}

		template<class FloatTypeArg> 
		FloatTypeArg *Set2
		(
			FloatTypeArg *pVec, 
			FloatTypeArg X, FloatTypeArg Y,
			size_t DestByteStride
		)
		{
			assert(pVec != nullptr);
			FloatTypeArg *pX = Util::AddPtr(pVec, 0);
			FloatTypeArg *pY = Util::AddPtr(pVec, DestByteStride);		
			*pX = X;
			*pY = Y;
			return pVec;
		}

		template<class FloatTypeArg> 
		FloatTypeArg *Set3
		(
			FloatTypeArg *pVec, 
			FloatTypeArg X, FloatTypeArg Y, FloatTypeArg Z,
			size_t DestByteStride
		)
		{
			assert(pVec != nullptr);
			FloatTypeArg *pX = Util::AddPtr(pVec, 0);
			FloatTypeArg *pY = Util::AddPtr(pVec, DestByteStride);
			FloatTypeArg *pZ = Util::AddPtr(pVec, 2 * DestByteStride);
			*pX = X;
			*pY = Y;
			*pZ = Z;	
			return pVec;
		}

		template<class FloatTypeArg> 
		FloatTypeArg *Set4
		(
			FloatTypeArg *pVec, 
			FloatTypeArg X, FloatTypeArg Y, FloatTypeArg Z, FloatTypeArg W,
			size_t DestByteStride
		)
		{
			assert(pVec != nullptr);
			FloatTypeArg *pX = Util::AddPtr(pVec, 0);
			FloatTypeArg *pY = Util::AddPtr(pVec, DestByteStride);
			FloatTypeArg *pZ = Util::AddPtr(pVec, 2 * DestByteStride);
			FloatTypeArg *pW = Util::AddPtr(pVec, 3 * DestByteStride);
			*pX = X;
			*pY = Y;
			*pZ = Z;		
			*pW = W;
			return pVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *CopyVec2(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			pDestVec[0] = pSrcVec[0];
			pDestVec[1] = pSrcVec[1];
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *CopyVec2_n
		(
			FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec,
			size_t DestStrideInBytes,
			size_t SrcStrideInBytes
		)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			*Util::AddPtr(pDestVec, 0) = *Util::AddPtr(pSrcVec, 0);
			*Util::AddPtr(pDestVec, DestStrideInBytes) = *Util::AddPtr(pSrcVec, SrcStrideInBytes);						
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *CopyVec3(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			pDestVec[0] = pSrcVec[0];
			pDestVec[1] = pSrcVec[1];
			pDestVec[2] = pSrcVec[2];
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *CopyVec3_n
		(
			FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec,
			size_t DestStrideInBytes,
			size_t SrcStrideInBytes
		)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			*Util::AddPtr(pDestVec, 0) = *Util::AddPtr(pSrcVec, 0);
			*Util::AddPtr(pDestVec, DestStrideInBytes) = *Util::AddPtr(pSrcVec, SrcStrideInBytes);			
			*Util::AddPtr(pDestVec, 2 * DestStrideInBytes) = *Util::AddPtr(pSrcVec, 2 *SrcStrideInBytes);			
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *CopyVec4(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			pDestVec[0] = pSrcVec[0];
			pDestVec[1] = pSrcVec[1];
			pDestVec[2] = pSrcVec[2];
			pDestVec[3] = pSrcVec[3];
			return pDestVec;
		}		

		template<class FloatTypeArg>
		FloatTypeArg *CopyVec4_n
		(
			FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec,
			size_t DestStrideInBytes,
			size_t SrcStrideInBytes
		)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			*Util::AddPtr(pDestVec, 0) = *Util::AddPtr(pSrcVec, 0);
			*Util::AddPtr(pDestVec, DestStrideInBytes) = *Util::AddPtr(pSrcVec, SrcStrideInBytes);			
			*Util::AddPtr(pDestVec, 2 * DestStrideInBytes) = *Util::AddPtr(pSrcVec, 2 *SrcStrideInBytes);			
			*Util::AddPtr(pDestVec, 3 * DestStrideInBytes) = *Util::AddPtr(pSrcVec, 3 *SrcStrideInBytes);			
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *SwizzleMask(FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec, int InMask)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			assert(pSrcVec != pDestVec);
			int dest_component_index = 0;
			if(InMask & (1 << 0))
			{
				pDestVec[dest_component_index] = pSrcVec[0];
				dest_component_index++;
			}
			if(InMask & (1 << 1))
			{
				pDestVec[dest_component_index] = pSrcVec[1];
				dest_component_index++;
			}
			if(InMask & (1 << 2))
			{
				pDestVec[dest_component_index] = pSrcVec[2];
				dest_component_index++;
			}
			if(InMask & (1 << 3))
			{
				pDestVec[dest_component_index] = pSrcVec[3];
				dest_component_index++;
			}
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *NegateMask(FloatTypeArg *pDestVec, int InMask)
		{
			assert(pDestVec != nullptr);						
			if(InMask & (1 << 0))
			{
				pDestVec[0] = -pDestVec[0];
				dest_component_index++;
			}
			if(InMask & (1 << 1))
			{
				pDestVec[1] = -pDestVec[1];				
			}
			if(InMask & (1 << 2))
			{
				pDestVec[2] = -pDestVec[2];
			}
			if(InMask & (1 << 3))
			{
				pDestVec[3] = -pDestVec[3];
			}
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Negate_n
		(
			FloatTypeArg *pDestVec,
			int numComponents,
			uint8_t destComponentByteStride
		)
		{
			assert(pDestVec);
			for(int i = 0; i < numComponents; i++)
			{
				typename FloatTypeArg *pComponent = Util::AddPtr(pDestVec, i * destComponentByteStride);
				*pComponent = -*pComponent;
			}
			return pDestVec;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCast2
		(
			DestFloatTypeArg *pDestVec, const SrcFloatTypeArg *pSrcVec, 
			uint8_t destComponentByteStep, 
			uint8_t srcComponentByteStep
		)
		{
			return CopyCast_n(pDestVec, pSrcVec, 2, destComponentByteStep, srcComponentByteStep);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCast3
		(
			DestFloatTypeArg *pDestVec, const SrcFloatTypeArg *pSrcVec, 
			uint8_t destComponentByteStep , 
			uint8_t srcComponentByteStep
		)
		{
			return CopyCast_n(pDestVec, pSrcVec, 3, destComponentByteStep, srcComponentByteStep);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCast4
		(
			DestFloatTypeArg *pDestVec, const SrcFloatTypeArg *pSrcVec, 
			uint8_t destComponentByteStep, 
			uint8_t srcComponentByteStep
		)
		{
			return CopyCast_n(pDestVec, pSrcVec, 4, destComponentByteStep, srcComponentByteStep);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCast_n
		(
			DestFloatTypeArg *pDestVec, const SrcFloatTypeArg *pSrcVec, 
			int numComponents,
			uint8_t destComponentByteStep, 
			uint8_t srcComponentByteStep
		)
		{			
			for(int component_index = 0; component_index < numComponents; component_index++)
			{
				DestFloatTypeArg	  *pDestComponent = Util::AddPtr(pDestVec, component_index * destComponentByteStep);
				const SrcFloatTypeArg *pSrcComponent  = Util::AddPtr(pSrcVec, component_index * srcComponentByteStep);
				*pDestComponent = static_cast<DestFloatTypeArg>(*pSrcComponent);
			}
			return pDestVec;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCastZeroed_n
		(
			DestFloatTypeArg *pDestVec, const SrcFloatTypeArg *pSrcVec,
			int numDestComponents,
			int numSrcComponents,
			uint8_t destComponentByteStep, 
			uint8_t srcComponentByteStep
		)
		{
			Set_all_n(pDestVec, (DestFloatTypeArg)0, numDestComponents, destComponentByteStep);
			CopyCast_n(pDestVec, pSrcVec, numSrcComponents, destComponentByteStep, srcComponentByteStep);
			return pDestVec;
		}

		// advanced functions
		template<class FloatTypeArg>
		FloatTypeArg *Vec2LengthLerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pStartVec, const FloatTypeArg *pLengthVec, FloatTypeArg coeff
		)
		{
			// 1 mult and 1 add
			pResultVec[0] = pStartVec[0] + pLengthVec[0] * coeff;
			// 1 mult and 1 add
			pResultVec[1] = pStartVec[1] + pLengthVec[1] * coeff;
			// total: 2 mults, 2 adds
			return pResultVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Vec3LengthLerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pStartVec, const FloatTypeArg *pLengthVec, FloatTypeArg coeff
		)
		{
			// 1 mult and 1 add
			pResultVec[0] = pStartVec[0] + pLengthVec[0] * coeff;
			// 1 mult and 1 add
			pResultVec[1] = pStartVec[1] + pLengthVec[1] * coeff;
			// 1 mult and 1 add
			pResultVec[2] = pStartVec[2] + pLengthVec[2] * coeff;
			// total: 3 mults, 3 adds
			return pResultVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Vec4LengthLerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pStartVec, const FloatTypeArg *pLengthVec, FloatTypeArg coeff
		)
		{
			// 1 mult and 1 add
			pResultVec[0] = pStartVec[0] + pLengthVec[0] * coeff;
			// 1 mult and 1 add
			pResultVec[1] = pStartVec[1] + pLengthVec[1] * coeff;
			// 1 mult and 1 add
			pResultVec[2] = pStartVec[2] + pLengthVec[2] * coeff;
			// 1 mult and 1 add
			pResultVec[3] = pStartVec[3] + pLengthVec[3] * coeff;
			// total: 4 mults, 4 adds
			return pResultVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Vec2Lerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pVec1, const FloatTypeArg *pVec2, FloatTypeArg coeff
		)
		{
			assert(pResultVec != nullptr);
			assert(pVec1 != nullptr);
			assert(pVec2 != nullptr);
			pResultVec[0] = pVec1[0] + coeff * (pVec2[0] - pVec1[0]);
			pResultVec[1] = pVec1[1] + coeff * (pVec2[1] - pVec1[1]);
		}

		template<class FloatTypeArg>
		FloatTypeArg *Vec3Lerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pVec1, const FloatTypeArg *pVec2, FloatTypeArg coeff
		)
		{
			assert(pResultVec != nullptr);
			assert(pVec1 != nullptr);
			assert(pVec2 != nullptr);
			pResultVec[0] = pVec1[0] + coeff * (pVec2[0] - pVec1[0]);
			pResultVec[1] = pVec1[1] + coeff * (pVec2[1] - pVec1[1]);
			pResultVec[2] = pVec1[2] + coeff * (pVec2[2] - pVec1[2]);
		}

		template<class FloatTypeArg>
		FloatTypeArg *Vec4Lerp
		(
			FloatTypeArg *pResultVec, 
			const FloatTypeArg *pVec1, const FloatTypeArg *pVec2, FloatTypeArg coeff
		)
		{
			assert(pResultVec != nullptr);
			assert(pVec1 != nullptr);
			assert(pVec2 != nullptr);
			pResultVec[0] = pVec1[0] + coeff * (pVec2[0] - pVec1[0]);
			pResultVec[1] = pVec1[1] + coeff * (pVec2[1] - pVec1[1]);
			pResultVec[2] = pVec1[2] + coeff * (pVec2[2] - pVec1[2]);
			pResultVec[3] = pVec1[3] + coeff * (pVec2[3] - pVec1[3]);
		}

		template<class FloatTypeArg>
		FloatTypeArg *Cross3
		(
			FloatTypeArg *pDestVector,
			const FloatTypeArg *pVec1,
			const FloatTypeArg *pVec2			
		)
		{
			return Cross3(pDestVector, pVec1, pVec2);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg, class SrcFloatTypeArgOne>
		DestFloatTypeArg *Cross3
		(
			DestFloatTypeArg				*pDestVec,
			const SrcFloatTypeArg			*pVec1,
			const SrcFloatTypeArgOne		*pVec2,
			uint8_t							 destComponentByteStep,
			uint8_t							 vec1ComponentByteStep,
			uint8_t							 vec2ComponentByteStep
		)
		{
			assert(pDestVec != nullptr);
			assert(pVec1 != nullptr);
			assert(pVec2 != nullptr);
			assert(destComponentByteStep > 0);
			assert(vec1ComponentByteStep > 0);
			assert(vec2ComponentByteStep > 0);
			typename const DestFloatTypeArg		X1 = *Util::AddPtr(pVec1, vec1ComponentByteStep * 0);
			typename const DestFloatTypeArg		X2 = *Util::AddPtr(pVec2, vec2ComponentByteStep * 0);
			typename const DestFloatTypeArg		Y1 = *Util::AddPtr(pVec1, vec1ComponentByteStep * 1);
			typename const DestFloatTypeArg		Y2 = *Util::AddPtr(pVec2, vec2ComponentByteStep * 1);
			typename const DestFloatTypeArg		Z1 = *Util::AddPtr(pVec1, vec1ComponentByteStep * 2);
			typename const DestFloatTypeArg		Z2 = *Util::AddPtr(pVec2, vec2ComponentByteStep * 2);
			// 2 mults, 1 add
			*Util::AddPtr(pDestVec, destComponentByteStep * 0) = Det2x2(Y1, Z1, Y2, Z2);
			// 2 mults, 1 add
			*Util::AddPtr(pDestVec, destComponentByteStep * 1) = -Det2x2(X1, Z1, X2, Z2);
			// 2 mults, 1 add
			*Util::AddPtr(pDestVec, destComponentByteStep * 2) = Det2x2(X1, Y1, X2, Y2);
			// total: 6 mults: 3 adds; =9 ops
			return pDestVec;
		}		

		template<class FloatTypeArg>
		FloatTypeArg *Absolutize2
		(
			FloatTypeArg *pDestVec,			
			uint8_t destComponentByteStride
		)
		{
			return Absolutize_n(pDestVec, 2, destComponentByteStride);
		}

		template<class FloatTypeArg>
		FloatTypeArg *Absolutize3
		(
			FloatTypeArg *pDestVec,			
			uint8_t destComponentByteStride
		)
		{
			return Absolutize_n(pDestVec, 3, destComponentByteStride);
		}

		template<class FloatTypeArg>
		FloatTypeArg *Absolutize_n
		(
			FloatTypeArg *pDestVec,
			int numComponents,
			uint8_t destComponentByteStride
		)
		{
			assert(pDestVec != nullptr);
			for(int i = 0; i < numComponents; i++)
			{
				FloatTypeArg *pComponent = Util::AddPtr(pDestVec, i * destComponentByteStride);
				*pComponent = Abs(*pComponent);
			};
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *CopyAbsolutize_n
		(
			FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec,
			int numComponents,
			uint8_t destComponentByteStride,
			uint8_t srcComponentByteStride
		)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);						
			for(int i = 0; i < numComponents; i++)
			{
				FloatTypeArg *pComponent = Util::AddPtr(pDestVec, i * destComponentByteStride);
				FloatTypeArg *pSrcComponent = Util::AddPtr(pSrcVec, i * srcComponentByteStride);
				*pComponent = Abs(*pSrcComponent);
			};
			return pDestVec;
		}

		template<class FloatTypeArg>
		FloatTypeArg *CopyAbsolutize2
		(
			FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec,
			uint8_t destComponentByteStride,
			uint8_t srcComponentByteStride
		)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			return CopyAbsolutize_n(pDestVec, pSrcVec, 2, destComponentByteStride, srcComponentByteStride);
		}

		template<class FloatTypeArg>
		FloatTypeArg *CopyAbsolutize3
		(
			FloatTypeArg *pDestVec, const FloatTypeArg *pSrcVec,
			uint8_t destComponentByteStride,
			uint8_t srcComponentByteStride
		)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			return CopyAbsolutize_n(pDestVec, pSrcVec, 3, destComponentByteStride, srcComponentByteStride);
		}
				
		template<class FloatTypeArg>
		int MinIndex_n
		(
			const FloatTypeArg *pVec,
			int numComponents,
			uint8_t componentByteStride
		)
		{
			assert(pVec != nullptr);
			int minIndex = 0;
			const FloatTypeArg *pMinElement = pVec[0];
			for(int i = 1; i < numComponents; i++)
			{
				const FloatTypeArg *pCurrElem = Util::AddPtr(pVec, i * componentByteStride);
				if(*pMinIndex > *pCurrElem)
				{
					minIndex = i;
					pMinElement = pCurrElem;
				}
			}
			return minIndex;
		}

		template<class FloatTypeArg>
		int MaximIndex_n
		(
			const FloatTypeArg *pVec,
			int numComponents,
			uint8_t componentByteStride
		)
		{
			assert(pVec != nullptr);
			int maximIndex = 0;
			const FloatTypeArg *pMaximElem = pVec[0];
			for(int i = 1; i < numComponents; i++)
			{
				const FloatTypeArg *pCurrElem = Util::AddPtr(pVec, i * componentByteStride);
				if(*pMaximElem < *pCurrElem)
				{
					maximIndex = i;
					pMaximElem = pCurrElem;
				}
			}
			return maximIndex;
		}

		template<class FloatTypeArg>
		int Min2Index
		(
			const FloatTypeArg *pVec,
			uint8_t componentByteStride
		)
		{
			return MinIndex_n(pVec, 2, componentByteStride);
		}

		template<class FloatTypeArg>
		int Maxim2Index
		(
			const FloatTypeArg *pVec,
			uint8_t componentByteStride
		)
		{
			return MaximIndex_n(pVec, 2, componentByteStride);
		}

		template<class FloatTypeArg>
		int Min3Index
		(
			const FloatTypeArg *pVec,
			uint8_t componentByteStride
		)
		{
			return MinIndex_n(pVec, 3, componentByteStride);
		}

		template<class FloatTypeArg>
		int Maxim3Index
		(
			const FloatTypeArg *pVec,
			uint8_t componentByteStride
		)
		{
			return MaximIndex_n(pVec, 3, componentByteStride);
		}

		template<class FloatTypeArg> 
		std::string VectorToDebugString
		(
			const FloatTypeArg *pVec, 
			int numComponents, 
			uint8_t componentByteStep,
			int precision_digs
		)
		{
			assert(pVec != nullptr);			
			std::string res_string;
			res_string.append("{");
			for(int i = 0; i < numComponents; i++)
			{
				typename const FloatTypeArg *pComponent = Util::AddPtr(pVec, componentByteStep * i);
				res_string.append("[");
				res_string.append(std::to_string(i));
				res_string.append("]=");
				res_string.append(std::to_string(*pComponent));
				res_string.append(";");
			}
			res_string.append("}");
			return res_string;
		}

		template<class FloatTypeArg> 
		std::string VectorToString
		(
			const FloatTypeArg *pVec, 
			int numComponents, 
			uint8_t componentByteStep,
			int precision_digs
		)
		{
			assert(pVec != nullptr);
			return VectorToDebugString(pVec, numComponents, componentByteStep, precision_digs);
		}

		template<class FloatTypeArg> 
		FloatTypeArg *Set_all_n
		(
			FloatTypeArg *pVec,
			FloatTypeArg componentValue, 
			int numComponents,
			uint8_t componentByteStride
		)
		{
			assert(pVec != nullptr);
			for(int i = 0; i < numComponents; i++)
			{
				typename FloatTypeArg *pComponent = Util::AddPtr(pVec, componentByteStride * i);
				*pComponent = componentValue;
			}
			return pVec;
		}

		template<class DestFloatTypeArg>
		DestFloatTypeArg *MkVecPartZero
		(
			DestFloatTypeArg				*pDestVector,
			size_t							 InSize,
			size_t							 InStartIndex,
			size_t							 InDestComponentStride
		)
		{
			assert(pDestVector != nullptr);
			assert(InSize > 0);
			assert(InStartIndex <= InSize);
			assert(InStartIndex > 0);
			DestFloatTypeArg		*pPartToZeroStart		= Util::AddPtr(pDestVector, InDestComponentStride * InStartIndex);
			size_t					 num_components			= InSize - InStartIndex;
			Util::ZeroStrided(pPartToZeroStart, num_components, InDestComponentStride);
			return pDestVector;
		}
		
		template<class FloatTypeArg> 
		FloatTypeArg *MkZero2(FloatTypeArg *pVec)
		{
			return Set_all_n(pVec, 0, 2);
		}

		// Zero 3d vector
		template<class FloatTypeArg> 
		FloatTypeArg *MkZero3(FloatTypeArg *pVec)
		{
			return Set_all_n(pVec, 0, 3);
		}

		template<class FloatTypeArg> 
		FloatTypeArg *MkZero4(FloatTypeArg *pVec)
		{
			return Set_all_n(pVec, 0, 4);
		}
		
		template<class FloatTypeArg>
		FloatTypeArg *MkComponent_n
		(
			FloatTypeArg *pVec, 
			FloatTypeArg componentValue, int valueComponentIndex, 
			int numComponents,
			uint8_t componentByteStride
		)
		{
			assert(pVec != nullptr);
			assert(valueComponentIndex < numComponents);
			Set_all_n(pVec, componentValue, numComponents, componentByteStride);
			*Util::AddPtr(pVec, componentByteStride * valueComponentIndex) = componentValue;
			return pVec;
		}

		template<class DestFloatTypeArg>
		DestFloatTypeArg *SetIdentityVectorPart
		(
			DestFloatTypeArg		*pDest,
			size_t					 InSize,				// size of the vector
			size_t					 InStartIndex,			// start index 
			size_t					 InOneComponentIndex,	// index of the component that have value of one
			size_t					 InDestComponentStride
		)
		{
			assert(pDest != nullptr);
			assert(InSize > 0);
			assert(InStartIndex >= 0);
			assert(InStartIndex <= InSize);
			assert(InOneComponentIndex >= 0);
			assert(InOneComponentIndex < InSize);			
			MkVecPartZero
			(
				pDest,
				InSize,
				InStartIndex,
				InDestComponentStride
			);
			if(InOneComponentIndex >= InStartIndex)
			{
				Util::SetStridedComponent
				(
					pDest,										// destination vector
					InOneComponentIndex,						// index of component to set
					static_cast<DestFloatTypeArg>(1),			// value to set component to
					InDestComponentStride
				);
			}
			return pDest;
		}

		template<class FloatTypeArg>
		FloatTypeArg SqrDist_n
		(
			const FloatTypeArg *pVec1,
			const FloatTypeArg *pVec2,
			int numComponents,
			uint8_t vec1ComponentByteStride,
			uint8_t vec2ComponentByteStride
		)
		{
			assert(pVec1 != nullptr);
			assert(pVec2 != nullptr);
			typename FloatTypeArg res_dist = 0;
			for(int i = 0; i < numComponents; i++)
			{
				typename const FloatTypeArg *pFirstComponent = Util::AddPtr(pVec1, i * vec1ComponentByteStride);
				typename const FloatTypeArg *pSecondComponent = Util::AddPtr(pVec2, i * vec2ComponentByteStride);
				typename const FloatTypeArg coord_delta = *pSecondComponent - *pFirstComponent;
				res_dist += coord_delta * coord_delta;
			}
			return res_dist;
		}		

		template<class FloatTypeArg>
		void Transform_XYZ_to_XZY
		(
			FloatTypeArg				*pOutX, 
			FloatTypeArg				*pOutY, 
			FloatTypeArg				*pOutZ
		)
		{
			assert(pOutX != nullptr);
			assert(pOutY != nullptr);
			assert(pOutZ != nullptr);
			typename FloatTypeArg new_Y = *pOutZ;
			typename FloatTypeArg new_Z = *pOutY;
			*pOutY = new_Y;
			*pOutZ = new_Z;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Transform_XYZ_to_XZY
		(
			FloatTypeArg				*pOut,
			size_t						 DestStrideInBytes
		)
		{
			assert(pOut != nullptr);
			typename FloatTypeArg *pOutX = pOut;
			typename FloatTypeArg *pOutY = Util::AddPtr(pOut, DestStrideInBytes);
			typename FloatTypeArg *pOutZ = Util::AddPtr(pOut, DestStrideInBytes * 2);
			Transform_XYZ_to_XZY(pOutX, pOutY, pOutZ);
			return pOut;
		}

		// Spherical space calculation
		// z = vertical axis
		// x,y = horz axis
		template<class FloatTypeArg>
		FloatTypeArg *CalcRadiusVector3D
		(
			FloatTypeArg			*pOutRadiusVector3D,
			FloatTypeArg			 angleInRadians_horz, 			
			FloatTypeArg			 angleInRadians_vertical, 			
			size_t					 DestStrideInBytes
		)
		{
			assert(pOutRadiusVector3D != nullptr);
			typename FloatTypeArg *pDestX = pOutRadiusVector3D;
			typename FloatTypeArg *pDestY = Util::AddPtr(pOutRadiusVector3D, DestStrideInBytes);
			typename FloatTypeArg *pDestZ = Util::AddPtr(pOutRadiusVector3D, 2 * DestStrideInBytes);

			CalcRadiusVector3D(pDestX, pDestY, pDestZ, angleInRadians_horz, angleInRadians_vertical);

			return pOutRadiusVector3D;
		}

		template<class FloatTypeArg>
		void CalcRadiusVector3D
		(
			FloatTypeArg			*pOutX, 
			FloatTypeArg			*pOutY,
			FloatTypeArg			*pOutZ,
			FloatTypeArg			 angleInRadians_horz, 			
			FloatTypeArg			 angleInRadians_vertical			
		)
		{
			typename FloatTypeArg cos_vertical_angle = cos(angleInRadians_vertical);
			typename FloatTypeArg sin_vertical_angle = sin(angleInRadians_vertical);
			typename FloatTypeArg cos_horz_angle = cos(angleInRadians_horz);
			typename FloatTypeArg sin_horz_angle = sin(angleInRadians_horz);
			typename FloatTypeArg dest_X = (FloatTypeArg)(cos_vertical_angle * cos_horz_angle);
			typename FloatTypeArg dest_Y = (FloatTypeArg)(sin_vertical_angle * cos_horz_angle);
			typename FloatTypeArg dest_Z = (FloatTypeArg)(sin_horz_angle);
			*pOutX = dest_X;
			*pOutY = dest_Y;
			*pOutZ = dest_Z;
		}
		
		// x,y = plane
		template<class FloatTypeArg>
		FloatTypeArg *CalcRadiusVector2D
		(
			FloatTypeArg			*pOutRadiusVector2D,
			FloatTypeArg			 angleInRadians, 			
			size_t					 DestStrideInBytes
		)
		{
			assert(pOutRadiusVector2D != nullptr);			
			FloatTypeArg *pDestX = pOutRadiusVector2D;
			FloatTypeArg *pDestY = Util::AddPtr(pOutRadiusVector2D, DestStrideInBytes);

			CalcRadiusVector2D(pDestX, pDestY, angleInRadians);

			return pOutRadiusVector2D;
		}

		template<class FloatTypeArg>
		void CalcRadiusVector2D
		(
			FloatTypeArg			*pOutX,
			FloatTypeArg			*pOutY,
			FloatTypeArg			 angleInRadians			
		)
		{
			assert(pOutX != nullptr);
			assert(pOutY != nullptr);
			typename FloatTypeArg dest_X = (FloatTypeArg)cos(angleInRadians);
			typename FloatTypeArg dest_Y = (FloatTypeArg)sin(angleInRadians);
			*pOutX = destX;
			*pOutY = destY;
		}

		/*********************************************************************************
		* Quaternions operations impl
		*********************************************************************************/
		template<class FloatTypeArg>
		FloatTypeArg *SetQuaternion_FromAxisAngle
		(
			FloatTypeArg			*pDestQuat,
			const FloatTypeArg		*pAxis,
			const FloatTypeArg		*pAngle,
			size_t					 DestStrideInBytes,
			size_t					 AxisStridInBytes			
		)
		{
			assert(pAxis != nullptr);
			const FloatTypeArg *pAxisX = Util::AddPtr(pAxis, 0);
			const FloatTypeArg *pAxisY = Util::AddPtr(pAxis, AxisStridInBytes);
			const FloatTypeArg *pAxisZ = Util::AddPtr(pAxis, 2* AxisStridInBytes);
			return SetQuaternion_FromAxisAngle(pDestQuat, pAxisX, pAxisY, pAxisZ, pAngle, DestStrideInBytes);
		}

		template<class FloatTypeArg>
		FloatTypeArg *SetQuaternion_FromAxisAngle
		(
			FloatTypeArg			*pDestQuat,
			const FloatTypeArg		*pAxisX,
			const FloatTypeArg		*pAxisY,
			const FloatTypeArg		*pAxisZ,
			const FloatTypeArg		*pAngle,
			size_t					 DestStrideInBytes
		)
		{
			assert(pDestQuat != nullptr);
			assert(pAxisX != nullptr);
			assert(pAxisY != nullptr);
			assert(pAxisZ != nullptr);
			assert(pAngle != nullptr);

			FloatTypeArg half_angle = *pAngle / 2.0F;
			FloatTypeArg sin_half_angle = static_cast<FloatTypeArg>(sin(half_angle));
			FloatTypeArg cos_half_angle = static_cast<FloatTypeArg>(cos(half_angle));
			Set3(pDestQuat, pAxisX, pAxisY, pAxisZ, DestStrideInBytes);
			MultScalar_n(pDestQuat, sin_half_angle, 3, DestStrideInBytes);
			FloatTypeArg *pQuadW = Util::AddPtr(pDestQuat, DestStrideInBytes * 3);
			*pQuatW = cos_half_angle;
			return pDestQuat;
		}

		template<class FloatTypeArg>
		FloatTypeArg *SetQuaternion_AsIdentity
		(
			FloatTypeArg			*pDestQuat,
			size_t					 DestStrideInBytes
		)
		{
			assert(pDestQuat != nullptr);
			Set4(pDestQuat, 0.0F, 0.0F, 0.0F, 1.0F, DestStrideInBytes);
			return pDestQuat;
		}

		template<class FloatTypeArg>
		bool Quaternion_AreStrictEqual
		(
			const FloatTypeArg			*pQuat,
			const FloatTypeArg			*pQuatTwo,			
			size_t						 quatStrideInBytes,
			size_t						 quatTwoStrideInBytes
		)
		{
			FloatTypeArg pQuatTwo_X = Util::AddPtr(pQuatTwo, 0);
			FloatTypeArg pQuatTwo_Y = Util::AddPtr(pQuatTwo, quatTwoStrideInBytes);
			FloatTypeArg pQuatTwo_Z = Util::AddPtr(pQuatTwo, 2 * quatTwoStrideInBytes);
			FloatTypeArg pQuatTwo_W = Util::AddPtr(pQuatTwo, 3 * quatTwoStrideInBytes);
			return Quaternion_AreStrictEqual
			(
				pQuat,
				pQuatTwo_X,
				pQuatTwo_Y,
				pQuatTwo_Z,
				pQuatTwo_W,
				quatTwoStrideInBytes
			);
		}

		template<class FloatTypeArg>
		bool Quaternion_AreStrictEqual
		(
			const FloatTypeArg			*pQuat,			
			const FloatTypeArg			*pQuatTwo_W,
			const FloatTypeArg			*pQuatTwo_X,
			const FloatTypeArg			*pQuatTwo_Y,
			const FloatTypeArg			*pQuatTwo_Z,			
			size_t						 quatStrideInBytes		
		)
		{
			FloatTypeArg pQuat_X = Util::AddPtr(pQuat, 0);
			FloatTypeArg pQuat_Y = Util::AddPtr(pQuat, quatStrideInBytes);
			FloatTypeArg pQuat_Z = Util::AddPtr(pQuat, 2 * quatStrideInBytes);
			FloatTypeArg pQuat_W = Util::AddPtr(pQuat, 3 * quatStrideInBytes);
			return Quaternion_AreStrictEqual
			(
				pQuat_X, pQuat_Y, pQuat_Z, pQuat_W,
				pQuatTwo_X, pQuatTwo_Y, pQuatTwo_Z, pQuatTwo_W,
				quatStrideInBytes
			);
		}

		template<class FloatTypeArg>
		bool Quaternion_AreStrictEqual
		(
			const FloatTypeArg			*pQuat_W,
			const FloatTypeArg			*pQuat_X,
			const FloatTypeArg			*pQuat_Y,
			const FloatTypeArg			*pQuat_Z,
			const FloatTypeArg			*pQuatTwo_W,
			const FloatTypeArg			*pQuatTwo_X,
			const FloatTypeArg			*pQuatTwo_Y,
			const FloatTypeArg			*pQuatTwo_Z			
		)
		{			
			if(*pQuat_X != pQuatTwo_X)
			{
				return false;
			}
			if(*pQuat_Y != pQuatTwo_Y)
			{
				return false;
			}
			if(*pQuat_Z != pQuatTwo_Z)
			{
				return false;
			}
			if(*pQuat_W != pQuatTwo_W)
			{
				return false;
			}
			return true;
		}

		template<class FloatTypeArg>
		FloatTypeArg Quaternion_CalcMagnitude
		(
			const FloatTypeArg			*pQuat,
			size_t						 DestStrideInBytes
		)
		{
			assert(pQuat != nullptr);
			FloatTypeArg res_sqr_mag = VecLength_n(pQuad, 4, DestStrideInBytes);
			FloatTypeArg res_mag = sqrt(res_sqr_mag);
			return res_mag;
		}

		template<class FloatTypeArg>
		FloatTypeArg Quaternion_CalcSquaredMagnitude
		(
			const FloatTypeArg			*pQuat,
			size_t						 DestStrideInBytes
		)
		{
			assert(pQuat != nullptr);
			FloatTypeArg res_mag = VecLength_n(pQuad, 4, DestStrideInBytes);
			return res_mag;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Quaterion_Conjugate
		(
			FloatTypeArg				*pQuat,
			size_t						 DestStrideInBytes
		)
		{
			return Negate_n(pQuat, 3, DestStrideInBytes);
		}

		template<class FloatTypeArg>
		FloatTypeArg *Quaternion_NegateAngle
		(
			FloatTypeArg				*pQuat,
			size_t						 DestStrideInBytes
		)
		{
			assert(pQuat != nullptr);
			FloatTypeArg *pW = Util::AddPtr(pQuad, 3 * DestStrideInBytes);
			*pW = -*pW;
			return pQuad;
		}

		template<class FloatTypeArg>
		void Quaternion_To_Matrix33
		(
			FloatTypeArg			*pDestMatrix,
			const FloatTypeArg		*pQuat,
			size_t					 destColumnStrideInBytes,
			size_t					 destRowStrideInBytes,
			size_t					 quatStrideInBytes
		)
		{
			FloatTypeArg *pRowOne = Util::AddPtr(pDestMatrix, destRowStrideInBytes * 0);
			FloatTypeArg *pRowTwo = Util::AddPtr(pDestMatrix, destRowStrideInBytes * 1);
			FloatTypeArg *pRowThree = Util::AddPtr(pDestMatrix, destRowStrideInBytes * 2);
			Quaternion_To_Matrix33(pRowOne, pRowTwo, pRowThree, pQuat, destColumnStrideInBytes, destRowStrideInBytes);
		}

		template<class FloatTypeArg>
		void Quaternion_To_Matrix33
		(
			FloatTypeArg			*pDestMatrix_RowOne,
			FloatTypeArg			*pDestMatrix_RowTwo,
			FloatTypeArg			*pDestMatrix_RowThree,
			const FloatTypeArg		*pQuat,
			size_t					 destColumnStrideInBytes,			
			size_t					 quatStrideInBytes
		)
		{
			assert(pDestMatrix_RowOne != nullptr);
			assert(pDestMatrix_RowTwo != nullptr);
			assert(pDestMatrix_RowThree != nullptr);
			assert(pQuat != nullptr);

			FloatTypeArg		VecX						= *Util::AddPtr(pQuat, 0);
			FloatTypeArg		VecY						= *Util::AddPtr(pQuat, quatStrideInBytes);
			FloatTypeArg		VecZ						= *Util::AddPtr(pQuat, 2 * quatStrideInBytes);
			FloatTypeArg		W							= *Util::AddPtr(pQuat, 3 * quatStrideInBytes);

			FloatTypeArg		Doubled_VecX				= 2.0F * VecX;
			FloatTypeArg		Doubled_VecY				= 2.0F * VecY;
			FloatTypeArg		Doubled_VecZ				= 2.0F * VecZ;

			FloatTypeArg		Doubled_Vec_X_X				= Doubled_VecX * VecX;
			FloatTypeArg		Doubled_Vec_Y_Y				= Doubled_VecY * VecY;
			FloatTypeArg		Doubled_Vec_Z_Z				= Doubled_VecZ * VecZ;

			FloatTypeArg		Doubled_Vec_X_Y				= Doubled_VecX * VecY;
			FloatTypeArg		Doubled_Vec_X_Z				= Doubled_VecX * VecZ;
			FloatTypeArg		Doubled_Vec_Y_Z				= Doubled_VecY * VecZ;

			FloatTypeArg		Doubled_VecX_W				= Doubled_VecX * W;
			FloatTypeArg		Doubled_VecY_W				= Doubled_VecY * W;
			FloatTypeArg		Doubled_VecZ_W				= Doubled_VecZ * W;
			
			FloatTypeArg		One_Minus_Doubled_VecX_X	= 1.0F - Doubled_Vec_X_X;
			FloatTypeArg		One_Minus_Doubled_VecY_Y	= 1.0F - Doubled_Vec_Y_Y;

			// Row X:
			FloatTypeArg		row_X_column_X = One_Minus_Doubled_VecY_Y - Doubled_Vec_Z_Z;
			FloatTypeArg		row_X_column_Y = Doubled_Vec_X_Y - Doubled_VecZ_W;
			FloatTypeArg		row_X_column_Z = Doubled_Vec_X_Z + Doubled_VecY_W;

			// Row Y:
			FloatTypeArg		row_Y_column_X = Doubled_Vec_X_Y + Doubled_VecZ_W;
			FloatTypeArg		row_Y_column_Y = One_Minus_Doubled_VecX_X - Doubled_Vec_Z_Z;
			FloatTypeArg		row_Y_column_Z = Doubled_Vec_Y_Z - Doubled_VecX_W;

			// Row Z:
			FloatTypeArg		row_Z_column_X = Doubled_Vec_X_Z - Doubled_VecY_W;
			FloatTypeArg		row_Z_column_Y = Doubled_Vec_Y_Z + Doubled_VecX_W;
			FloatTypeArg		row_Z_column_Z = One_Minus_Doubled_VecX_X - Doubled_Vec_Y_Y;

			// Setting the matrix
			Set3(pDestMatrix_RowOne, row_X_column_X, row_X_column_Y, row_X_column_Z, destColumnStrideInBytes);
			Set3(pDestMatrix_RowTwo, row_Y_column_X, row_Y_column_Y, row_Y_column_Z, destColumnStrideInBytes);
			Set3(pDestMatrix_RowThree, row_Z_column_X, row_Z_column_Y, row_Z_column_Z, destColumnStrideInBytes);
		}

		template<class FloatTypeArg>
		void Quatenion_IntertialToObject_To_Euler
		(
			FloatTypeArg			*pHeading,
			FloatTypeArg			*pPitch,
			FloatTypeArg			*pBank,			
			const FloatTypeArg		*pQuat,			
			size_t					 quatStrideInBytes
		)
		{
			// This is the minimum value where the gimbal lock may occure
			const FloatTypeArg MIN_SIN_PITCH_FOR_GIMBAL_LOCK = 0.9999F;

			assert(pHeading != nullptr);
			assert(pPitch != nullptr);
			assert(pBank != nullptr);
			
			FloatTypeArg X = *Utils::AddPtr(pQuad, 0);						// quaternion vector x
			FloatTypeArg Y = *Util::AddPtr(pQuat, quatStrideInBytes);		// quaternion vector Y
			FloatTypeArg Z = *Utils::AddPtr(pQuad, 2 * quatStrideInBytes);	// quaternion vector Z
			FloatTypeArg W = *Utils::AddPtr(pQuad, 3 * quatStrideInBytes);	// quaternion W

			FloatTypeArg heading					= 0;
			FloatTypeArg pitch						= 0;
			FloatTypeArg bank						= 0;

			/*******************************************************************************
			* Pitch calculation
			*******************************************************************************/
			FloatTypeArg SinPitch					= -2.0 * (Y*Z + W*X);

			// Check whether the gimbal lock occured
			if(MIN_SIN_PITCH_FOR_GIMBAL_LOCK < SinPitch || 
			   SinPitch < -MIN_SIN_PITCH_FOR_GIMBAL_LOCK)
			{
				pitch = M_PI/2 * SinPitch;

				// This formula is used only in the case of the gimbal lock		
				FloatTypeArg TanHeading_Numerator_GimbalLock			= - X * Z - W * Y;
				FloatTypeArg TanHeading_Denominator_GimbalLock			= static_cast<FloatTypeArg>(1.0F/2.0F) - Y * Y - Z * Z;

				heading		= atan2(TanHeading_Numerator_GimbalLock, TanHeading_Denominator_GimbalLock);
				bank		= 0.0F;
			}
			else
			{
				// no gimbal lock occured
				// This formulas are used only if the cosinus of pitch is not zero
				// (so the pitch angle is not +-90, gimbal lock occurs)		

				pitch = asin(SinPitch);	

				FloatTypeArg TanHeading_Numerator_CosPitchNotZero		= X * Z - W * Y;
				FloatTypeArg TanHeading_Denominator_CosPitchNotZero		= static_cast<FloatTypeArg>(1.0F/2.0F) - X * X - Y * Y;
				heading = atan2(TanHeading_Numerator_CosPitchNotZero, TanHeading_Denominator_CosPitchNotZero);

				FloatTypeArg TanBank_Numerator_CosPitchNotZero = X * Y - W * Z;			
				FloatTypeArg TanBank_Denorator_CosPitchNotZero = static_cast<FloatTypeArg>(1.0F/2.0F) - X * X - Z * Z;
				bank = atan2(TanBank_Denorator_CosPitchNotZero, TanBank_Denorator_CosPitchNotZero);
			}

			*pHeading = heading;
			*pPitch = pitch;
			*pBank = bank;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Euler_HeadingPitchBank_To_Matrix33_InertialToObjectRotation
		(
			FloatTypeArg			*pMatrixRowOne,
			FloatTypeArg			*pMatrixRowTwo,
			FloatTypeArg			*pMatrixRowThree,
			const FloatTypeArg		*pHeading,
			const FloatTypeArg		*pPitch,
			const FloatTypeArg		*pBank,
			size_t					 destColumnStrideInBytes			
		)
		{
			assert(pMatrixRowOne != nullptr);
			assert(pMatrixRowTwo != nullptr);
			assert(pMatrixRowThree != nullptr);
			assert(pHeading != nullptr);
			assert(pPitch != nullptr);
			assert(pBank != nullptr);
			FloatTypeArg CosHeading		= static_cast<FloatTypeArg>(cos(*pHeading));
			FloatTypeArg SinHeading		= static_cast<FloatTypeArg>(sin(*pHeading));
			FloatTypeArg CosPitch		= static_cast<FloatTypeArg>(cos(*pPitch));
			FloatTypeArg SinPitch		= static_cast<FloatTypeArg>(sin(*pPitch));
			FloatTypeArg CosBank		= static_cast<FloatTypeArg>(cos(*pBank));
			FloatTypeArg SinBank		= static_cast<FloatTypeArg>(sin(*pBank));

			// Shared fragments
			FloatTypeArg CosHeading_Mult_CosBank = static_cast<FloatTypeArg>(CosHeading * CosBank);
			FloatTypeArg SinHeading_Mult_SinPitch = static_cast<FloatTypeArg>(SinHeading * SinPitch);
			FloatTypeArg CosHeading_Mult_SinBank = static_cast<FloatTypeArg>(CosHeading * SinBank);
			// Unical fragments
			FloatTypeArg SinHeading_Mult_CosPitch = static_cast<FloatTypeArg>(SinHeading * CosPitch);
			FloatTypeArg SinBank_Mult_CosPitch = static_cast<FloatTypeArg>(SinBank * CosPitch);
			FloatTypeArg CosBank_Mult_CosPitch = static_cast<FloatTypeArg>(CosBank * CosPitch);
			FloatTypeArg SinHeading_Mult_CosBank = static_cast<FloatTypeArg>(SinHeading * CosBank);
			FloatTypeArg SinBank_Mult_SinHeading = static_cast<FloatTypeArg>(SinHeading * SinBank);
			FloatTypeArg CosHeading_Mult_CosPitch = static_cast<FloatTypeArg>(CosHeading * CosPitch);
			// Matrix elements
			// Control:
			// 1) Total of three minuses are used at entire matrix
			// Row X:
			FloatTypeArg		row_X_column_X = CosHeading_Mult_CosBank + SinHeading_Mult_SinPitch * SinBank;
			FloatTypeArg		row_X_column_Y = -CosHeading_Mult_SinBank + SinHeading_Mult_SinPitch * CosBank; // first minus
			FloatTypeArg		row_X_column_Z = SinHeading_Mult_CosPitch;

			// Row Y:
			FloatTypeArg		row_Y_column_X = SinBank_Mult_CosPitch;
			FloatTypeArg		row_Y_column_Y = CosBank_Mult_CosPitch;
			FloatTypeArg		row_Y_column_Z = -SinPitch; // second minus

			// Row Z:
			FloatTypeArg		row_Z_column_X = -SinHeading_Mult_CosBank + CosHeading_Mult_SinBank * SinBank; // third minus
			FloatTypeArg		row_Z_column_Y = SinBank_Mult_SinHeading + CosHeading_Mult_CosBank * SinPitch;
			FloatTypeArg		row_Z_column_Z =  CosHeading_Mult_CosPitch;

			// Building the matrix
			Set3(pMatrixRowOne, row_X_column_X, row_X_column_Y, row_X_column_Z, destColumnStrideInBytes);
			Set3(pMatrixRowTwo, row_Y_column_X, row_Y_column_Y, row_Y_column_Z, destColumnStrideInBytes);
			Set3(pMatrixRowThree, row_Z_column_X, row_Z_column_Y, row_Z_column_Z, destColumnStrideInBytes);
			return pMatrixRowOne;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Euler_HeadingPitchBank_To_Matrix33_InertialToObjectRotation
		(
			FloatTypeArg			*pMatrix33,
			const FloatTypeArg		*pHeading,
			const FloatTypeArg		*pPitch,
			const FloatTypeArg		*pBank,
			size_t					 destColumnStrideInBytes,
			size_t					 destRowStrideInBytes
		)
		{
			assert(pMatrix33 != nullptr);
			FloatTypeArg pMatrixRowFirst = Util::AddPtr(pMatrix,0);
			FloatTypeArg pMatrixRowTwo = Util::AddPtr(pMatrix,destRowStrideInBytes);
			FloatTypeArg pMatrixRowThird = Util::AddPtr(pMatrix,2 * destRowStrideInBytes);
			return Euler_HeadingPitchBank_To_Matrix33_InertialToObjectRotation
			(
				pMatrixRowFirst,
				pMatrixRowTwo,
				pMatrixRowThird,
				pHeading,
				pPitch,
				pBank,
				destColumnStrideInBytes			
			)
			return pMatrix33;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Matrix33_InertialToObjectRotation_To_Euler_HeadingPitchBank
		(
			FloatTypeArg				*pHeading,
			FloatTypeArg				*pPitch,
			FloatTypeArg				*pBank,
			const FloatTypeArg			*pMatrix33,
			size_t						 destColumnStrideInBytes,
			size_t						 destRowStrideInBytes				
		)
		{
			const FloatTypeArg *pMatrixRowOne			= Util::AddPtr(pMatrix33, 0);
			const FloatTypeArg *pMatrixRowTwo			= Util::AddPtr(pMatrix33, destRowStrideInBytes);
			const FloatTypeArg *pMatrixRowThree			= Util::AddPtr(pMatrix33, 2 * destRowStrideInBytes);
			return Matrix33_InertialToObjectRotation_To_Euler_HeadingPitchBank
			(
				pHeading,
				pPitch,
				pBank,
				pMatrixRowOne,
				pMatrixRowTwo,
				pMatrixRowThree,
				destRowStridInBytes
			);
		}

		template<class FloatTypeArg>
		FloatTypeArg *Matrix33_InertialToObjectRotation_To_Euler_HeadingPitchBank
		(
			FloatTypeArg				*pHeading,
			FloatTypeArg				*pPitch,
			FloatTypeArg				*pBank,
			const FloatTypeArg			*pMatrixRowOne,
			const FloatTypeArg			*pMatrixRowTwo,
			const FloatTypeArg			*pMatrixRowThree,
			size_t						 destColumnStrideInBytes		
		)
		{
			// This is the minimum value where the gimbal lock may occure
			const FloatTypeArg MIN_SIN_PITCH_FOR_GIMBAL_LOCK = 0.9999F;

			assert(pHeading != nullptr);
			assert(pPitch != nullptr);
			assert(pBank != nullptr);
			assert(pMatrixRowOne != nullptr);
			assert(pMatrixRowTwo != nullptr);
			assert(pMatrixRowThree != nullptr);

			// Row X:
			FloatTypeArg		row_X_column_X = *Util::AddPtr(pMatrixRowOne, 0);
			FloatTypeArg		row_X_column_Y = *Util::AddPtr(pMatrixRowOne, destColumnStrideInBytes); 
			FloatTypeArg		row_X_column_Z = *Util::AddPtr(pMatrixRowOne, 2 * destColumnStrideInBytes);

			// Row Y:
			FloatTypeArg		row_Y_column_X = *Util::AddPtr(pMatrixRowTwo, 0);
			FloatTypeArg		row_Y_column_Y = *Util::AddPtr(pMatrixRowTwo, destColumnStrideInBytes); 
			FloatTypeArg		row_Y_column_Z = *Util::AddPtr(pMatrixRowTwo, 2 * destColumnStrideInBytes); 

			// Row Z:
			FloatTypeArg		row_Z_column_X = *Util::AddPtr(pMatrixRowThree, 0); 
			FloatTypeArg		row_Z_column_Y = *Util::AddPtr(pMatrixRowThree, destColumnStrideInBytes);
			FloatTypeArg		row_Z_column_Z = *Util::AddPtr(pMatrixRowThree, 2 * destColumnStrideInBytes);

			// Result values
			FloatTypeArg heading			= 0;
			FloatTypeArg bank				= 0;
			FloatTypeArg pitch				= 0;

			// WARNING !!! The case where abs(SinPitch) > 1.0 may be confronted!
			// This is due to the imprecistion floating point arithmetic.
			FloatTypeArg		SinPitch = -row_Y_column_Y;

			/*********************************************************************************
			* Pitch calculation
			*
			* The pitch is always calculated
			*********************************************************************************/
			if(SinPitch < -1.0F)
			{
				pitch = -static_cast_<FloatTypeArg>(M_PI/2.0F);
			}
			else
			{
				if(SinPitch > 1.0F)
				{
					pitch = static_cast<FloatTypeArg>(M_PI/2.0F);
				}
				else
				{
					// Get the pitch within	-PI/2 to PI/2 (as necessary)		
					pitch = static_cast<FloatTypeArg>(asin(SinPitch));
				}
			}			

			/******************************************************************************************
			* Computing the heading and the bank
			******************************************************************************************/
			// Getting the values for computing the heading
			FloatTypeArg		SinHeading_Mult_CosPitch = row_X_column_Z;
			FloatTypeArg		CosHeading_Mult_CosPitch = row_Z_column_Z;

			// Getting the values for computing the bank:
			FloatTypeArg		SinBank_Mult_CosPitch = row_Y_column_X;
			FloatTypeArg		CosBank_Mult_CosPitch = row_Y_column_Y;
			

			// We can only compute the heading and bank if we have not the gimbal lock situation:
			if
			(
				SinPitch < MIN_SIN_PITCH_FOR_GIMBAL_LOCK &&
				-MIN_SIN_PITCH_FOR_GIMBAL_LOCK < SinPitch
			)
			{
				heading	= atan2(SinHeading_Mult_CosPitch, CosHeading_Mult_CosPitch);
				bank	= atan2(SinBank_Mult_CosPitch, CosBank_Mult_CosPitch);
			}
			else
			{
				// gimbal lock occured
				FloatTypeArg CosHeading =  row_X_column_X;
				FloatTypeArg SinHeading = -row_Z_column_X;
				heading					= atan2(SinHeading, CosHeading);
				bank					= 0; // bank is always zero in the gimbal lock state
			}

			// Output the values
			*pHeading			= heading;
			*pPitch				= pitch;
			*pBank				= bank;
			
			return pMatrixRowOne;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCastRow_TransposedToDestColumn
		(
			DestFloatTypeArg *pDestMatrix, const SrcFloatTypeArg *pSrcMatrix, 
			int InNumSrcColumns,
			int InSrcRowIndex, int InDestColumnIndex,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep, 			
			uint8_t srcComponentByteStep	
		)
		{
			const SrcFloatTypeArg *pSrcRow		= Util::AddPtr(pSrcMatrix, InSrcRowIndex * srcComponentRowByteStep);
			// Get the address of the dest column:
			// Note: the index src row is equal to the dest src column index
			DestFloatTypeArg *pDestColumnStart  = Util::AddPtr(pDestMatrix, InDestColumnIndex * destComponentByteStep);
			return CopyCast_n
			(
				pDestColumnStart, pSrcRow,
				InNumSrcColumns,
				destComponentRowByteStep, 
				srcComponentByteStep
			);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCastMatrix_Transposed
		(
			DestFloatTypeArg *pDestMatrix, const SrcFloatTypeArg *pSrcMatrix, 
			int InNumSrcRows, int InNumSrcColumns,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep, 			
			uint8_t srcComponentByteStep			
		)
		{
			assert(pDestMatrix != nullptr);
			assert(pSrcMatrix != nullptr);
			assert(InNumSrcRows > 0);
			assert(InNumSrcColumns > 0);
			for(int index_src_row = 0; index_src_row < InNumSrcRows; index_src_row++)
			{
				/*
				const SrcFloatTypeArg *pSrcRow		= Util::AddPtr(pSrcMatrix, index_src_row * srcComponentRowByteStep);
				// Get the address of the dest column:
				// Note: the index src row is equal to the dest src column index
				DestFloatTypeArg *pDestColumnStart  = Util::AddPtr(pDestMatrix, index_src_row * destComponentByteStep);
				CopyCast_n
				(
					pDestColumnStart, pSrcRow,
					InNumSrcColumns,
					destComponentRowByteStep, 
					srcComponentByteStep
				);
				*/
				CopyCastRow_TransposedToDestColumn
				(
					pDestMatrix, pSrcMatrix, 
					InNumSrcColumns, 
					index_src_row, index_src_row,
					destComponentRowByteStep, srcComponentRowByteStep,
					destComponentByteStep, srcComponentByteStep
				);
			}
			return pDestMatrix;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCastRow
		(
			DestFloatTypeArg *pDestMatrix, const SrcFloatTypeArg *pSrcMatrix, 
			int InNumColumns,
			int InSrcRowIndex, int InDestRowIndex,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep, 			
			uint8_t srcComponentByteStep
		)
		{
			assert(pDestMatrix != nullptr);
			assert(pSrcMatrix != nullptr);
			assert(InNumColumns > 0);
			const SrcFloatTypeArg *pSrcRowVec	= Util::AddPtr(pSrcMatrix, InSrcRowIndex * srcComponentRowByteStep);
			DestFloatTypeArg *pDestRowVec		= Util::AddPtr(pDestMatrix, InDestRowIndex * destComponentRowByteStep);
			return CopyCast_n
			(
				pDestRowVec, pSrcRowVec, 
				InNumColumns,
				destComponentByteStep, 
				srcComponentByteStep
			);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrixFromMatrix_n
		(
			DestFloatTypeArg					*pDestMatrix,
			const SrcFloatTypeArg				*pSrcMatrix,
			size_t								 InDestSize,
			size_t								 InSubmatrixSize,
			size_t								 InDestRowStride,
			size_t								 InSrcRowStride,
			size_t								 InDestComponentStride,
			size_t								 InSrcComponentStride
		)
		{
			assert(pDestMatrix != nullptr);
			assert(pSrcMatrix != nullptr);	
			assert(InDestSize > 0);
			assert(InSubmatrixSize > 0);
			assert(InSubmatrixSize <= InDestSize);
			SetIdentityMatrix(pDestMatrix, InDestSize, InDestRowStride, InDestComponentStride);
			CopyCastMatrix
			(
				pDestMatrix, pSrcMatrix, 
				InSubmatrixSize, InSubmatrixSize, 
				InDestRowStride, 
				InSrcRowStride, 
				InDestComponentStride, 
				InSrcComponentStride
			);
			return pDestMatrix;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *CopyCastMatrix
		(
			DestFloatTypeArg *pDestMatrix, const SrcFloatTypeArg *pSrcMatrix, 
			int InNumRows, int InNumColumns,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep, 			
			uint8_t srcComponentByteStep	
		)
		{
			assert(pDestMatrix != nullptr);
			assert(pSrcMatrix != nullptr);
			assert(InNumRows > 0);
			assert(InNumColumns > 0);
			for(int index_row = 0; index_row < InNumRows; index_row++)
			{
				/*
				const SrcFloatTypeArg *pSrcRowVec = Util::AddPtr(pSrcMatrix, index_row * srcComponentRowByteStep);
				DestFloatTypeArg *pDestRowVec = Util::AddPtr(pDestMatrix, index_row * destComponentRowByteStep);
				CopyCast_n
				(
					pDestRowVec, pSrcRowVec, 
					InNumColumns,
					destComponentByteStep, 
					srcComponentByteStep
				);
				*/
				CopyCastRow
				(
					pDestMatrix, pSrcMatrix, 
					InNumColumns, 
					index_row, index_row,
					destComponentRowByteStep, srcComponentRowByteStep, 
					destComponentByteStep, srcComponentByteStep
				);
			}
			return pDestMatrix;
		}

		template<class FloatTypeArg>		
		FloatTypeArg *SetMatrixElement
		(
			FloatTypeArg			*pDest, 
			int						 InRowIndex,
			int						 InColumnIndex,
			const FloatTypeArg&		 InValue,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes
		)
		{
			FloatTypeArg *pDestRow = Util::AddPtr(pDest, rowStrideInBytes * InRowIndex);
			FloatTypeArg *pElement = Util::AddPtr(pDestRow, strideInBytes * InColumnIndex);
			*pElement			   = InValue;
			return pElement;
		}

		template<class FloatTypeArg>			
		void SetMainDiagElements
		(
			FloatTypeArg			*pDest, 
			int						 InNumRows,
			const FloatTypeArg&		 InValue,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes
		)
		{
			for(int i = 0; i < InNumRows; i++)
			{
				SetMatrixElement(pDest, i, i, InValue, rowStrideInBytes, strideInBytes);
			}
		}

		template<class FloatTypeArg>			
		void SetIdentityMatrix
		(
			FloatTypeArg			*pDest, 
			int						 InNumRows,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes
		)
		{
			assert(pDest != nullptr);
			ZeroMatrix(pDest, InNumRows, InNumRows, rowStrideInBytes, strideInBytes);
			SetMainDiagElements(pDest, InNumRows, static_cast<FloatTypeArg>(1.0F), rowStrideInBytes, strideInBytes);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix2x2
		(
			DestFloatTypeArg			*pDest,
			size_t						InMatrixSize,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes
		)
		{
			assert(pDest != nullptr);
			assert(InMatrixSize >= 2);
			SetIdentityMatrix(pDest, InMatrixSize, rowStrideInBytes, strideInBytes);
			SetMatrix2x2
			(
				pDest, 
				In00, In01, 
				In10, In11, 
				rowStrideInBytes, 
				strideInBytes
			);
			return pDest;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix3x3
		(
			DestFloatTypeArg			*pDest,
			size_t						InMatrixSize,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01, SrcFloatTypeArg In02,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11, SrcFloatTypeArg In12,
			SrcFloatTypeArg	In20, SrcFloatTypeArg In21, SrcFloatTypeArg In22,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes
		)
		{
			assert(pDest != nullptr);
			assert(InMatrixSize >= 3);
			SetIdentityMatrix(pDest, InMatrixSize, rowStrideInBytes, strideInBytes);
			SetMatrix3x3
			(
				pDest, 
				In00, In01, In02, 
				In10, In11, In12, 
				In20, In21, In22, 
				rowStrideInBytes, 
				strideInBytes
			);
			return pDest;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix2x2_FromColumns
		(
			DestFloatTypeArg		   *pDest,
			size_t						InMatrixSize,
			const SrcFloatTypeArg	   *pColumn0,
			const SrcFloatTypeArg	   *pColumn1,			
			size_t						InDestRowStride,
			size_t						InDestComponentStride,
			size_t						InColumnOneComponentStride,
			size_t						InColumnTwoComponentStride		
		)
		{
			assert(pDest != nullptr);
			assert(InMatrixSize > 0);
			assert(pColumn0 != nullptr);
			assert(pColumn1 != nullptr);
			ResetMatrixColumn
			(
				pDest,
				InMatrixSize, // total number of columns
				2, // number of columns to set
				0, // column index
				pColumn0,
				InDestRowStride,
				InDestComponentStride,
				InColumnOneComponentStride				
			);
			ResetMatrixColumn
			(
				pDest,
				InMatrixSize, // total number of columns
				2, // number of columns to set
				1, // column index
				pColumn1,
				InDestRowStride,
				InDestComponentStride,
				InColumnTwoComponentStride				
			);
			return pDest;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix3x3_FromColumns
		(
			DestFloatTypeArg		   *pDest,
			size_t						InMatrixSize,
			const SrcFloatTypeArg	   *pColumn0,
			const SrcFloatTypeArg	   *pColumn1,
			const SrcFloatTypeArg	   *pColumn2,
			size_t						InDestRowStride,
			size_t						InDestComponentStride,
			size_t						InColumnOneComponentStride,
			size_t						InColumnTwoComponentStride,
			size_t						InColumnThreeComponentStride
		)
		{
			assert(pDest != nullptr);
			assert(InMatrixSize > 0);
			assert(pColumn0 != nullptr);
			assert(pColumn1 != nullptr);
			assert(pColumn2 != nullptr);	
			ResetMatrixColumn
			(
				pDest,
				InMatrixSize,			// size of the destination matrix to set
				3,						// number of rows of the column to set
				0,						// column index to set
				pColumn0,
				InDestRowStride,
				InDestComponentStride,
				InColumnOneComponentStride				
			);
			ResetMatrixColumn
			(
				pDest,
				InMatrixSize,			// size of the destination matrix to set
				3,						// number of rows of the column to set
				1,						// column index to set
				pColumn1,
				InDestRowStride,
				InDestComponentStride,
				InColumnTwoComponentStride				
			);
			ResetMatrixColumn
			(
				pDest,
				InMatrixSize,			// size of the destination matrix to set
				3,						// number of rows of the column to set
				2,						// column index to set
				pColumn2,
				InDestRowStride,
				InDestComponentStride,
				InColumnThreeComponentStride				
			);
			return pDest;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix4x4_FromColumns
		(
			DestFloatTypeArg		   *pDest,
			size_t						InMatrixSize,
			const SrcFloatTypeArg	   *pColumn0,
			const SrcFloatTypeArg	   *pColumn1,
			const SrcFloatTypeArg	   *pColumn2,
			const SrcFloatTypeArg	   *pColumn3,
			size_t						InDestRowStride,
			size_t						InDestComponentStride,
			size_t						InColumnOneComponentStride,
			size_t						InColumnTwoComponentStride,
			size_t						InColumnThreeComponentStride,
			size_t						InColumnFourComponentStride
		)
		{
			assert(pDest != nullptr);
			assert(InMatrixSize > 0);
			assert(pColumn0 != nullptr);
			assert(pColumn1 != nullptr);
			assert(pColumn2 != nullptr);
			assert(pColumn3 != nullptr);
			ResetMatrixColumn
			(
				pDest,
				InMatrixSize,			// size of the destination matrix to set
				4,						// number of rows of the column to set
				0,						// column index to set
				pColumn0,
				InDestRowStride,
				InDestComponentStride,
				InColumnOneComponentStride				
			);
			ResetMatrixColumn
			(
				pDest,
				InMatrixSize,			// size of the destination matrix to set
				4,						// number of rows of the column to set
				1,						// column index to set
				pColumn1,
				InDestRowStride,
				InDestComponentStride,
				InColumnTwoComponentStride				
			);
			ResetMatrixColumn
			(
				pDest,
				InMatrixSize,			// size of the destination matrix to set
				4,						// number of rows of the column to set
				2,						// column index to set
				pColumn2,
				InDestRowStride,
				InDestComponentStride,
				InColumnThreeComponentStride				
			);
			ResetMatrixColumn
			(
				pDest,
				InMatrixSize,			// size of the destination matrix to set
				4,						// number of rows of the column to set
				3,						// column index to set
				pColumn3,
				InDestRowStride,
				InDestComponentStride,
				InColumnFourComponentStride				
			);
			return pDest;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix2x2_FromRows
		(
			DestFloatTypeArg		   *pDest,
			size_t						InMatrixSize,
			const SrcFloatTypeArg	   *pRow0,
			const SrcFloatTypeArg	   *pRow1,			
			size_t						InDestRowStride,
			size_t						InDestComponentStride,
			size_t						InRowOneComponentStride,
			size_t						InRowTwoComponentStride		
		)
		{
			assert(pDest != nullptr);
			assert(InMatrixSize > 0);
			assert(pRow0 != nullptr);
			assert(pRow1 != nullptr);
			ResetMatrixRow
			(
				pDest,
				InMatrixSize,						// total number of components
				2,									// number of components to set
				0,									// row index
				pRow0,								// src row to copy from					
				InDestRowStride,
				InDestComponentStride,
				InRowOneComponentStride				
			);
			ResetMatrixRow
			(
				pDest,
				InMatrixSize,						// total number of components
				2,									// number of components to set
				1,									// row index
				pRow1,								// src row to copy from					
				InDestRowStride,
				InDestComponentStride,
				InRowTwoComponentStride		
			);
			return pDest;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix3x3_FromRows
		(
			DestFloatTypeArg		   *pDest,
			size_t						InMatrixSize,
			const SrcFloatTypeArg	   *pRow0,
			const SrcFloatTypeArg	   *pRow1,
			const SrcFloatTypeArg	   *pRow2,
			size_t						InDestRowStride,
			size_t						InDestComponentStride,
			size_t						InRowOneComponentStride,
			size_t						InRowTwoComponentStride,
			size_t						InRowThreeComponentStride
		)
		{
			assert(pDest != nullptr);
			assert(InMatrixSize >= 3);
			assert(pRow0 != nullptr);
			assert(pRow1 != nullptr);
			assert(pRow2 != nullptr);
			ResetMatrixRow
			(
				pDest,
				InMatrixSize,						// total number of components
				3,									// number of components to set
				0,									// row index
				pRow0,								// src row to copy from					
				InDestRowStride,
				InDestComponentStride,
				InRowOneComponentStride				
			);
			ResetMatrixRow
			(
				pDest,
				InMatrixSize,						// total number of components
				3,									// number of components to set
				1,									// row index
				pRow1,								// src row to copy from					
				InDestRowStride,
				InDestComponentStride,
				InRowTwoComponentStride		
			);
			ResetMatrixRow
			(
				pDest,
				InMatrixSize,						// total number of components
				3,									// number of components to set
				2,									// row index
				pRow2,								// src row to copy from					
				InDestRowStride,
				InDestComponentStride,
				InRowThreeComponentStride		
			);
			return pDest;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix4x4_FromRows
		(
			DestFloatTypeArg		   *pDest,
			size_t						InMatrixSize,
			const SrcFloatTypeArg	   *pRow0,
			const SrcFloatTypeArg	   *pRow1,
			const SrcFloatTypeArg	   *pRow2,
			const SrcFloatTypeArg	   *pRow3,
			size_t						InDestRowStride,
			size_t						InDestComponentStride,
			size_t						InRowOneComponentStride,
			size_t						InRowTwoComponentStride,
			size_t						InRowThreeComponentStride,
			size_t						InRowFourComponentStride
		)
		{
			assert(pDest != nullptr);
			assert(InMatrixSize > 0);
			assert(pRow0 != nullptr);
			assert(pRow1 != nullptr);
			assert(pRow2 != nullptr);
			assert(pRow3 != nullptr);
			ResetMatrixRow
			(
				pDest,
				InMatrixSize,						// total number of components
				4,									// number of components to set
				0,									// row index
				pRow0,								// src row to copy from					
				InDestRowStride,
				InDestComponentStride,
				InRowOneComponentStride				
			);
			ResetMatrixRow
			(
				pDest,
				InMatrixSize,						// total number of components
				4,									// number of components to set
				1,									// row index
				pRow1,								// src row to copy from					
				InDestRowStride,
				InDestComponentStride,
				InRowTwoComponentStride		
			);
			ResetMatrixRow
			(
				pDest,
				InMatrixSize,						// total number of components
				4,									// number of components to set
				2,									// row index
				pRow2,								// src row to copy from					
				InDestRowStride,
				InDestComponentStride,
				InRowThreeComponentStride		
			);
			ResetMatrixRow
			(
				pDest,
				InMatrixSize,						// total number of components
				4,									// number of components to set
				3,									// row index
				pRow3,								// src row to copy from					
				InDestRowStride,
				InDestComponentStride,
				InRowFourComponentStride		
			);
			return pDest;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrix4x4
		(
			DestFloatTypeArg			*pDest,
			size_t						InMatrixSize,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01, SrcFloatTypeArg In02, SrcFloatTypeArg In03,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11, SrcFloatTypeArg In12, SrcFloatTypeArg In13,
			SrcFloatTypeArg	In20, SrcFloatTypeArg In21, SrcFloatTypeArg In22, SrcFloatTypeArg In23,
			SrcFloatTypeArg	In30, SrcFloatTypeArg In31, SrcFloatTypeArg In32, SrcFloatTypeArg In33,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes
		)
		{
			assert(pDest != nullptr);
			assert(InMatrixSize >= 4);
			SetIdentityMatrix(pDest, InMatrixSize, rowStrideInBytes, strideInBytes);
			SetMatrix4x4
			(
				pDest, 
				In00, In01, In02, In03,
				In10, In11, In12, In13,
				In20, In21, In22, In23,
				In30, In31, In32, In33,
				rowStrideInBytes, 
				strideInBytes
			);
			return pDest;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetTransposedMatrix2x2
		(
			DestFloatTypeArg			*pDest,			
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes
		)
		{
			return SetMatrix2x2
			(
				pDest, 
				InRowIndex,
				In00, In10,
				In01, In11,
				rowStrideInBytes,
				strideInBytes
			);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetTransposedMatrix3x3
		(
			DestFloatTypeArg			*pDest,			
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01, SrcFloatTypeArg	In02, 
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11,SrcFloatTypeArg	In12, 
			SrcFloatTypeArg	In20, SrcFloatTypeArg In21,SrcFloatTypeArg	In22, 
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes
		)
		{
			return SetMatrix3x3
			(
				pDest, 
				InRowIndex,
				In00, In10, In20,
				In01, In11, In21,
				In02, In12, In22,
				rowStrideInBytes,
				strideInBytes
			);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetTransposedMatrix4x4
		(
			DestFloatTypeArg			*pDest,			
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01, SrcFloatTypeArg In02, SrcFloatTypeArg In03,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11, SrcFloatTypeArg In12, SrcFloatTypeArg In13,
			SrcFloatTypeArg	In20, SrcFloatTypeArg In21, SrcFloatTypeArg In22, SrcFloatTypeArg In23,
			SrcFloatTypeArg	In30, SrcFloatTypeArg In31, SrcFloatTypeArg In32, SrcFloatTypeArg In33,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes
		)
		{
			return SetMatrix4x4
			(
				pDest, 				
				In00, In10, In20, In30,
				In01, In11, In21, In31,
				In02, In12, In22, In32,
				In03, In13, In23, In33,
				rowStrideInBytes,
				strideInBytes
			);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetMatrix2x2
		(
			DestFloatTypeArg			*pDest,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes
		)
		{
			assert(pDest != nullptr);
			SetRow2(pDest, 0, In00, In01, rowStrideInBytes, strideInBytes);
			SetRow2(pDest, 1, In10, In11, rowStrideInBytes, strideInBytes);
			return pDest;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetMatrix3x3
		(
			DestFloatTypeArg			*pDest,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01, SrcFloatTypeArg In02,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11, SrcFloatTypeArg In12,
			SrcFloatTypeArg	In20, SrcFloatTypeArg In21, SrcFloatTypeArg In22,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes
		)
		{
			assert(pDest != nullptr);
			SetRow3(pDest, 0, In00, In01, In02, rowStrideInBytes, strideInBytes);
			SetRow3(pDest, 1, In10, In11, In12, rowStrideInBytes, strideInBytes);
			SetRow3(pDest, 2, In20, In21, In22, rowStrideInBytes, strideInBytes);
			return pDest;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetMatrix4x4
		(
			DestFloatTypeArg			*pDest,
			SrcFloatTypeArg	In00, SrcFloatTypeArg In01, SrcFloatTypeArg In02, SrcFloatTypeArg In03,
			SrcFloatTypeArg	In10, SrcFloatTypeArg In11, SrcFloatTypeArg In12, SrcFloatTypeArg In13,
			SrcFloatTypeArg	In20, SrcFloatTypeArg In21, SrcFloatTypeArg In22, SrcFloatTypeArg In23,
			SrcFloatTypeArg	In30, SrcFloatTypeArg In31, SrcFloatTypeArg In32, SrcFloatTypeArg In33,
			size_t						 rowStrideInBytes,			
			size_t						 strideInBytes
		)
		{
			assert(pDest != nullptr);
			SetRow4(pDest, 0, In00, In01, In02, In03, rowStrideInBytes, strideInBytes);
			SetRow4(pDest, 1, In10, In11, In12, In13, rowStrideInBytes, strideInBytes);
			SetRow4(pDest, 2, In20, In21, In22, In23, rowStrideInBytes, strideInBytes);
			SetRow4(pDest, 3, In30, In31, In32, In33, rowStrideInBytes, strideInBytes);
			return pDest;
		}

		template<class FloatTypeArg> 
		FloatTypeArg *ZeroRow
		(	
			FloatTypeArg			*pDest, 		
			int						 InNumColumns,
			int						 InRowIndex,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes
		)
		{
			assert(pDest != nullptr);
			FloatTypeArg *pDestRow = Util::AddPtr(pDest, rowStrideInBytes * InRowIndex);
			Set_all_n(pDestRow, (FloatTypeArg)0, InNumColumns, strideInBytes);
			return pDestRow;
		}	

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetRow2
		(
			DestFloatTypeArg		*pDestMatrix,
			int						 InRowIndex,
			SrcFloatTypeArg			 InComponentFirst, 
			SrcFloatTypeArg			 InComponentSecond,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes
		)
		{
			assert(pDestMatrix != nullptr);
			assert(InRowIndex >= 0);
			DestFloatTypeArg *pDestRow = Util::AddPtr(pDestMatrix, InRowIndex * rowStrideInBytes);
			Set2(pDestRow, InComponentFirst, InComponentSecond, strideInBytes);
			return pDestRow;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetRow3
		(
			DestFloatTypeArg		*pDestMatrix,
			int						 InRowIndex,
			SrcFloatTypeArg			 InComponentFirst, 
			SrcFloatTypeArg			 InComponentSecond,
			SrcFloatTypeArg			 InComponentThird,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes
		)
		{
			assert(pDestMatrix != nullptr);
			assert(InRowIndex >= 0);
			DestFloatTypeArg *pDestRow = Util::AddPtr(pDestMatrix, InRowIndex * rowStrideInBytes);
			Set3(pDestRow, InComponentFirst, InComponentSecond, InComponentThird, strideInBytes);
			return pDestRow;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *SetRow4
		(
			DestFloatTypeArg		*pDestMatrix,
			int						 InRowIndex,
			SrcFloatTypeArg			 InComponentFirst, 
			SrcFloatTypeArg			 InComponentSecond,
			SrcFloatTypeArg			 InComponentThird,
			SrcFloatTypeArg			 InComponentFourth,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes
		)
		{
			assert(pDestMatrix != nullptr);
			assert(InRowIndex >= 0);
			DestFloatTypeArg *pDestRow = Util::AddPtr(pDestMatrix, InRowIndex * rowStrideInBytes);
			Set4(pDestRow, InComponentFirst, InComponentSecond, InComponentThird, InComponentFourth, strideInBytes);
			return pDestRow;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrixRow
		(
				DestFloatTypeArg		*pDestMatrix,
				size_t					 InMatrixSize,
				int						 InNumElemsToCopy,
				int						 InDestRowIndex,
				const SrcFloatTypeArg	*pSrcRow,
				size_t					 InDestRowStride,
				size_t					 InDestComponentStride,
				size_t					 InSrcComponentStride				
		)
		{
			assert(pDestMatrix != nullptr);
			assert(InMatrixSize > 0);
			assert(InNumElemsToCopy <= InMatrixSize);
			assert(pSrcRow != nullptr);
			DestFloatTypeArg *pDestRow = Util::AddPtr(pDestMatrix, InDestRowIndex * InDestRowStride);
			Util::CopyStrided
			(
				pDestRow,
				pSrcRow,
				InNumElemsToCopy,
				InDestComponentStride,
				InSrcComponentStride	
			);
			// THERE WAS BUG HERE:
			SetIdentityVectorPart
			(
				pDestRow,
				InMatrixSize,			// total number of elements in the matrix
				InNumElemsToCopy,		// number of already filled elements
				InDestRowIndex,			// index of value that have value 1.0
				InDestComponentStride   // stride between the destination vector components
			);
			return pDestMatrix;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *ResetMatrixColumn
		(
				DestFloatTypeArg		*pDestMatrix,
				size_t					 InMatrixSize,
				int						 InNumElemsToCopy,
				int						 InDestColumnIndex,
				const SrcFloatTypeArg	*pSrcColumn,
				size_t					 InDestRowStride,
				size_t					 InDestComponentStride,
				size_t					 InSrcColumnComponentStride				
		)
		{
			assert(pDestMatrix != nullptr);
			assert(InMatrixSize > 0);
			assert(InNumElemsToCopy <= InMatrixSize);
			assert(pSrcColumn != nullptr);

			DestFloatTypeArg *pDestColumn = Util::AddPtr(pDestMatrix, InDestColumnIndex * InDestComponentStride);
			Util::CopyStrided
			(
				pDestColumn,
				pSrcColumn,
				InNumElemsToCopy,
				InDestRowStride,
				InSrcColumnComponentStride
			);
			SetIdentityVectorPart
			(
				pDestColumn,
				InMatrixSize,			// total number of elements in the matrix
				InNumElemsToCopy,		// number of already filled elements
				InDestColumnIndex,		// index of value that have value 1.0
				InDestRowStride
			);
			return pDestMatrix;
		}

		template<class FloatTypeArg>
		void ZeroMatrix
		(
			FloatTypeArg			*pDest, 			
			int						 InNumRows,
			int						 InNumColumns,
			size_t					 rowStrideInBytes,			
			size_t					 strideInBytes
		)
		{
			assert(pDest != nullptr);
			for(int i = 0; i < InNumRows; i++)
			{
				ZeroRow(pDest, InNumColumns, i, rowStrideInBytes, strideInBytes);
			}
		}

		template<class FloatTypeArg>
		FloatTypeArg *FillMatrix
		(
			FloatTypeArg		*pDestMatrix,
			size_t				InSize,
			FloatTypeArg		InValue,
			size_t				rowStrideInBytes,
			size_t				strideInBytes
		)
		{
			assert(pDestMatrix != nullptr);
			for(int row_index = 0; row_index < InSize; row_index++)
			{
				FloatTypeArg *pDestRow = Util::AddPtr(pDestMatrix, rowStrideInBytes * row_index);
				Set_all_n(pDestRow, InValue, InSize, strideInBytes);
			}
			return pDestMatrix;
		}

		template<class FloatTypeArg, class CoeffType>
		FloatTypeArg *MultScalar
		(
			FloatTypeArg		*pDestMatrix,
			CoeffType			InCoeff,
			int					InNumComponents,
			size_t				InRowByteStride,		
			size_t				InComponentByteStride
		)
		{			
			for(int row_index = 0; row_index < InNumComponents; row_index++)
			{
				FloatTypeArg *pRow = Util::AddPtr(pDestMatrix, InRowByteStride);
				*pRow *= InCoeff;
			}
			return pDestMatrix;
		}

		template<class FloatTypeArg, class CoeffType>
		FloatTypeArg *DivScalar
		(
			FloatTypeArg		*pDestMatrix,
			CoeffType			InCoeff,
			int					InNumComponents,
			size_t				InRowByteStride,		
			size_t				InComponentByteStride
		)
		{
			for(int row_index = 0; row_index < InNumComponents; row_index++)
			{
				FloatTypeArg *pRow = Util::AddPtr(pDestMatrix, InRowByteStride);
				*pRow /= InCoeff;
			}
			return pDestMatrix;
		}

		template<class FloatTypeArg>
		FloatTypeArg *Negate
		(
			FloatTypeArg		*pDestMatrix,			
			int					InNumComponents,
			size_t				InRowByteStride,		
			size_t				InComponentByteStride
		)
		{
			return MultScalar
			(
				pDestMatrix, -1, InNumComponents, 
				InRowByteStride, InComponentByteStride
			);
			return pDestMatrix;
		}

		template
		<
			class DestFloatTypeArg, 
			class SrcFloatTypeArg
		>
		DestFloatTypeArg *AddMatrixRow_To
		(
			DestFloatTypeArg *pDestMatrix, const SrcFloatTypeArg *pSrcMatrix, 
			int row_index, 
			int InNumColumns, 
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep, 			
			uint8_t srcComponentByteStep
		)
		{
			DestFloatTypeArg *pDestRow			= Util::AddPtr	(pDestMatrix, row_index * destComponentRowByteStep);
			const DestFloatTypeArg *pSrcRow		= Util::AddPtr	(pSrcMatrix, row_index * srcComponentRowByteStep);
			Add_n_To(pDestRow, pSrcRow, InNumColumns, destComponentByteStep, srcComponentByteStep);
			return pDestMatrix;
		}

		template
		<
			class DestFloatTypeArg, 
			class SrcFloatTypeArg
		>
		DestFloatTypeArg *RemoveMatrixRow_From
		(
			DestFloatTypeArg *pDestMatrix, const SrcFloatTypeArg *pSrcMatrix, 
			int row_index, 
			int InNumColumns, 
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep, 			
			uint8_t srcComponentByteStep
		)
		{
			DestFloatTypeArg *pDestRow			= Util::AddPtr	(pDestMatrix, row_index * destComponentRowByteStep);
			const DestFloatTypeArg *pSrcRow		= Util::AddPtr	(pSrcMatrix, row_index * srcComponentRowByteStep);
			Subtract_n_from(pDestRow, pSrcRow, InNumColumns, destComponentByteStep, srcComponentByteStep);
			return pDestMatrix;
		}

		template
		<
			class DestFloatTypeArg, 
			class SrcFloatTypeArg
		>
		DestFloatTypeArg *AddMatrix_To
		(
			DestFloatTypeArg *pDestMatrix, 
			const SrcFloatTypeArg *pSrcMatrix, 
			int InNumRows, int InNumColumns,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep, 			
			uint8_t srcComponentByteStep
		)
		{
			for(int row_index = 0; row_index < InNumRows; row_index++)
			{
				AddMatrixRow_To
				(
					pDestMatrix, pSrcMatrix, 
					row_index, 
					InNumColumns, 
					destComponentRowByteStep, srcComponentRowByteStep, 			
					destComponentByteStep, srcComponentByteStep
				);
			}
			return pDestMatrix;
		}

		template
		<
			class DestFloatTypeArg, 
			class SrcFloatTypeArg
		>
		DestFloatTypeArg *SubtractMatrix_From
		(
			DestFloatTypeArg *pDestMatrix, 
			const SrcFloatTypeArg *pSrcMatrix, 
			int InNumRows, int InNumColumns,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t destComponentByteStep, 			
			uint8_t srcComponentByteStep
		)
		{
			for(int row_index = 0; row_index < InNumRows; row_index++)
			{
				RemoveMatrixRow_From
				(
					pDestMatrix, pSrcMatrix, 
					row_index, 
					InNumColumns, 
					destComponentRowByteStep, srcComponentRowByteStep, 			
					destComponentByteStep, srcComponentByteStep
				);
			}
			return pDestMatrix;
		}

		template
		<
			class DestFloatTypeArg, 
			class SrcFloatTypeArg,
			class SrcTwoFloatTypeArg
		>
		void MatrixCopyMult
		(
			DestFloatTypeArg			*pDestMatrix, 
			const SrcFloatTypeArg		*pSrcMatrix,
			const SrcTwoFloatTypeArg	*pSrcTwoMatrix,
			int InDestNumRows, int InDestNumColumns,
			uint8_t destComponentRowByteStep, 
			uint8_t srcComponentRowByteStep, 			
			uint8_t srcTwoComponentRowByteStep, 	
			uint8_t destComponentByteStep,
			uint8_t srcComponentByteStep,
			uint8_t srcTwoComponentByteStep
		)
		{
			assert(pDestMatrix != nullptr);
			assert(pSrcMatrix != nullptr);
			assert(pSrcTwoMatrix != nullptr);
			// Memory regions may not overlap
			assert(static_cast<const void *>(pSrcMatrix) != static_cast<const void *>(pSrcTwoMatrix));
			assert(static_cast<void *>(pDestMatrix) != static_cast<const void *>(pSrcMatrix));
			assert(static_cast<void *>(pDestMatrix) != static_cast<const void *>(pSrcTwoMatrix));

			for(int row_index = 0; row_index < InDestNumRows; row_index++)
			{
				for(int column_index = 0; column_index < InDestNumColumns; column_index++)
				{
					const SrcFloatTypeArg		*pSrcRow		             = Util::AddPtr(pSrcMatrix, row_index * srcComponentRowByteStep);
					const SrcTwoFloatTypeArg	*pSrcTwoColumn		         = Util::AddPtr(pSrcTwoMatrix, column_index * srcTwoComponentByteStep); 
					DestFloatTypeArg			*pDestComponent              = Util::AddPtr(pDestMatrix, row_index * destComponentRowByteStep + column_index * destComponentByteStep);

					// Calculating a dot to compute the matrix multiplication result at the given cell:
					// WARNING: to treat the second vector as column matrix, we pass the component ROW stride
					// where the function requires to pass the component stride.
					*pDestComponent = Dot_n(pSrcRow, pSrcTwoColumn, InDestNumColumns, srcComponentByteStep, srcTwoComponentRowByteStep);
				}
			}			
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *MatrixCopyInverse_UsePrecomputedDet44
		(
			DestFloatTypeArg			*pDestMatrix,
			const SrcFloatTypeArg		*pSrcMatrix,
			SrcFloatTypeArg				 InPrecomputedDet,			
			size_t						 InDestRowByteStride,	
			size_t						 InSrcRowByteStride,
			size_t						 InDestComponentByteStride,
			size_t						 InSrcComponentByteStride
		)
		{
			assert(pDestMatrix != nullptr);
			assert(pSrcMatrix != nullptr);
			assert(pDestMatrix != pSrcMatrix);
			for(int dest_row_index = 0; dest_row_index < 4; dest_row_index++)
			{
				for(int dest_column_index = 0; dest_column_index < 4; dest_column_index++)
				{
					// We swapped the the dest column index and the dest row index
					// since the result is to be transposed
					DestFloatTypeArg *pDestComponent = Util::AddPtr
					(
						pDestMatrix, 
						InDestRowByteStride * dest_column_index + 
						InDestComponentByteStride * dest_row_index
					);
					SrcFloatTypeArg cofactor = ComputeSubmatrixCofactor44
					(
						pSrcMatrix,
						dest_row_index, dest_column_index,
						InSrcRowByteStride,
						InSrcComponentByteStride
					);
					*pDestComponent = cofactor / InPrecomputedDet;
				}
			}
			return pDestMatrix;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *MatrixCopyInverse_UsePrecomputedDet33
		(
			DestFloatTypeArg			*pDestMatrix,
			const SrcFloatTypeArg		*pSrcMatrix,
			SrcFloatTypeArg				 InPrecomputedDet,
			size_t						 InDestRowByteStride,
			size_t						 InSrcRowByteStride,
			size_t						 InDestComponentByteStride,
			size_t						 InSrcComponentByteStride
		)
		{
			assert(pDestMatrix != nullptr);
			assert(pSrcMatrix != nullptr);
			assert(pDestMatrix != pSrcMatrix);
			for (int dest_row_index = 0; dest_row_index < 3; dest_row_index++)
			{
				for (int dest_column_index = 0; dest_column_index < 3; dest_column_index++)
				{
					// We swapped the the dest column index and the dest row index
					// since the result is to be transposed
					DestFloatTypeArg *pDestComponent = Util::AddPtr
					(
						pDestMatrix,
						InDestRowByteStride * dest_column_index +
						InDestComponentByteStride * dest_row_index
					);
					SrcFloatTypeArg cofactor = ComputeSubmatrixCofactor33
					(
						pSrcMatrix,
						dest_row_index, dest_column_index,
						InSrcRowByteStride,
						InSrcComponentByteStride
					);
					*pDestComponent = cofactor / InPrecomputedDet;
				}
			}
			return pDestMatrix;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *MatrixCopyInverse_UsePrecomputedDet22
		(
			DestFloatTypeArg			*pDestMatrix,
			const SrcFloatTypeArg		*pSrcMatrix,
			SrcFloatTypeArg				 InPrecomputedDet,
			size_t						 InDestRowByteStride,
			size_t						 InSrcRowByteStride,
			size_t						 InDestComponentByteStride,
			size_t						 InSrcComponentByteStride
		)
		{
			assert(pDestMatrix != nullptr);
			assert(pSrcMatrix != nullptr);
			assert(pDestMatrix != pSrcMatrix);

			// Rows
			auto pSrcRow_0 = pSrcMatrix;
			auto pSrcRow_1 = Util::AddPtr(pSrcRow_0, InSrcRowByteStride);

			// Components of row 0
			auto pSrc_0_0 = pSrcRow_0;
			auto pSrc_1_0 = Util::AddPtr(pSrcRow_0, InSrcComponentByteStride);

			// Components of row 1
			auto pSrc_0_1 = pSrcRow_1;
			auto pSrc_1_1 = Util::AddPtr(pSrcRow_1, InSrcComponentByteStride);

			// Setting up dest components: row 0
			auto pDest_0_0 = pDestMatrix;
			*pDestMatrix =  *pSrc_1_1 / InPrecomputedDet;

			auto pDest0_1 = Util::AddPtr(pDest_0_0, InDestComponentByteStride);
			*pDest0_1 = -(*pSrc_0_1) / InPrecomputedDet;

			// Setting up dest components: row 1
			auto pDest_1_0 = Util::AddPtr(pDestMatrix, InDestRowByteStride);
			*pDest_1_0 = -*pSrc_1_0 / InPrecomputedDet;

			auto pDest_1_1 = Util::AddPtr(pDest_1_0, InDestComponentByteStride);
			*pDest_1_1 = *pSrc_0_0 / InPrecomputedDet;

			return pDestMatrix;
		}

		template<class SrcFloatTypeArg>
		SrcFloatTypeArg ComputeSubmatrixCofactor44
		(
			const SrcFloatTypeArg *pSrcMatrix,
			size_t InRowIndex, size_t InColumnIndex,
			size_t InSrcRowByteStride,
			size_t InSrcComponentByteStride
		)
		{
			assert(pSrcMatrix != nullptr);
			assert(InRowIndex >= 0);
			assert(InColumnIndex >= 0);
			assert(InRowIndex < 4);
			int index_sum = InRowIndex + InColumnIndex;
			int sign_coeff = 1 + (index_sum & 1) * (-2);
			SrcFloatTypeArg minor_det_value = Minor_Det33
			(
				pSrcMatrix, 
				InRowIndex, InColumnIndex,
				InSrcRowByteStride,
				InSrcComponentByteStride
			);
			SrcFloatTypeArg res_value = sign_coeff * minor_det_value;			
			return res_value;
		}

		template<class SrcFloatTypeArg>
		SrcFloatTypeArg ComputeSubmatrixCofactor33
		(
			const SrcFloatTypeArg *pSrcMatrix,
			size_t InRowIndex, size_t InColumnIndex,
			size_t InSrcRowByteStride,
			size_t InSrcComponentByteStride
		)
		{
			assert(pSrcMatrix != nullptr);
			assert(InRowIndex >= 0);
			assert(InColumnIndex >= 0);
			assert(InRowIndex < 3);
			int index_sum = InRowIndex + InColumnIndex;
			int sign_coeff = 1 + (index_sum & 1) * (-2);
			SrcFloatTypeArg minor_det_value = Minor_Det22
			(
				pSrcMatrix,
				InRowIndex, InColumnIndex,
				InSrcRowByteStride,
				InSrcComponentByteStride
			);
			SrcFloatTypeArg res_value = sign_coeff * minor_det_value;
			return res_value;
		}

		template<class SrcFloatTypeArg>
		SrcFloatTypeArg Minor_Det33
		(
			const SrcFloatTypeArg *pSrcMatrix, 
			size_t InRowIndex, size_t InColumnIndex,
			size_t InSrcRowByteStride,
			size_t InSrcComponentByteStride
		)
		{
			assert(pSrcMatrix != nullptr);
			assert(InRowIndex >= 0);
			assert(InColumnIndex >= 0);
			assert(InRowIndex < 4);
			assert(InColumnIndex < 4);

			// This functions uses the triangle rule
			// to calculate the determinant of the 3*3 matrix

			// This variables are multiples of the elements:			
			SrcFloatTypeArg mult_main_diag = 1;
			SrcFloatTypeArg mult_secondry_diag = 1;
			SrcFloatTypeArg mult_upper_left_tri = 1;
			SrcFloatTypeArg mult_down_left_tri = 1;
			SrcFloatTypeArg mult_down_right_tri = 1;
			SrcFloatTypeArg mult_upper_right_tri = 1;

			// Calculating multiplications

			// Let's represent our matrix as
			// | a b c |
			// | d e f |
			// | g h i |

			int row_index = 0;
			int column_index = 0;
			const SrcFloatTypeArg *pRow = pSrcMatrix;			
			const SrcFloatTypeArg *pComponent = pRow;
			
			// Let's find first non crossed-out row:
			if(row_index == InRowIndex)
			{
				row_index++;
				pRow = Util::AddPtr(pRow, InSrcRowByteStride);
				pComponent = pRow;
			}

			// At this point all variables point to the first row of the submatrix
			
			// Let's find the "a" coeff:

			// Skip the first column if it's crossed out
			if(column_index == InColumnIndex)
			{
				column_index++;
				pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);
			}

			// pComponent now points to the "A" coeff:
			SrcFloatTypeArg A = *pComponent;

			// Go to the next component
			column_index++;
			pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);

			// Skip the column if it's crossed out
			if(column_index == InColumnIndex)
			{
				column_index++;
				pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);
			}

			// pComponent now points to the "B" coeff:
			SrcFloatTypeArg B = *pComponent;

			// Go to the next component
			column_index++;
			pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);

			// Skip the column if it's crossed out
			if(column_index == InColumnIndex)
			{
				column_index++;
				pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);
			}

			// pComponent now points to the "C" coeff:
			SrcFloatTypeArg C = *pComponent;

			// Go to the next row
			row_index++;
			pRow = Util::AddPtr(pRow, InSrcRowByteStride);

			// Skip the row if it's crossed out
			if(row_index == InRowIndex)
			{
				row_index++;
				pRow = Util::AddPtr(pRow, InSrcRowByteStride);
			}
			column_index = 0;
			pComponent = pRow;

			// Skip the column if it's crossed out
			if(column_index == InColumnIndex)
			{
				column_index++;
				pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);
			}

			// Current row is second row now
			// Current element is "D" coeff
			SrcFloatTypeArg D = *pComponent;

			// Go to the next component
			column_index++;
			pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);

			// Skip the column if it's crossed out
			if(column_index == InColumnIndex)
			{
				column_index++;
				pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);
			}

			// Current element is "E"
			SrcFloatTypeArg E = *pComponent;

			// Go to the next component
			column_index++;
			pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);

			// Skip the column if it's crossed out
			if(column_index == InColumnIndex)
			{
				column_index++;
				pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);
			}

			// Current element is "F"
			SrcFloatTypeArg F = *pComponent;

			// Go to the next row
			row_index++;
			pRow = Util::AddPtr(pRow, InSrcRowByteStride);

			// Skip the row if it's crossed out
			if(row_index == InRowIndex)
			{
				row_index++;
				pRow = Util::AddPtr(pRow, InSrcRowByteStride);
			}
			column_index = 0;
			pComponent = pRow;

			// Skip the column if it's crossed out
			if(column_index == InColumnIndex)
			{
				column_index++;
				pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);
			}

			// Current row is third row
			// Current element is "G" coeff
			SrcFloatTypeArg G = *pComponent;

			// Go to the next component
			column_index++;
			pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);

			// Skip the column if it's crossed out
			if(column_index == InColumnIndex)
			{
				column_index++;
				pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);
			}

			// Current element is "H" coeff
			SrcFloatTypeArg H = *pComponent;

			// Go to the next component
			column_index++;
			pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);

			// Skip the column if it's crossed out
			if(column_index == InColumnIndex)
			{
				column_index++;
				pComponent = Util::AddPtr(pComponent, InSrcComponentByteStride);
			}

			// Current element is "I" coeff
			SrcFloatTypeArg I = *pComponent;


			mult_main_diag = A * E * I;
			mult_upper_left_tri = A * F * H;		

			mult_secondry_diag = C * E * G;
			mult_down_left_tri = B * F * G;
			mult_down_right_tri = B * D * I;

			mult_upper_right_tri = C * D * H;


			// Summing
			// Sum of all elements (triangles and diag mults) which to be taken with positive sign
			SrcFloatTypeArg sum_positive =
				mult_main_diag + mult_down_left_tri + mult_upper_right_tri;
			// Sum of all elements (triangles and diag mults) which to be taken with negative sign
			SrcFloatTypeArg sum_negative =
				mult_secondry_diag + mult_upper_left_tri + mult_down_right_tri;
			SrcFloatTypeArg res_det = sum_positive - sum_negative;
			return res_det;
		}

		template<class SrcFloatTypeArg>
		SrcFloatTypeArg Minor_Det22
		(
			const SrcFloatTypeArg *pSrcMatrix,
			size_t InRowIndex, size_t InColumnIndex,
			size_t InSrcRowByteStride,
			size_t InSrcComponentByteStride
		)
		{
			// index of row that we will use for determinant row zero
			auto det_row_zero = 0;
			// index of row that we will use for determinant row one
			auto det_row_one = 1;
			switch(InRowIndex)
			{
			case 0:
				det_row_zero = 1;
				det_row_one = 2;
				break;

			case 1:
				det_row_zero = 0;
				det_row_one = 2;
				break;

			case 2:
				// default values are ok
				break;

			default:
				assert(false == "Should not get here");
			}

			// index of column that we will use for determinant column zero
			auto det_column_zero = 0;
			// index of column that we will use for determinant column one
			auto det_column_one = 1;

			switch (InColumnIndex)
			{
			case 0:
				det_column_zero = 1;
				det_column_one = 2;
				break;

			case 1:
				det_column_zero = 0;
				det_column_one = 2;
				break;

			case 2:
				// default values are ok
				break;

			default:
				assert(false == "Should not get here");
			}

			auto pSrcDetRow_Zero = Util::AddPtr(pSrcMatrix, InSrcRowByteStride * det_row_zero);
			auto pSrcDet_0_0 = Util::AddPtr(pSrcDetRow_Zero, InSrcComponentByteStride * det_column_zero);
			auto pSrcDet_0_1 = Util::AddPtr(pSrcDetRow_Zero, InSrcComponentByteStride * det_column_one);

			auto pSrcDetRow_One = Util::AddPtr(pSrcMatrix, InSrcRowByteStride * det_row_one);
			auto pSrcDet_1_0 = Util::AddPtr(pSrcDetRow_One, InSrcComponentByteStride * det_column_zero);
			auto pSrcDet_1_1 = Util::AddPtr(pSrcDetRow_One, InSrcComponentByteStride * det_column_one);

			auto result = *pSrcDet_0_0 * *pSrcDet_1_1 - *pSrcDet_0_1 * *pSrcDet_1_0;
			return result;
		}


		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg *MatrixTransposeCopy
		(
			DestFloatTypeArg			*pDestMatrix,
			const SrcFloatTypeArg		*pSrcMatrix,
			size_t						 InSize,
			size_t						 InDestRowByteStride,
			size_t						 InSrcRowByteStride,
			size_t						 InDestComponentByteStride,
			size_t						 InSrcComponentByteStride
		)
		{
			assert(pDestMatrix != nullptr);
			assert(pSrcMatrix != nullptr);
			assert(InSize > 0);

			for(int row_index = 0; row_index < InSize; row_index++)
			{
				const SrcFloatTypeArg		*pSrcRow		= Util::AddPtr(pSrcMatrix, InSrcRowByteStride * row_index);
				for(int column_index = 0; column_index < InSize; column_index++)
				{
					const SrcFloatTypeArg		*pSrcComponent	= Util::AddPtr(pSrcRow, InSrcComponentByteStride * column_index);

					DestFloatTypeArg	*pDestRow		= Util::AddPtr(pDestMatrix, InDestRowByteStride * column_index);
					DestFloatTypeArg	*pDestComponent = Util::AddPtr(pDestRow, InDestComponentByteStride * row_index);

					*pDestComponent = *pSrcComponent;
				}
			}
			return pDestMatrix;
		}

		template<class DestFloatTypeArg>
		DestFloatTypeArg *MatrixTranspose
		(
			DestFloatTypeArg			*pMatrix,
			size_t						 InSize,
			size_t						 InRowByteStride,
			size_t						 InComponentByteStride
		)
		{
			assert(pMatrix != nullptr);
			assert(InSize > 0);			

			// ALGORITHM NOTE:
			//		We just swap each pair of elements (i,j) and (j,i)
			//	where (i,j) is an element ABOVE the main diagonal.
			for(int row_index = 0; row_index < InSize; row_index++)
			{
				DestFloatTypeArg *pSrcRow = Util::AddPtr(pMatrix, InRowByteStride * row_index);

				for(int column_index = row_index + 1; column_index < InSize; column_index++)
				{					
					DestFloatTypeArg	*pSrcElement	= Util::AddPtr(pSrcRow, InComponentByteStride * column_index);	

					// Get address of the destination
					DestFloatTypeArg		*pDestRow		= Util::AddPtr(pMatrix, InRowByteStride * column_index);
					DestFloatTypeArg        *pDestElement	= Util::AddPtr(pDestRow, InComponentByteStride * row_index);

					std::swap(*pDestElement, *pSrcElement);
				}				
			}
			return pMatrix;
		}

		template<class SrcFloatTypeArg>
		std::string MatrixToDebugString
		(
			const SrcFloatTypeArg			*InMatrix,
			int								 InSize,
			size_t							 InRowStride,
			size_t							 InComponentStide
		)
		{
			assert(InMatrix != nullptr);

			std::string res_string;
			for(int row_index = 0; row_index < InSize; row_index++)
			{
				const SrcFloatTypeArg *pRow = Util::AddPtr(InMatrix, InRowStride * row_index);
				std::string str_row = VectorToDebugString(pRow, InSize, InComponentStide);
				res_string.append(str_row);
				res_string.append("\n");
			}
			return res_string;
		}

		template<class SrcFloatTypeArg>
		std::string MatrixToString
		(
			const SrcFloatTypeArg			*InMatrix,
			int								 InSize,
			size_t							 InRowStride,
			size_t							 InComponentStide
		)
		{
			return MatrixToDebugString(InMatrix, InSize, InRowStride, InComponentStide);
		}
	} // Math
} // DvUtils

#endif // _MathUtil_Core_LinearAlgebraUtils_h_
