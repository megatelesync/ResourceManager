#ifndef _MathUtil_3DMath_Tri_h_
#define _MathUtil_3DMath_Tri_h_

#include <array>
#include "../geometry/MathUtil_Geometry_Tri.h"

namespace DvUtils
{
	namespace Math
	{
		// Compute unnormalized normal of the 3D triangle
		// The length of the normal is equal to the are of the parallelogram 
		// based on vectors AB and AC
		template<class FloatTypeArg>
		FloatTypeArg *ComputeTriSignedAreaNormal3D
		(
			FloatTypeArg *pRes,
			const FloatTypeArg *pInA, 
			const FloatTypeArg *pInB,
			const FloatTypeArg *pInC,			
			uint8_t res_componentByteStride = sizeof(FloatTypeArg),
			uint8_t a_componentByteStride = sizeof(FloatTypeArg),
			uint8_t b_componentByteStride = sizeof(FloatTypeArg),
			uint8_t c_componentByteStride = sizeof(FloatTypeArg)			
		);

		// Computes the double area of the 2D triangle given with 3 vertices;
		// The formula used is:
		// | x1 y1 1 |
		// | x2 y2 1 |
		// | x3 y3 1 |		
		// For the positive result the vertices to be taken COUNTER clockwise.
		// MAY BE WRONG: WHY IT TAKES 9 operations if should take only 7 operations (2d determinant value)?
		template<class FloatTypeArg>
		FloatTypeArg ComputeTriangleSignedDoubledArea2D
		(
			const FloatTypeArg& X1, const FloatTypeArg& Y1,
			const FloatTypeArg& X2, const FloatTypeArg& Y2,
			const FloatTypeArg& X3, const FloatTypeArg& Y3
		);






		template<class FloatTypeArg>
		FloatTypeArg ComputeTriangleSignedDoubledArea2D
		(
			const FloatTypeArg& X1, const FloatTypeArg& Y1,
			const FloatTypeArg& X2, const FloatTypeArg& Y2,
			const FloatTypeArg& X3, const FloatTypeArg& Y3
		)
		{
			// Y2-Y3:						1 subs
			// X2-X3:						1 subs			 
			// X2 * Y3 - X3 * Y2:			1 subs			
			//								2 outermost subs/adds
			// X1 * (***)															1 mult
			// Y1 * (***)															1 mult
			// X2 * Y3																1 mult
			// X3 * Y2																1 mult
			// ----------------------------------------------------------------------------
			// total:
			//								5 subs/adds								4 mults
			//																   9 operations
			return X1 * (Y2-Y3) - Y1 * (X2-X3) + (X2 * Y3 - X3 * Y2);
		};

		template<class FloatTypeArg>
		FloatTypeArg *ComputeTriSignedAreaNormal3D
		(
			FloatTypeArg *pRes,
			const FloatTypeArg *pInA, 
			const FloatTypeArg *pInB,
			const FloatTypeArg *pInC,			
			uint8_t res_componentByteStride,
			uint8_t a_componentByteStride,
			uint8_t b_componentByteStride,
			uint8_t c_componentByteStride			
		)
		{
			assert(pRes != nullptr);
			assert(pInA != nullptr);
			assert(pInB != nullptr);
			assert(pInC != nullptr);			
			typename FloatTypeArg AB_vector[3];
			// 3 adds
			SetSubtract_n(AB_vector, pInB, pInA, 3, sizeof(FloatTypeArg), b_componentByteStride, a_componentByteStride);
			typename FloatTypeArg AC_vector[3];
			// 3 adds
			SetSubtract3(AB_vector, pInC, pInA, 3, sizeof(FloatTypeArg), c_componentByteStride, a_componentByteStride);
			// 6 mults; 3 adds			
			Cross3(pRes, AB_vector, AC_vector, res_componentByteStride);
			// total: 9 adds; 6 mults; 15 operations
			return pRes;
		}
	} // Math
} // DvUtils

#endif // _MathUtil_3DMath_Tri_h_