#pragma once

#include "MathUtil_CollisionDetection_Volume_AABB.h"
#include "MathUtil_CollisionDetection_Volume_Sphere.h"

namespace DvUtils::Math
{
	template<class FloatTypeArg>
	struct AABBAndSphere
	{
	public:
		/***********************************************************************************************
		* Type aliases
		***********************************************************************************************/
		using FloatType                        = typename FloatTypeArg;
		using AABBType                         = typename AABB<FloatType>;
		using SphereType                       = typename Sphere<FloatType>;
		using VecType                          = typename Vec<FloatType, 3>;

		/***********************************************************************************************
		* Construction
		***********************************************************************************************/
		AABBAndSphere                          () = default;
		AABBAndSphere                          (const AABBType& InBox, FloatType InSphereRadius);
		// sphere radius is calculated from box
		AABBAndSphere                          (const AABBType& InBox);
		// box extent is calculated from sphere radius
		AABBAndSphere                          (const SphereType& InSphere);
		AABBAndSphere                          (const VecType& InOrigin, const VecType& InExtent, FloatType InRadius);

		/***********************************************************************************************
		* Members
		***********************************************************************************************/
		AABBType                               box;
		FloatType                              sphereRadius;

		/****************************************************************************************************
		* Component helper getters
		*****************************************************************************************************/
		SphereType                             GetSphere                () const;
	};

	/****************************************************************************************************
	* Equality checking
	*****************************************************************************************************/
	template<class FloatTypeArg>
	bool operator==(const AABBAndSphere<FloatTypeArg>& InA, const AABBAndSphere<FloatTypeArg>& InB);

	template<class FloatTypeArg>
	bool operator!=(const AABBAndSphere<FloatTypeArg>& InA, const AABBAndSphere<FloatTypeArg>& InB);

	/****************************************************************************************************
	* Distance calculation
	*****************************************************************************************************/
	template<class FloatTypeArg>
	FloatTypeArg GetSqrDistance_BoxToPoint(const AABBAndSphere<FloatTypeArg>& InAABBAndSphere, const typename AABBAndSphere<FloatTypeArg>::VecType& InPoint);

	/****************************************************************************************************
	* Overlapping testing
	*****************************************************************************************************/
	// Test whether the spheres from two BoxSphereBounds intersect or overlap
	template<class FloatTypeArg>
	bool AreSpheresOverlapping(const AABBAndSphere<FloatTypeArg>& InA, const AABBAndSphere<FloatTypeArg>& InB, float InTolerance);

	template<class FloatTypeArg>
	bool AreBoxesOverlapping(const AABBAndSphere<FloatTypeArg>& InA, const AABBAndSphere<FloatTypeArg>& InB, float InTolerance);

	/****************************************************************************************************
	* Transformation
	*****************************************************************************************************/
	// Expanding both the sphere and box
	template<class FloatTypeArg>
	void ExpandBy(AABBAndSphere<FloatTypeArg>* pOutAABB, float InExpandAmount);	

	/****************************************************************************************************
	* implementation
	*****************************************************************************************************/
	template<class FloatTypeArg>
	AABBAndSphere<FloatTypeArg>::AABBAndSphere(const AABBType& InBox, const FloatType InSphereRadius) :
		box(InBox),
		sphereRadius(InSphereRadius)
	{
	}

	template<class FloatTypeArg>
	AABBAndSphere<FloatTypeArg>::AABBAndSphere(const AABBType& InBox) :
		box(InBox),
		sphereRadius(Length(InBox.extent))
	{
	}

	template<class FloatTypeArg>
	AABBAndSphere<FloatTypeArg>::AABBAndSphere(const SphereType& InSphere) :
		box(InSphere.origin, VecType(InSphere.radius, InSphere.radius, InSphere.radius)),
		sphereRadius(InSphere.radius)
	{
	}

	template<class FloatTypeArg>
	AABBAndSphere<FloatTypeArg>::AABBAndSphere(const VecType& InOrigin, const VecType& InExtent, FloatType InRadius) :
		box(InOrigin, InExtent),
		sphereRadius(InRadius)
	{
	}

	template<class FloatTypeArg>
	typename AABBAndSphere<FloatTypeArg>::SphereType AABBAndSphere<FloatTypeArg>::GetSphere() const
	{
		return SphereType(box.origin, sphereRadius)
	}
} // DvUtils::Math