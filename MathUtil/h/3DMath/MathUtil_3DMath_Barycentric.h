#ifndef _MathUtil_3DMath_Barycentric_h_
#define _MathUtil_3DMath_Barycentric_h_

#include <array>
#include <utility>
#include "MathUtil_3DMath_Tri.h"
#include "../Core/MathUtil_Core_LinearAlgebraUtils.h"
#include "../MathUtil_Solve.h"

namespace DvUtils
{
	// TODO:
	// 1. Compute triangle barycentric 3D
	// 2. Create an operation for calculation of barycentric coordinates based on some earlier precomputed data 
	// 2.1. 2D +
	// 2.2. 3D

	namespace Math
	{
		// Compute u,v and w, barycentric coordinates of the point of the triangle
		// P = u*A+v*B+w*C
		// Result:
		//		pRes[0] = u;
		//		pRes[1] = v;
		//		pRes[2] = w;
		// WARNING: Use other (optimized) version if you compute barycentric coordinates 
		// of several points for the same triangle
		template<class FloatTypeArg>
		FloatTypeArg *ComputeTriangleBarycentric2D
		(
			FloatTypeArg *pRes,
			const FloatTypeArg *pInA, 
			const FloatTypeArg *pInB,
			const FloatTypeArg *pInC,
			const FloatTypeArg *pInPoint
		);

		// Optimized version for the barycentric coordinate calculation
		// Arguments:
		// dir_AB: 2D vector, vector from A to B;
		// dir_AC: 2D vector, vector from A to C;
		// AB dot AB: 2D vector, (vector from A to B) dot (vector from A to B)
		// AC dot AC: 2D vector, (vector from A to C) dot (vector from A to C)
		// AB dot AC: 2D vector, (vector from A to B) dot (vector from A to C)
		template<class FloatTypeArg>
		FloatTypeArg *ComputePrecomputedTriangleBarycentric2D_vw
		(
			FloatTypeArg *pRes,
			const FloatTypeArg *pInA, 
			const FloatTypeArg *pInB,
			const FloatTypeArg *pInC,			
			const FloatTypeArg *pInPoint,
			const FloatTypeArg *vector_AB,
			const FloatTypeArg *vector_AC,
			FloatTypeArg AB_dot_AB,
			FloatTypeArg AB_dot_AC,
			FloatTypeArg AC_dot_AC
		);

		// Compute barycentric coordinates u,v,w of the 3D triangle
		// P = u*A+v*B+w*C
		// Result:
		//		pRes[0] = u;
		//		pRes[1] = v;
		//		pRes[2] = w;
		// Arguments:
		//		pInA, pInB, pInC: 3D vectors, points of the 3D triangle
		//		pInPoint: 3D vector, point to compute barycentric coordinates of
		// WARNING: Use other (optimized) version if you compute barycentric coordinates 
		// of several points for the same triangle
		template<class FloatTypeArg>
		FloatTypeArg *ComputeTriangleBarycentric3D
		(
			FloatTypeArg *pRes,
			const FloatTypeArg *pInA, 
			const FloatTypeArg *pInB,
			const FloatTypeArg *pInC,
			const FloatTypeArg *pInPoint,
			uint8_t res_componentByteStride = sizeof(FloatTypeArg),
			uint8_t a_componentByteStride = sizeof(FloatTypeArg),
			uint8_t b_componentByteStride = sizeof(FloatTypeArg),
			uint8_t c_componentByteStride = sizeof(FloatTypeArg),
			uint8_t point_componentByteStride = sizeof(FloatTypeArg)
		);

		// Result:
		//		pRes[0] = u;
		//		pRes[1] = v;	
		// WARNING: Use other (optimized) version if you compute barycentric coordinates 
		// of several points for the same triangle
		template<class FloatTypeArg>
		FloatTypeArg *ComputeTriangleBarycentric2D_vw
		(
			FloatTypeArg *pRes,
			const FloatTypeArg *pInA, 
			const FloatTypeArg *pInB,
			const FloatTypeArg *pInC,
			const FloatTypeArg *pInPoint
		);





		// Impl
		template<class FloatTypeArg>
		FloatTypeArg *ComputeTriangleBarycentric3D
		(
			FloatTypeArg *pRes,
			const FloatTypeArg *pInA, 
			const FloatTypeArg *pInB,
			const FloatTypeArg *pInC,
			const FloatTypeArg *pInPoint,
			uint8_t res_componentByteStride,
			uint8_t a_componentByteStride,
			uint8_t b_componentByteStride,
			uint8_t c_componentByteStride,
			uint8_t point_componentByteStride
		)
		{
			assert(pRes != nullptr);
			assert(pInA != nullptr);
			assert(pInB != nullptr);
			assert(pInC != nullptr);
			assert(pInPoint != nullptr);		
			typename FloatTypeArg AB_cross_AC[3];
			ComputeTriSignedAreaNormal3D
			(				
				AB_cross_AC,
				pInA, pInB, pInC,
				a_componentByteStride,
				b_componentByteStride,
				c_componentByteStride			
			)
			typename FloatTypeArg abs_AB_cross_AC[3];
			CopyAbsolutize3(abs_AB_cross_AC, AB_cross_AC);			
			int max_abs_axis = Max3Index(abs_AB_cross_AC);

		}

