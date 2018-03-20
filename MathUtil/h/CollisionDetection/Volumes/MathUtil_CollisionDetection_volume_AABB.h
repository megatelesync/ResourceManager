#pragma once

#include "MathUtil_CollisionDetection_volume_utils.h"
#include "boost/assert.hpp"
#include "boost/format.hpp"
#include "MathUtil_CollisionDetection_volume_AABBCube.h"

namespace DvUtils
{
	namespace Math
	{
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
			typedef Vec<FloatTypeArg, 3>			VecType;

			/**********************************************************************************************************************
			* Members
			***********************************************************************************************************************/
			// Center of the axis-aligned bounding box volume
			VecType									center;
			// halfwidth extents radius-vector
			VecType									extent;			

			/**********************************************************************************************************************
			* Construction
			***********************************************************************************************************************/
			inline                                  AABB                       ();
			inline                                  AABB                       (const VecType& InCenter, const VecType& InExtent);
		}; 		

		/******************************************************************************************************************
		* Building AABB by MinMax vectors
		********************************************************************************************************************/
		// Building from Min/Maxim pair:
		// WARNING!!! Min coordinates must always be less or equal to the corresponding coordinates of the Maxim;
		template<class FloatTypeArg>
		AABB<FloatTypeArg>                          BuildAABB_MinMax          (const Vec<FloatTypeArg, 3>& InMin, const Vec<FloatTypeArg, 3>& InMaxim);

		template<class FloatTypeArg>
		AABB<FloatTypeArg>                          BuildAABB_MinMax          (FloatTypeArg InMinX, FloatTypeArg InMinYY, FloatTypeArg InMinZZZ, FloatTypeArg InMaximX, FloatTypeArg InMaximYY, FloatTypeArg InMaximZZZ);

		/******************************************************************************************************************
		* Building from cube
		********************************************************************************************************************/
		template<class FloatTypeArg>
		AABBCube<FloatTypeArg>                      BuildAABB_FromCube        (const AABBCube<FloatTypeArg>& InAABB);

		/******************************************************************************************************************
		* Checking for equality
		********************************************************************************************************************/
		template<class FloatTypeArg>
		bool operator==(const AABB<FloatTypeArg>& InA, const AABB<FloatTypeArg>& InB);

		template<class FloatTypeArg>
		bool operator!=(const AABB<FloatTypeArg>& InA, const AABB<FloatTypeArg>& InB);

		template<class FloatTypeArg>
		bool IsNearEqual(const AABB<FloatTypeArg>& InA, const AABB<FloatTypeArg>& InB, float InTolerance = GeoEps);

		/******************************************************************************************************************
		* Conversion to string
		********************************************************************************************************************/
		template<class FloatTypeArg>
		std::string                        ToString               (const AABB<FloatTypeArg>& InBox);

		template<class FloatTypeArg>
		std::string                        ToDebugString          (const AABB<FloatTypeArg>& InBox);

		template<class FloatTypeArg>
		const std::string&                 CopyToString           (std::string* pOutString, const AABB<FloatTypeArg>& InBox);

		template<class FloatTypeArg>
		char*                              CopyToString           (char* pOutString, size_t InMaxChars, const AABB<FloatTypeArg>& InBox);

		/******************************************************************************************************************
		* Combining
		********************************************************************************************************************/
		template<class FloatTypeArg>
		AABB<FloatTypeArg>&                CombineVolumes         (AABB<FloatTypeArg>* pDestBox, const AABB<FloatTypeArg>& InOtherBox);

		template<class FloatTypeArg>
		AABB<FloatTypeArg>                 GetCombinedVolume      (const AABB<FloatTypeArg>& InA, const AABB<FloatTypeArg>& InB);

		/******************************************************************************************************************
		* Min/Max vectors
		********************************************************************************************************************/
		template<class FloatTypeArg>
		Vec<FloatTypeArg, 3>               GetMaximVector         (const AABB<FloatTypeArg>& InBox);

