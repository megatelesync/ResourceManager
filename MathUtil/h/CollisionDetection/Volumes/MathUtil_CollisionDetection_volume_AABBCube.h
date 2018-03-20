#pragma once

#include "MathUtil_CollisionDetection_volume_utils.h"
#include "boost/assert.hpp"
#include "boost/format.hpp"

// WARNING!!! This header file should NEVER include AABB's header (to prevent recursion)!

namespace DvUtils::Math
{
	template<class FloatTypeArg>
	struct AABBCube
	{
		using FloatType = typename FloatTypeArg;
		using VecType = typename Vec<FloatTypeArg, 3>;

		VecType                                        center;
		float                                          rad = 0.0F;

		AABBCube() = default;
		AABBCube(const VecType& InCenter, float InRadius);
	};

	/***************************************************************************************************
	* Checking validity
	****************************************************************************************************/
	template<class FloatTypeArg>
	bool IsValid(AABBCube<FloatTypeArg>& InVolume);

	/***************************************************************************************************
	* Comparison
	****************************************************************************************************/
	template<class FloatTypeArg>
	bool operator==(const AABBCube<FloatTypeArg>& InA, const AABBCube<FloatTypeArg>& InB);

	template<class FloatTypeArg>
	bool operator!=(const AABBCube<FloatTypeArg>& InA, const AABBCube<FloatTypeArg>& InB);

	template<class FloatTypeArg>
	bool IsNearEqual(const AABBCube<FloatTypeArg>& InA, const AABBCube<FloatTypeArg>& InB, FloatTypeArg InTolerance = GeoEps);

	/***************************************************************************************************
	* Conversion to string
	****************************************************************************************************/
	template<class FloatTypeArg>
	std::string ToDebugString(const AABBCube<FloatTypeArg>& InVolume);

	template<class FloatTypeArg>
	std::string ToString(const AABBCube<FloatTypeArg>& InVolume);

	/***************************************************************************************************
	* Volume
	****************************************************************************************************/
	template<class FloatTypeArg>
	FloatTypeArg GetVolume(const AABBCube<FloatTypeArg>& InVolume);

	/***************************************************************************************************
	* Checking validity implementation
	****************************************************************************************************/
	template<class FloatTypeArg>
	bool IsValid(AABBCube<FloatTypeArg>& InVolume)
	{
		return InVolume.rad > 0;
	}

	/***************************************************************************************************
	* Comparison implementation
	****************************************************************************************************/
	template<class FloatTypeArg>
	bool operator==(const AABBCube<FloatTypeArg>& InA, const AABBCube<FloatTypeArg>& InB)
	{
		return InA.center == InB.center && InA.rad == InB.rad;
	}

	template<class FloatTypeArg>
	bool operator!=(const AABBCube<FloatTypeArg>& InA, const AABBCube<FloatTypeArg>& InB)
	{
		return (false == operator==(InA, InB));
	}

	template<class FloatTypeArg>
	bool IsNearEqual(const AABBCube<FloatTypeArg>& InA, const AABBCube<FloatTypeArg>& InB, FloatTypeArg InTolerance)
	{
		return IsNearEqual(InA.center, InB.center, InTolerance) && IsNearEqual(InA.rad, InB.rad, InTolerance);
	}

	/***************************************************************************************************
	* Conversion to string implementation
	****************************************************************************************************/
	template<class FloatTypeArg>
	std::string ToDebugString(const AABBCube<FloatTypeArg>& InVolume)
	{
		return ToString(InVolume);
	}

	template<class FloatTypeArg>
	std::string ToString(const AABBCube<FloatTypeArg>& InVolume)
	{
		return (boost::format("{center=%1%; rad=%2%}") % ToString(InVolume.center) % InVolume.rad).str();
	}

	/***************************************************************************************************
	* Volume implementation
	****************************************************************************************************/
	template<class FloatTypeArg>
	FloatTypeArg GetVolume(const AABBCube<FloatTypeArg>& InVolume)
	{
		return 8 * InVolume.rad * InVolume.rad * InVolume.rad;
	}

	/***************************************************************************************************
	* implementation
	****************************************************************************************************/
	template<class FloatTypeArg>
	AABBCube<FloatTypeArg>::AABBCube(const VecType& InCenter, float InRadius) :
		center(InCenter),
		rad(InRadius)
	{
	}
} // DvUtils::Math