		template<class FloatTypeArg>
		FloatTypeArg *ComputeTriangleBarycentric2D
		(
			FloatTypeArg *pRes,
			const FloatTypeArg *pInA, 
			const FloatTypeArg *pInB,
			const FloatTypeArg *pInC,
			const FloatTypeArg *pInPoint
		)
		{
			assert(pRes != nullptr);
			assert(pInA != nullptr);
			assert(pInB != nullptr);
			assert(pInC != nullptr);
			assert(pInPoint != nullptr);
			ComputeTriangleBarycentric2D_vw(pRes + 1, pInA, pInB, pInC, pInPoint);
			pRes[0] = 1.0 - pRes[1] - pRes[2];
			return pRes;
		}

		template<class FloatTypeArg>
		FloatTypeArg *ComputePrecomputedTriangleBarycentric2D_vw
		(
			FloatTypeArg *pRes,
			const FloatTypeArg *pInA, 
			const FloatTypeArg *pInB,
			const FloatTypeArg *pInC,			
			const FloatTypeArg *pInPoint,
			const FloatTypeArg *vector_AB,
			const FloatTypeArg *vector_AC,
			FloatTypeArg AB_dot_AB,
			FloatTypeArg AB_dot_AC,
			FloatTypeArg AC_dot_AC
		)
		{
			assert(pRes != nullptr);
			assert(pInA != nullptr);
			assert(pInB != nullptr);
			assert(pInC != nullptr);
			assert(pInPoint != nullptr);
			assert(vector_AB);
			assert(vector_AC);

			// 2 subs
			typename FloatTypeArg vector_A_to_Point[2];			
			SetSubtract2(vector_A_to_Point, pInPoint, pInA);
			// 1 adds, 2 mult
			typename FloatTypeArg A_to_Point_dot_AB = Dot2(vector_A_to_Point, vector_AB);
			// 1 adds, 2 mult
			typename FloatTypeArg A_to_Point_dot_AC = Dot2(vector_A_to_Point, vector_AC);
			// 3 adds; 6 mults; 2 divs
			Solve2x2
			(
				pRes,
				AB_dot_AB, AB_dot_AC, 
				AB_dot_AC, AC_dot_AC, 
				A_to_Point_dot_AB, A_to_Point_dot_AC
			);
			// total:
			// 19 operations (2+1+1+3=7 adds/subs; 2+2+6=10 mults; 2 divs); 			
			return pRes;
		}

		template<class FloatTypeArg>
		FloatTypeArg *ComputeTriangleBarycentric2D_vw
		(
			FloatTypeArg *pRes,
			const FloatTypeArg *pInA, 
			const FloatTypeArg *pInB,
			const FloatTypeArg *pInC,
			const FloatTypeArg *pInPoint
		)
		{
			assert(pRes != nullptr);
			assert(pInA != nullptr);
			assert(pInB != nullptr);
			assert(pInC != nullptr);
			assert(pInPoint != nullptr);
			typename FloatTypeArg vector_AB[2];
			// 2 subs
			SetSubtract2(vector_AB, pInB, pInA);			
			typename FloatTypeArg vector_AC[2];
			// 2 subs
			SetSubtract2(vector_AC, pInC, pInA);
			// 1 adds; 2 mults
			typename FloatTypeArg AB_dot_AB = Dot2(vector_AB, vector_AB);			
			// 1 adds; 2 mults
			typename FloatTypeArg AB_dot_AC = Dot2(vector_AB, vector_AC);
			// 1 adds; 2 mults
			typename FloatTypeArg AC_dot_AC = Dot2(vector_AC, vector_AC);

			// 7 adds/subs; 10 mults; 2 divs
			ComputePrecomputedTriangleBarycentric2D_vw
			(
				pRes, 
				pInA, pInB, pInC, 
				pInPoint, 
				vector_AB, vector_AC, 
				AB_dot_AB, AB_dot_AC, AC_dot_AC
			);
			// total:
			// 32 operations (7+1+1+1+2+2=14 adds/subs; 10+2+2+2=16 mults; 2 divs); 			
			return pRes;
		}
	} // Math
} // DvUtils

#endif // _MathUtil_3DMath_Barycentric_h_