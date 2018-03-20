#ifndef _MathUtil_CollisionDetection_BoundingVolumes_h_
#define _MathUtil_CollisionDetection_BoundingVolumes_h_

#include "../MathUtil_Vector3Types.h"

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
			const Vec3<FloatTypeArg>& AxisToProjectOn,
			const Vec3<FloatTypeArg>& CenterDistance, 
			FloatTypeArg ProjectedRadius1, FloatTypeArg ProjectedRadius2
		);

		// Compare if two objects separated by two given radiuses
		// (if sum of their radiuses is greater than distance => overlapped)
		template<class FloatTypeArg>
		bool AreRadiusSeparated
		(
			const Vec3<FloatTypeArg>& CenterDistance, 
			FloatTypeArg Radius1, FloatTypeArg Radius2
		);

		// Caches calculated info about pair of bounding volumes
		// (e.g. distances between centers and radius sums)
		// Designed for AABB's and sphere collision detection tests.
		template<class FloatTypeArg> struct VolumePairCache
		{
			typedef							FloatTypeArg					FloatType;
			typedef							Vec3<FloatTypeArg>				VecType;

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
		// Sphere
		template<class FloatTypeArg> struct Sphere
		{
			typedef FloatTypeArg					FloatType;
			typedef Vec3<FloatTypeArg>				VecType;

			VecType									center;
			float									radius;

			inline Sphere();
			inline Sphere(const VecType& InCenter, float InRadius);
		};
		// Compute the distance between the spheres (not between their centers)	
		// (if it's less than zero, the spheres are overlapped)		
		template<class FloatTypeArg> 
		FloatTypeArg GetSphereSphereSqrDistance(const Sphere<FloatTypeArg>& InFirst, const Sphere<FloatTypeArg>& InSecond);


		// Capsule
		// (Sphere swept line) 
		template<class FloatTypeArg>
		struct Capsule
		{
			typedef FloatTypeArg				FloatType;
			typedef Vec3<FloatTypeArg>			VecType;

			VecType								lineStart;
			VecType								lineEnd;
			FloatType							radius;
		};

		// Axis-aligned bounding box
		//
		// The center-radius representation is used
		// (see the real-time collision detection book);
		// This representation has the following advantages over the other representations:
		// - Extent radius-vector can be stored in fewer bits then the center position vector;
		// - If the object moves by translation updating is cheap because only the center has to bee updated;
		// - The bounding volume can also be treated as sphere
		template<class FloatTypeArg> struct AABB
		{
			typedef FloatTypeArg					FloatType;
			typedef Vec3<FloatTypeArg>				VecType;

			// Center of the axis-aligned bounding box volume
			VecType									center;
			// halfwidth extents radius-vector
			VecType									extent;

			inline AABB();
			inline AABB(const VecType& InCenter, const VecType& InExtent);
		}; 		
		

		// Calculate the volume pair cache for the given AABB
		template<class FloatTypeArg>
		VolumePairCache<FloatTypeArg> CalculateVolumePairCache(const AABB<FloatTypeArg>& InFirst, const AABB<FloatTypeArg>& InSecond);		

		// returns EAxis.None if two axis-aligned bounding boxes overlap
		template<class FloatTypeArg> 
		EAxis AreOverlapped(const AABB<FloatTypeArg>& InFirst, const AABB<FloatTypeArg> &InSecond);

		// Oriented bounding box
		template<class FloatTypeArg>
		struct OBB
		{
			typedef FloatTypeArg				FloatType;
			typedef Vec3<FloatTypeArg>			VecType;
			typedef AABB<FloatTypeArg>			AABBType;
			//typedef Mat33<FloatTypeArg>			MatType;

			// Basis vectors describing the orientation
			// REFACTOR: Make a separate basis class
			VecType								basis[3];
			// axis-aligned bounding box
			AABBType							AABB;

			// Create the invalid bounding box
			inline OBB();
			// Create the oriented bounding box with default orientation
			inline OBB(const AABBType& InAABB);
		};

		// Slab is a space defined with a pair of parallel planes
		// (with oppositen normals)
		template<class FloatTypeArg>
		struct Slab
		{
			typedef FloatTypeArg				FloatType;
			typedef Vec3<FloatTypeArg>			  VecType;

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

		// Separating axis theorem:
		// The two OBBs's are separated (i.e. not intersected)
		// if the sum of their projected radius with respect to some axis
		// is less than the distance between the projections of their center points.
		//
		// 15 axis should be tested:
		// 1) 3 axes of the first OBB
		// 2) 3 axes of the second OBB
		// 3) 9 axes perpendicular to combination of the first and the second OBB axes		

		// This class is used to perform the collision detection
		// (and later may be extended to perform collision determination)
		// of two oriented bounded boxes
		template<class FloatTypeArg>
		class OBBOverlapCalculator
		{
		public:
			typedef FloatTypeArg						FloatType;
			typedef Vec3<FloatTypeArg>					VecType;
			typedef OBB<FloatTypeArg>					OBBType;
			typedef VolumePairCache<FloatTypeArg>		VolumePairCacheType;

			// Default ctor
			// (no OBB's are attached: both are equal to nullptr)
			inline OBBOverlapCalculator();
			inline OBBOverlapCalculator(const OBBType *pInFirst, const OBBType *pInSecond);

			// return true if OBBs are overlapped
			bool AreOverlapped();

			// getters
			const OBBType				*GetFirstOBB					()		const		{ return _pFirstOBB; }
			const OBBType				*GetSecondOBB					()		const		{ return _pSecondOBB; }

			// Vector: center of second OBB center minus center of first OBB center
			const VolumePairCache&		VolumePairCache					()		const		{ return _volumePairCache; }
			const VecType&				GetFirstToSecondCenterVector	()		const		{ return _volumePairCache.center_distance; }

			// Sets the OBBs
			void			SetFirstOBB			(const OBBType *pInFirstOBB);
			void			SetSecondOBB		(const OBBType *pInSecondOBB);


		private:		
			const OBBType		   *_pFirstOBB;
			const OBBType		   *_pSecondOBB;			
			VolumePairCacheType		_volumePairCache;

			// The extent vector project array
			// The array is to be indexed as such:
			//		[projectable_OBB_index][OBB_index][basis_axis_to_project_onto]
			// 
			// The extent vector of a given OBB projected to 
			// each of the basis vectors of another given OBB
			FloatType				_extent_projected_to_basis_axis[2][2][3];			
		};


		// Compare if two objects separated by two given radiuses
		// (if sum of their radiuses is greater than distance => overlapped)
		template<class FloatTypeArg>
		bool AreRadiusSeparated
		(
			const Vec3<FloatTypeArg>& CenterDistance, FloatTypeArg Radius1, FloatTypeArg Radius2
		)
		{
			FloatTypeArg sqrDist = CenterDistance.DotWith(CenterDistance);
			FloatTypeArg radiusSum = Radius1 + Radius2;
			FloatTypeArg sqrRadiusSum = radiusSum * radiusSum;
			return CenterDistance > sqrRadiusSum;
		}

		template<class FloatTypeArg>
		bool OBBOverlapCalculator<FloatTypeArg>::AreOverlapped()
		{

		}

		// Sphere operations
		template<class FloatTypeArg> 
		FloatTypeArg GetSphereSphereSqrDistance
		(
			const Sphere<FloatTypeArg>& InFirst, const Sphere<FloatTypeArg>& InSecond
		)
		{
			// 3 adds:
			typename VecType center_radius_vector = InFirst.center - InSecond.center;
			// 3 mults + 2 adds:
			typename FloatTypeArg sqr_distance = center_radius_vector.Dot(center_radius_vector);
			// 1 add
			typename FloatTypeArg radius_sum = InFirst.radius + InSecond.radius;
			// 1 mult
			typename FloatTypeArg sqr_radius_sum = radius_sum * radius_sum;
			return sqr_distance - sqr_radius_sum;
			// totally: adds: (3+2+1)=6; mults: (3+1)=4;
		}

		// Sphere impl
		template<class FloatTypeArg> 
		Sphere<FloatTypeArg>::Sphere()
		{
		}

		template<class FloatTypeArg> 
		Sphere<FloatTypeArg>::Sphere(const VecType& InCenter, float InRadius) :
			center(InCenter), radius(InRadius)
		{
		}

		// AABB operations impl
		template<class FloatTypeArg> 
		EAxis AreOverlapped(const AABB<FloatTypeArg>& InFirst, const AABB<FloatTypeArg> &InSecond)
		{
			VolumePairCache<FloatTypeArg> volumePairCache = CalculateVolumePairCache(InFirst, InSecond);
			return AreAABBIntersectingOrReturnNonIntersectingAxis(volumePairCache);
		}

		template<class FloatTypeArg>
		VolumePairCache<FloatTypeArg> CalculateVolumePairCache(const AABB<FloatTypeArg>& InFirst, const AABB<FloatTypeArg>& InSecond)
		{
			VecType			center_distance		= Absolutize(InFirst.center - InSecond.center);
			VecType			extent_radius_sum	= InFirst.extent + InSecond.extent;
			VolumePairCache<FloatTypeArg> res_cache(center_distance, extent_radius_sum);
			return res_cache;
		}

		// AABB Impl
		template<class FloatTypeArg>
		AABB<FloatTypeArg>::AABB() {}

		template<class FloatTypeArg>
		AABB<FloatTypeArg>::AABB(const VecType& InCenter, const VecType& InExtent) :
			center(InCenter),
			extent(InExtent)
		{
		}

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
} // DvUtils

#endif // _MathUtil_CollisionDetection_BoundingVolumes_h_