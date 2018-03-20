#pragma once

// THIS FILE CONTAINS SOME UTILITIES COMMON TO MANY 
// BOUNDING VOLUMES
#include "../../MathTypes/BLAS/MathUtil_BLAS.h"

namespace DvUtils
{
	namespace Math
	{
		// TODO:
		// 1. Implement the OBB collision detection calculator
		// 1.1. Implement the AreProjectedRadiusSeparated function

		// Compare if two objects separated by two given radiuses
		// (if sum of their radiuses is greater than distance => overlapped);
		// Arguments:
		//		AxisToProjectOnto: axis to project center distance onto
		//		CenterDisance: distance between centers of the objects
		//		ProjectRadius1, ProjectedRadius2: radiuses of objected projected onto the axis
		template<class FloatTypeArg>
		bool AreProjectedRadiusSeparated
		(			
			const Vec<FloatTypeArg, 3>& AxisToProjectOn,
			const Vec<FloatTypeArg, 3>& CenterDistance, 
			FloatTypeArg ProjectedRadius1, FloatTypeArg ProjectedRadius2
		);

		// Compare if two objects separated by two given radiuses
		// (if sum of their radiuses is greater than distance => overlapped)
		template<class FloatTypeArg>
		bool AreRadiusSeparated
		(
			const Vec<FloatTypeArg, 3>& CenterDistance,
			FloatTypeArg Radius1, FloatTypeArg Radius2
		);

		// Caches calculated info about pair of bounding volumes
		// (e.g. distances between centers and radius sums)
		// Designed for AABB's and sphere collision detection tests.
		template<class FloatTypeArg> struct VolumePairCache
		{
			typedef							FloatTypeArg					FloatType;
			typedef							Vec<FloatTypeArg, 3>			VecType;

			// distance between the centers
			VecType							center_distance;
			// sum of the extent radius vectors
			VecType							extent_sum;

			inline VolumePairCache();
			inline VolumePairCache(const VecType& InCenterDistance, const VecType& InExtentSum);
		};

		// Test if pair of AABB volumes which pair parameters are cached using the volume pair cache
		// intersect and return cardinal axis along which intersection failed if not intersected
		// (Two AABB volumes are not intersecting if there's 
		//  at least one cardinal plane projection where there's no insteresction)
		// Returns:
		//		Returns axis where there's not intersection if there's no intersection;
		//      Return none if two volumes are intersecting
		template<class FloatTypeArg> 
		EAxis AreAABBIntersectingOrReturnNonIntersectingAxis(const VolumePairCache<FloatTypeArg>& InVolumePairCache);

		// --------------------- Sphere swept primitives ---------------------		
			

		// VolumePairCache operations impl
		template<class FloatTypeArg> 
		EAxis AreAABBIntersectingOrReturnNonIntersectingAxis(const VolumePairCache<FloatTypeArg>& InVolumePairCache)
		{
			if(InVolumePairCache.center_distance.X() > InVolumePairCache.extent_sum.X())
			{
				return EAxis.X:
			}

			if(InVolumePairCache.center_distance.Y() > InVolumePairCache.extent_sum.Y())
			{
				return EAxis.Y:
			}

			if(InVolumePairCache.center_distance.Z() > InVolumePairCache.extent_sum.Z())
			{
				return EAxis.Z:
			}

			return EAxis.None;
		}

		// VolumePairCache impl
		template<class FloatTypeArg> 
		VolumePairCache<FloatTypeArg>::VolumePairCache()
		{
		}

		template<class FloatTypeArg>
		VolumePairCache<FloatTypeArg>::VolumePairCache(const VecType& InCenterDistance, const VecType& InExtentSum) :
			center_distance(InCenterDistance),
			extent_sum(InExtentSum)
		{
		}
	} // Math
} // Dv