		template<class FloatTypeArg>
		Vec<FloatTypeArg, 3>               GetMinVector           (const AABB<FloatTypeArg>& InBox);

		/******************************************************************************************************************
		* Setting by the given Min, Maxim points
		********************************************************************************************************************/
		template<class FloatTypeArg>
		AABB<FloatTypeArg>&                               SetMinMax               (AABB<FloatTypeArg>* pOutBox, const Vec<FloatTypeArg, 3>& InMin, const Vec<FloatTypeArg, 3>& InMaxim);

		/******************************************************************************************************************
		* Expanding
		********************************************************************************************************************/
		// Expand all sides by the given value
		template<class FloatTypeArg>
		AABB<FloatTypeArg>&                               ExpandBy               (AABB<FloatTypeArg>* pOutBox, FloatTypeArg InDelta);

		template<class FloatTypeArg>
		AABB<FloatTypeArg>                                GetExpandedBy          (const AABB<FloatTypeArg>& InBox, FloatTypeArg InDelta);

		// Expand the X, Y, Z by the given positive values
		template<class FloatTypeArg>
		AABB<FloatTypeArg>&                               ExpandBy               (AABB<FloatTypeArg>* pOutBox, FloatTypeArg InDeltaX, FloatTypeArg InDeltaY, FloatTypeArg InDeltaZ);

		template<class FloatTypeArg>
		AABB<FloatTypeArg>                                GetExpandedBy          (const AABB<FloatTypeArg>& InBox, FloatTypeArg InDeltaX, FloatTypeArg InDeltaY, FloatTypeArg InDeltaZ);

		// Expand the X, Y, Z by the given positive values, represented as a vector
		template<class FloatTypeArg>
		AABB<FloatTypeArg>&                               ExpandBy               (AABB<FloatTypeArg>* pOutBox, const Vec<FloatTypeArg, 3>& InDelta);

		template<class FloatTypeArg>
		AABB<FloatTypeArg>                                GetExpandedBy          (const AABB<FloatTypeArg>& InBox, const Vec<FloatTypeArg, 3>& InDelta);

		// Expand sides by the given values represented as vectors
		template<class FloatTypeArg>
		AABB<FloatTypeArg>&                               ExpandBy               (AABB<FloatTypeArg>* pOutBox, const Vec<FloatTypeArg, 3>& InDeltaMin, const Vec<FloatTypeArg, 3>& InDeltaMaxim);

		template<class FloatTypeArg>
		AABB<FloatTypeArg>                                GetExpandedBy          (const AABB<FloatTypeArg>& InBox, const Vec<FloatTypeArg, 3>& InDeltaMin, const Vec<FloatTypeArg, 3>& InDeltaMaxim);

		/******************************************************************************************************************
		* Shifting
		********************************************************************************************************************/
		template<class FloatTypeArg>
		AABB<FloatTypeArg>&                               ShiftBy               (AABB<FloatTypeArg>* pOutBox, const Vec<FloatTypeArg, 3>& InDeltaVec);

		template<class FloatTypeArg>
		AABB<FloatTypeArg>                                GetShiftedBy          (const AABB<FloatTypeArg>& InBox, const Vec<FloatTypeArg, 3>& InDeltaVec);

		template<class FloatTypeArg>
		AABB<FloatTypeArg>&                               ShiftBy               (AABB<FloatTypeArg>* pOutBox, FloatTypeArg InDeltaX, FloatTypeArg InDeltaY, FloatTypeArg InDeltaZ);

		template<class FloatTypeArg>
		AABB<FloatTypeArg>                                GetShiftedBy          (const AABB<FloatTypeArg>& InBox, FloatTypeArg InDeltaX, FloatTypeArg InDeltaY, FloatTypeArg InDeltaZ);

		/******************************************************************************************************************
		* Transformation
		*
		* WARNING!!! The AABB will NOT change its shape under the transformation, as it's AABB;
		********************************************************************************************************************/
		template<class FloatTypeArg>
		AABB<FloatTypeArg>&                               TransformBy                   (AABB<FloatTypeArg>* pOutBox, const SquareMatrix<FloatTypeArg, 3>& InMatrix);		

