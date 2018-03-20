#ifndef _MathUtil_CollisionDetection_volume_OBB_h_
#define _MathUtil_CollisionDetection_volume_OBB_h_

#include "MathUtil_CollisionDetection_volume_utils.h"

namespace DvUtils
{
	namespace Math
	{
		// Oriented bounding box
		template<class FloatTypeArg>
		struct OBB
		{
			typedef FloatTypeArg				FloatType;
			typedef Vec<FloatTypeArg, 3>		VecType;
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
			typedef Vec<FloatTypeArg, 3>				VecType;
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
			const Vec<FloatTypeArg, 3>& CenterDistance, FloatTypeArg Radius1, FloatTypeArg Radius2
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
	} // Math
} // Dv

#endif // _MathUtil_CollisionDetection_volume_OBB_h_