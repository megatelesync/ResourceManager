#ifndef _MathUtil_CollisionDetection_volume_Capsule_h_
#define _MathUtil_CollisionDetection_volume_Capsule_h_

#include "MathUtil_CollisionDetection_volume_utils.h"

namespace DvUtils
{
	namespace Math
	{
		// Capsule
		// (Sphere swept line) 
		template<class FloatTypeArg>
		struct Capsule
		{
			typedef FloatTypeArg				FloatType;
			typedef Vec<FloatTypeArg, 3>		VecType;

			VecType								lineStart;
			VecType								lineEnd;
			FloatType							radius;
		};
	} // Math
} // Dv

#endif // _MathUtil_CollisionDetection_volume_Capsule_h_