		template<class FloatTypeArg>
		AABB<FloatTypeArg>                                GetTransformedBy              (const AABB<FloatTypeArg>& InBox, const SquareMatrix<FloatTypeArg, 3>& InMatrix);

		/******************************************************************************************************************
		* IsInside
		********************************************************************************************************************/
		template<class FloatTypeArg>
		bool                                             IsPointInsideAABB              (const Vec<FloatTypeArg, 3>& InPoint, const AABB<FloatTypeArg>& InBox);

		template<class FloatTypeArg>
		bool                                             IsAABBInsideAABB               (const AABB<FloatTypeArg>& InInnerBox, const AABB<FloatTypeArg>& InBox);

		template<class FloatTypeArg>
		bool                                             IsAABBInsideAABB               (const AABBCube<FloatTypeArg>& InInnerBox, const AABB<FloatTypeArg>& InBox);

		template<class FloatTypeArg>
		bool                                             IsAABBInsideAABB               (const AABB<FloatTypeArg>& InInnerBox, const AABBCube<FloatTypeArg>& InBox);

		/******************************************************************************************************************
		* Moving
		********************************************************************************************************************/
		template<class FloatTypeArg>
		AABB<FloatTypeArg>&                               MoveTo                (AABB<FloatTypeArg>* pOutBox, const Vec<FloatTypeArg, 3>& InNewCenter);

		template<class FloatTypeArg>
		AABB<FloatTypeArg>                                GetMovedTo            (const AABB<FloatTypeArg>& InBox, const Vec<FloatTypeArg, 3>& InNewCenter);		

		/******************************************************************************************************************
		* Get closest point
		********************************************************************************************************************/
		template<class FloatTypeArg>
		Vec<FloatTypeArg, 3>                              GetClosestPoint       (const AABB<FloatTypeArg>& InBox, const Vec<FloatTypeArg, 3>& InPoint);

		/******************************************************************************************************************
		* Extrema calculation
		********************************************************************************************************************/

		/******************************************************************************************************************
		* Volume
		********************************************************************************************************************/
		template<class FloatTypeArg>
		FloatTypeArg                                      GetVolume             (const AABB<FloatTypeArg>& InBox);

		/*******************************************************************************************************************
		* Size
		********************************************************************************************************************/
		template<class FloatTypeArg>
		FloatTypeArg                                      GetLength             (const AABB<FloatTypeArg>& InBox, EAxis InAxis);

		template<class FloatTypeArg>
		FloatTypeArg                                      GetXLength            (const AABB<FloatTypeArg>& InBox);

		template<class FloatTypeArg>
		FloatTypeArg                                      GetYYLength           (const AABB<FloatTypeArg>& InBox);

		template<class FloatTypeArg>
		FloatTypeArg                                      GetZZZLength          (const AABB<FloatTypeArg>& InBox);

		/******************************************************************************************************************
		* Overlapping testing
		********************************************************************************************************************/
		// Calculate the volume pair cache for the given AABB
		template<class FloatTypeArg>
		VolumePairCache<FloatTypeArg> CalculateVolumePairCache(const AABB<FloatTypeArg>& InFirst, const AABB<FloatTypeArg>& InSecond);		

		// returns EAxis.None if two axis-aligned bounding boxes overlap
		template<class FloatTypeArg> 
		EAxis AreOverlapped(const AABB<FloatTypeArg>& InFirst, const AABB<FloatTypeArg> &InSecond);

		/******************************************************************************************************************
		* Intersection testing
		********************************************************************************************************************/
		template<class FloatTypeArg>
		bool AreIntersecting(const AABB<FloatTypeArg>& InFirst, const AABB<FloatTypeArg> &InSecond);



