#ifndef _MathUtil_CollisionDetection_volume_Slab_h_
#define _MathUtil_CollisionDetection_volume_Slab_h_

#include "MathUtil_CollisionDetection_volume_utils.h"

namespace DvUtils
{
	namespace Math
	{
		// Slab is a space defined with a pair of parallel planes
		// (with oppositen normals)
		template<class FloatTypeArg>
		struct Slab
		{
			typedef FloatTypeArg				FloatType;
			typedef Vec<FloatTypeArg, 3>		VecType;

			// positive normal
			VecType						normal;
			// distance to first plane
			FloatType					minDist;
			// distance to last plane
			FloatType					maximDist;

			inline Slab();
			inline Slab
			(
				const VecType& InVecType, 
				const FloatType& InMinDist, const FloatType& InMaximDist
			);
		};		
	} // Math
} // Dv

#endif // _MathUtil_CollisionDetection_volume_Slab_h_