#ifndef _MathUtil_3DMath_Frustum_h_
#define _MathUtil_3DMath_Frustum_h_

#include <boost/property_tree/ptree.hpp>

namespace DvUtils
{
	namespace Math
	{
		// Structure that contains only geometic data about frustum
		// without aspect
		struct FrustumGeoInfo
		{
			/************************************************************************************
			* public members
			************************************************************************************/
			float							zNear;
			float							zFar;
			float							halfFov;

			/************************************************************************************
			* ctors
			************************************************************************************/
			// initialize the frustum geo info with default zNear and zFar values and human halfFov
			inline							FrustumGeoInfo				();
			inline							FrustumGeoInfo				(float zNearIn, float zFarIn, float halfFovIn);
		};
		inline float FrustumDepth(const FrustumGeoInfo& frustumInfo);
		// load from property tree (returns default if some fields are not specified in the config file)
		FrustumGeoInfo LoadFrustumGeoInfo_FromPropertyTree(const boost::property_tree::ptree& ptree);





		// IMPL
		inline float FrustumDepth(const FrustumGeoInfo& frustumInfo) { return frustumInfo.zFar - frustumInfo.zNear; }
		inline FrustumGeoInfo::FrustumGeoInfo() :
			zNear(0.0F), zFar(100.0F), halfFov(3.1415926434897/4.0F)
		{
		}
		inline FrustumGeoInfo::FrustumGeoInfo(float zNearIn, float zFarIn, float halfFovIn) :
			zNear(zNearIn),
			zFar(zFarIn),
			halfFov(halfFovIn)
		{
		}
	} // Math
} // Dv

#endif // _MathUtil_3DMath_Frustum_h_