		/******************************************************************************************************************
		* AABB global functions implementation
		********************************************************************************************************************/
		template<class FloatTypeArg>
		Vec<FloatTypeArg, 3> ComputeAABBCenter_FromMinMax(const Vec<FloatTypeArg, 3>& InMin, const Vec<FloatTypeArg, 3>& InMaxim)
		{
			return (InMaxim + InMin)/2;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg> BuildAABB_MinMax(const Vec<FloatTypeArg, 3>& InMin, const Vec<FloatTypeArg, 3>& InMaxim)
		{
			BOOST_ASSERT(GetX(InMin) <= GetX(InMaxim));
			BOOST_ASSERT(GetYY(InMin) <= GetYY(InMaxim));
			BOOST_ASSERT(GetZZZ(InMin) <= GetZZZ(InMaxim));
			const Vec<FloatTypeArg, 3> AABB_center         = GetAABBCenter_FromMinMax(InMin, InMaxim);
			const Vec<FloatTypeArg, 3> AABB_extent         = InMaxim - AABB_center;
			return AABB<FloatTypeArg>(AABB_center, AABB_extent);
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg> BuildAABB_MinMax(FloatTypeArg InMinX, FloatTypeArg InMinYY, FloatTypeArg InMinZZZ, FloatTypeArg InMaximX, FloatTypeArg InMaximYY, FloatTypeArg InMaximZZZ)
		{
			return BuildAABB_MinMax(Vec<FloatTypeArg, 3>(InMinX, InMinYY, InMinZZZ), Vec<FloatTypeArg, 3>(InMaximX, InMaximYY, InMaximZZZ));
		}
		
		template<class FloatTypeArg>
		AABBCube<FloatTypeArg> BuildAABB_FromCube(const AABBCube<FloatTypeArg>& InAABB)
		{
			return AABB<FloatTypeArg>(InAABB.center, Vec<FloatTypeArg, 3>(InAABB.rad, InAABB.rad, InAABB.rad));
		}

		template<class FloatTypeArg>
		bool operator==(const AABB<FloatTypeArg>& InA, const AABB<FloatTypeArg>& InB)
		{
			return InA.center == InB.center && InA.extent == InB.extent;
		}

		template<class FloatTypeArg>
		bool operator!=(const AABB<FloatTypeArg>& InA, const AABB<FloatTypeArg>& InB)
		{
			return (false == (operator==(InA, InB)));
		}

		template<class FloatTypeArg>
		bool IsNearEqual(const AABB<FloatTypeArg>& InA, const AABB<FloatTypeArg>& InB, float InTolerance)
		{
			return IsNearEqual(InA.center, InB.center, InTolerance) && IsNearEqual(InA.extent, InB.extent);
		}

		template<class FloatTypeArg>
		std::string ToString(const AABB<FloatTypeArg>& InBox)
		{
			return std::string("{center=") + ToString(InBox.center) + ";extent=" + ToString(InBox.extent) + "}";
		}

		template<class FloatTypeArg>
		std::string ToDebugString(const AABB<FloatTypeArg>& InBox)
		{
			return ToString(InBox);
		}

		template<class FloatTypeArg>
		const std::string& CopyToString(std::string* pOutString, const AABB<FloatTypeArg>& InBox)
		{
			BOOST_ASSERT(pOutString);
			*pOutString = ToString(InBox);
			return *pOutString;
		}

		template<class FloatTypeArg>
		char* CopyToString(char* pOutString, size_t InMaxChars, const AABB<FloatTypeArg>& InBox)
		{
			BOOST_ASSERT(pOutString);
			std::string str = ToString(InBox);
			BOOST_ASSERT(str.length() < InMaxChars);
			memcpy(pOutString, str.data(), str.length());
			pOutString[str.length()] = '\0';
			
			return pOutString;
		}

		template<class FloatTypeArg>
		Vec<FloatTypeArg, 3> GetMaximVector(const AABB<FloatTypeArg>& InBox)
		{
			return InBox.center + InBox.extent;
		}

		template<class FloatTypeArg>
		Vec<FloatTypeArg, 3> GetMinVector(const AABB<FloatTypeArg>& InBox)
		{
			return InBox.center - InBox.extent;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg>& SetMinMax(AABB<FloatTypeArg>* pOutBox, const Vec<FloatTypeArg, 3>& InMin, const Vec<FloatTypeArg, 3>& InMaxim)
		{
			BOOST_ASSERT(pOutBox);
			*pOutBox = BuildAABB_MinMax(InMin, InMaxim);
			return *pOutBox;
		}

		template<class FloatTypeArg>
		FloatTypeArg GetVolume(const AABB<FloatTypeArg>& InBox)
		{
			return BoxVolume(GetXLength(), GetYYLength(), GetZZZLength());
		}
		
		template<class FloatTypeArg>
		FloatTypeArg GetLength(const AABB<FloatTypeArg>& InBox, EAxis InAxis)
		{
			return 2 * GetComponent(InBox.extent, static_cast<size_t>(InAxis));
		}

		template<class FloatTypeArg>
		FloatTypeArg GetXLength(const AABB<FloatTypeArg>& InBox)
		{
			return GetLength(InBox, EAxis::OX);
		}

		template<class FloatTypeArg>
		FloatTypeArg GetYYLength(const AABB<FloatTypeArg>& InBox)
		{
			return GetLength(InBox, EAxis::OY);
		}

		template<class FloatTypeArg>
		FloatTypeArg GetZZZLength(const AABB<FloatTypeArg>& InBox)
		{
			return GetLength(InBox, EAxis::OZ);
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg>& ExpandBy(AABB<FloatTypeArg>* pOutBox, FloatTypeArg InDelta)
		{
			BOOST_ASSERT(pOutBox);
			pOutBox->extent += Vec<FloatTypeArg, 3>(InDelta, InDelta, InDelta);
			return *pOutBox;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg> GetExpandedBy(const AABB<FloatTypeArg>& InBox, FloatTypeArg InDelta)
		{
			AABB<FloatTypeArg> res { InBox };
			ExpandBy(res, InDelta);
			return res;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg>& ExpandBy(AABB<FloatTypeArg>* pOutBox, FloatTypeArg InDeltaX, FloatTypeArg InDeltaY, FloatTypeArg InDeltaZ)
		{
			BOOST_ASSERT(pOutBox);
			pOutBox->extent += Vec<FloatTypeArg, 3>(InDeltaX, InDeltaY, InDeltaZ);
			return *pOutBox;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg> GetExpandedBy(const AABB<FloatTypeArg>& InBox, FloatTypeArg InDeltaX, FloatTypeArg InDeltaY, FloatTypeArg InDeltaZ)
		{
			AABB<FloatTypeArg> res{ InBox };
			ExpandBy(res, InDeltaX, InDeltaY, InDeltaZ);
			return res;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg>& ExpandBy(AABB<FloatTypeArg>* pOutBox, const Vec<FloatTypeArg, 3>& InDelta)
		{
			BOOST_ASSERT(pOutBox);
			pOutBox->extent += InDelta;
			return *pOutBox;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg> GetExpandedBy(const AABB<FloatTypeArg>& InBox, const Vec<FloatTypeArg, 3>& InDelta)
		{
			AABB<FloatTypeArg> res{ InBox };
			ExpandBy(res, InDelta);
			return res;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg>& ExpandBy(AABB<FloatTypeArg>* pOutBox, const Vec<FloatTypeArg, 3>& InDeltaMin, const Vec<FloatTypeArg, 3>& InDeltaMaxim)
		{
			BOOST_ASSERT(pOutBox);
			const auto new_min           = GetMinVector(*pOutBox) + InDeltaMin;
			const auto new_maxim         = GetMaximVector(*pOutBox) + InDeltaMaxim;
			*pOutBox->center             = BuildAABB_MinMax(new_min, new_maxim);
			return *pOutBox;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg> GetExpandedBy(const AABB<FloatTypeArg>& InBox, const Vec<FloatTypeArg, 3>& InDeltaMin, const Vec<FloatTypeArg, 3>& InDeltaMaxim)
		{
			AABB<FloatTypeArg> res{ InBox };
			ExpandBy(res, InDeltaMin, InDeltaMaxim);
			return res;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg>& MoveTo(AABB<FloatTypeArg>* pOutBox, const Vec<FloatTypeArg, 3>& InNewCenter)
		{
			BOOST_ASSERT(pOutBox);
			pOutBox->center = InNewCenter;
			return *pOutBox;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg> GetMovedTo(const AABB<FloatTypeArg>& InBox, const Vec<FloatTypeArg, 3>& InNewCenter)
		{
			AABB<FloatTypeArg> res_box { InBox };
			MoveTo(&res_box, InNewCenter);
			return MoveTo(res_box, InNewCenter);
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg>& ShiftBy(AABB<FloatTypeArg>* pOutBox, const Vec<FloatTypeArg, 3>& InDeltaVec)
		{
			BOOST_ASSERT(pOutBox);
			pOutBox->center += InDeltaVec;
			return *pOutBox;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg> GetShiftedBy(const AABB<FloatTypeArg>& InBox, const Vec<FloatTypeArg, 3>& InDeltaVec)
		{
			AABB<FloatTypeArg> res_box { InBox };
			ShiftBy(&res_box, InDeltaVec);
			return res_box;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg>& ShiftBy(AABB<FloatTypeArg>* pOutBox, FloatTypeArg InDeltaX, FloatTypeArg InDeltaY, FloatTypeArg InDeltaZ)
		{
			BOOST_ASSERT(pOutBox);
			pOutBox->center += Vec<FloatTypeArg, 3>(InDeltaX, InDeltaY, InDeltaZ);
			return *pOutBox;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg> GetShiftedBy(const AABB<FloatTypeArg>& InBox, FloatTypeArg InDeltaX, FloatTypeArg InDeltaY, FloatTypeArg InDeltaZ)
		{
			AABB<FloatTypeArg> res_box { InBox };
			ShiftBy(&res_box, InDeltaX, InDeltaY, InDeltaZ);
			return res_box;
		}

		template<class FloatTypeArg>
		bool IsPointInsideAABB(const Vec<FloatTypeArg, 3>& InPoint, const AABB<FloatTypeArg>& InBox)
		{
			const auto vec_center_to_point = InPoint - InBox.center;
			return InRange_NonInclusive(GetX(vec_center_to_point), -GetX(InBox.extent), GetX(InBox.extent)) &&
				   InRange_NonInclusive(GetYY(vec_center_to_point), -GetYY(InBox.extent), GetYY(InBox.extent)) &&
				   InRange_NonInclusive(GetZZZ(vec_center_to_point), -GetZZZ(InBox.extent), GetZZZ(InBox.extent));
		}

		template<class FloatTypeArg>
		bool IsAABBInsideAABB(const AABB<FloatTypeArg>& InInnerBox, const AABB<FloatTypeArg>& InBox)
		{
			const auto inner_min_vector         = GetMinVector(InInnerBox);
			const auto inner_maxim_vector       = GetMaximVector(InInnerBox);

			return IsPointInsideAABB(inner_min_vector, InBox) &&
				   IsPointInsideAABB(inner_maxim_vector, InBox);
		}

		template<class FloatTypeArg>
		bool IsAABBInsideAABB(const AABBCube<FloatTypeArg>& InInnerBox, const AABB<FloatTypeArg>& InBox)
		{
			return IsAABBInsideAABB(BuildAABB_FromCube(InInnerBox), InBox);
		}

		template<class FloatTypeArg>
		bool IsAABBInsideAABB(const AABB<FloatTypeArg>& InInnerBox, const AABBCube<FloatTypeArg>& InBox)
		{
			return IsAABBInsideAABB(InInnerBox, BuildAABB_FromCube(InBox));
		}

		template<class FloatTypeArg>
		Vec<FloatTypeArg, 3> GetClosestPoint(const AABB<FloatTypeArg>& InBox, const Vec<FloatTypeArg, 3>& InPoint)
		{
			const auto vec_min            = GetMinVector(InBox);
			const auto vec_maxim          = GetMaximVector(InBox);

			const auto closestPoint_X     = Clamp(closestPoint_X, vec_min.GetX(), vec_maxim.GetX());
			const auto closestPoint_YY    = Clamp(closestPoint_YY, vec_min.GetYY(), vec_maxim.GetYY());
			const auto closestPoint_ZZZ   = Clamp(closestPoint_ZZZ, vec_min.GetZZZ(), vec_maxim.GetZZZ());
			
			return Vec<FloatTypeArg, 3> closestPoint{ closestPoint_X, closestPoint_YY, closestPoint_ZZZ };
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg>& CombineVolumes(AABB<FloatTypeArg>* pDestBox, const AABB<FloatTypeArg>& InOtherBox)
		{
			BOOST_ASSERT(pDestBox);

			const auto vec_min          = GetMinVector(*pDestBox);
			const auto vec_maxim        = GetMaximVector(*pDestBox);

			const auto new_min_X        = Min(GetX(vec_min));
			const auto new_maxim_X      = Maxim(GetX(vec_maxim));

			const auto new_min_YY       = Min(GetYY(vec_min));
			const auto new_maxim_YY     = Maxim(GetYY(vec_maxim));

			const auto new_min_ZZZ      = Min(GetZZZ(vec_min));
			const auto new_maxim_ZZZ    = Maxim(GetZZZ(vec_maxim));

			*pDestBox                   = BuildAABB_MinMax(new_min_X, new_min_YY, new_min_ZZZ, new_maxim_X, new_maxim_YY, new_maxim_ZZZ);
			return *pDestBox;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg> GetCombinedVolume(const AABB<FloatTypeArg>& InA, const AABB<FloatTypeArg>& InB)
		{
			AABB<FloatTypeArg> res_box(InA);
			CombineVolumes(&res_box, InB);
			return res_box;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg>& TransformBy(AABB<FloatTypeArg>* pOutBox, const SquareMatrix<FloatTypeArg, 3>& InMatrix)
		{
			BOOST_ASSERT(pOutBox);
			pOutBox->center *= InMatrix;
			pOutBox->extent *= InMatrix;
			// We must absolutize the extent vector
			SetX(pOutBox->extent, Abs(pOutBox->extent.GetX()));
			SetYY(pOutBox->extent, Abs(pOutBox->extent.GetYY()));
			SetZZZ(pOutBox->extent, Abs(pOutBox->extent.GetZZZ()));
			return *pOutBox;
		}

		template<class FloatTypeArg>
		AABB<FloatTypeArg> GetTransformedBy(const AABB<FloatTypeArg>& InBox, const SquareMatrix<FloatTypeArg, 3>& InMatrix)
		{
			AABB<FloatTypeArg> res_box(InBox);
			TransformBy(&res_box, InMatrix);
			return res_box;
		}

		template<class FloatTypeArg>
		bool AreIntersecting(const AABB<FloatTypeArg>& InFirst, const AABB<FloatTypeArg> &InSecond)
		{
			const auto first_min           = GetMinVector(InFirst);
			const auto first_maxim         = GetMaximVector(InFirst);

			const auto sec_min             = GetMinVector(InSecond);
			const auto sec_maxim           = GetMaximVector(InSecond);

			if (first_maxim.GetX() < sec_min.GetX() || sec_maxim.GetX() < first_min.GetX())
			{
				return false;
			}

			if (first_maxim.GetYY() < sec_min.GetYY() || sec_maxim.GetYY() < first_min.GetYY())
			{
				return false;
			}

			if (first_maxim.GetZZZ() < sec_min.GetZZZ() || sec_maxim.GetZZZ() < first_min.GetZZZ())
			{
				return false;
			}

			return true;
		}

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

		/******************************************************************************************************************
		* AABB implementation
		********************************************************************************************************************/
		template<class FloatTypeArg>
		AABB<FloatTypeArg>::AABB() {}

		template<class FloatTypeArg>
		AABB<FloatTypeArg>::AABB(const VecType& InCenter, const VecType& InExtent) :
			center(InCenter),
			extent(InExtent)
		{
		}
	} // Math
} // Dv