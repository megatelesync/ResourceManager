#pragma once
#include "MathUtil_PlaneTypes.h"
#include "MathTypes\BLAS\MathUtil_BLAS.h"
#include "Core/MathUtil_Core_LinearAlgebraUtils.h"

namespace DvUtils
{
   namespace Math
   {
		// Get signed distance between a point in space and a plane 
	    // (assuming the plane normal is normalized)
	    template<class FloatTypeArg>
		Vec<FloatTypeArg, 3> SignedDistanceNormalized(const Vec<FloatTypeArg, 3>& P, const Plane& plane);





		// IMPL
		template<class FloatTypeArg>
		Vec<FloatTypeArg, 3> SignedDistanceNormalized(const Vec<FloatTypeArg, 3>& P, const Plane& plane)
		{
			const FloatTypeArg *pPointFloatArr = P.ToFloatArr();
			const FloatTypeArg *pPlaneFloatArr = plane.ToFloatArr();
			const FloatTypeArg plane_d = pPointFloatArr[3];
			// Dot3 (3 mults + 2 adds) + 1 sub = 6 float operations 			
			return Dot3(pPointFloatArr, pPlaneFloatArr) - plane_d;
		}
   } // namespace Math
} // namespace DvUtils
