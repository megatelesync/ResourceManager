#ifndef _MathUtil_3DMath_DirectedFrustum_h_
#define _MathUtil_3DMath_DirectedFrustum_h_

#include "MathUtil_3DMath_AngleDirectedPosition.h"
#include "MathUtil_3DMath_FrustumInfo.h"

namespace DvUtils
{
	namespace Math
	{
		// DirectedFrustum
		struct DirectedFrustum
		{
			FrustumInfo								info;
			AngleDirectedPosition					positionDirection;

			const DirectX::XMFLOAT3&				GetPosition()			const		{ return positionDirection.position; }
			const AnglePair&						GetDirection()			const		{ return positionDirection.direction; }			

			inline DirectedFrustum() : info(), positionDirection() {}
			inline DirectedFrustum(const FrustumInfo& frustumInfoIn, AngleDirectedPosition& positionDirectionIn);
		};
		inline DirectX::XMMATRIX CalcViewProjMatrix(const DirectedFrustum& directedFrustum);		





		// DirectedFrustum impl
		inline DirectedFrustum::DirectedFrustum
		(
			const FrustumInfo& frustumInfoIn, 
			AngleDirectedPosition& positionDirectionIn
		) : info(frustumInfoIn), positionDirection(positionDirectionIn)
		{
		}

		inline DirectX::XMMATRIX CalcViewProjMatrix(const DirectedFrustum& directedFrustum)
		{
			DirectX::XMMATRIX viewMatrix = CalcViewMatrix(directedFrustum.positionDirection);
			//DirectX::XMMATRIX projMatrix = CalcProjMatrix(directedFrustum.info);
			DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(2.0F*directedFrustum.info.GetHalfFov(), directedFrustum.info.aspectWidthOverHeight, directedFrustum.info.GetZNear(), directedFrustum.info.GetZFar());
			DirectX::XMMATRIX res = DirectX::XMMatrixMultiply(viewMatrix,projMatrix);
			return res;
		}		
	} // Math
} // DvUtils

#endif // _MathUtil_3DMath_DirectedFrustum_h_
