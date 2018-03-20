#ifndef _MathUtil_3DMath_FrustumInfo_h_
#define _MathUtil_3DMath_FrustumInfo_h_

#include "MathUtil_3DMath_Frustum.h"
#include "MathUtil_3DMath_Utils.h"

namespace DvUtils
{
	namespace Math
	{
		// FrustumInfo
		struct FrustumInfo
		{
			/************************************************************************************
			* public variables
			************************************************************************************/
			FrustumGeoInfo					geo;
			float							aspectWidthOverHeight;


			/************************************************************************************
			* helper getters
			************************************************************************************/
			inline float					GetZNear							()			const			{ return geo.zNear; }
			inline float					GetZFar								()			const			{ return geo.zFar; }
			inline float					GetHalfFov							()			const			{ return geo.halfFov; }				


			/************************************************************************************
			* ctors
			************************************************************************************/
			// default ctor: initialize frustum with aspect of 1.0F and default frustum geo info
			inline FrustumInfo();
			inline FrustumInfo(float zNearIn, float zFarIn, float halfFovIn, float aspectWidthOverHeightIn);
			inline FrustumInfo(const FrustumGeoInfo& InFrustumGeoInfo, float InAspectWidthOverHeight);
		};		
		inline DirectX::XMMATRIX CalcProjMatrix(const FrustumInfo& frustumInfo);
		inline float FrustumDepth(const FrustumInfo& frustumInfo) { return FrustumDepth(frustumInfo.geo); }


		// FrustumInfo impl
		inline FrustumInfo::FrustumInfo(const FrustumGeoInfo& InFrustumGeoInfo, float InAspectWidthOverHeight) :
			geo(InFrustumGeoInfo),			
			aspectWidthOverHeight(InAspectWidthOverHeight)
		{
		}
		inline FrustumInfo::FrustumInfo() : 
			aspectWidthOverHeight(1.0F)
		{
		}
		inline FrustumInfo::FrustumInfo(float zNearIn, float zFarIn, float halfFovIn, float aspectWidthOverHeightIn) :
			geo(zNearIn, zFarIn, halfFovIn),
			aspectWidthOverHeight(aspectWidthOverHeightIn)
		{
		}
		inline DirectX::XMMATRIX CalcProjMatrix(const FrustumInfo& frustumInfo)
		{						
			return CalcPerspectiveProjMatrix
			(
				frustumInfo.GetZFar(), frustumInfo.GetZNear(),
				frustumInfo.GetHalfFov(),
				frustumInfo.aspectWidthOverHeight
			);
		}
	} // Math
} // DvUtils

#endif // _MathUtil_3DMath_FrustumInfo_h_
