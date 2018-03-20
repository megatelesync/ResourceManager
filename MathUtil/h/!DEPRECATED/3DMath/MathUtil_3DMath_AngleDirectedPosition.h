#ifndef _MathUtil_3DMath_AngleDirectedPosition_h_
#define _MathUtil_3DMath_AngleDirectedPosition_h_

#include <DirectXMath.h>
#include "../Core/MathUtil_Core_AnglePair.h"
#include <boost/property_tree/ptree.hpp>
#include "MathUtil_3DMath_Utils.h"

namespace DvUtils
{
	namespace Math
	{
		struct AngleDirectedPosition
		{
			/***********************************************************************************************
			* public variables
			************************************************************************************************/
			DirectX::XMFLOAT3							position;
			AnglePair									direction;


			/***********************************************************************************************
			* helper getters
			************************************************************************************************/
			float										GetHorzAngle					()				const			{ return direction.horz; }
			float										GetVerticalAngle				()				const			{ return direction.horz; }


			/***********************************************************************************************
			* ctors
			************************************************************************************************/
			// default ctor: initialize as the object with zero position and direction vector with zero angles
			inline										AngleDirectedPosition			() : 
				position(DirectX::XMFLOAT3(0.0F,0.0F,0.0F)), 
				direction() {}
			inline										AngleDirectedPosition			(const DirectX::XMFLOAT3& positionIn, const AnglePair& directionIn);
			inline										AngleDirectedPosition			(const DirectX::XMFLOAT3& positionIn, float horzAngleIn, float verticalAngleIn);
		};		
		AngleDirectedPosition LoadAngleDirectedPosition_FromPropertyTree(const boost::property_tree::ptree& ptree);
		
		inline DirectX::XMMATRIX CalcAffineMatrix(const AngleDirectedPosition& AngleDirectedPosition);
		inline DirectX::XMMATRIX CalcViewMatrix(const AngleDirectedPosition& AngleDirectedPosition);





		inline DirectX::XMMATRIX CalcViewMatrix(const AngleDirectedPosition& angleDirectedPosition)
		{
			DirectX::XMMATRIX affineMatrix = CalcAffineMatrix(angleDirectedPosition);
			DirectX::XMMATRIX resMatrix = DirectX::XMMatrixInverse(nullptr, affineMatrix);
			return resMatrix;
		}

		inline DirectX::XMMATRIX CalcAffineMatrix(const AngleDirectedPosition& angleDirectedPosition)
		{
			DirectX::XMMATRIX resMatrix = RotationMatrixXVertYHorz(angleDirectedPosition.direction);
			DirectX::XMFLOAT4 position4 (angleDirectedPosition.position.x, angleDirectedPosition.position.y, angleDirectedPosition.position.z, 1.0F);
			resMatrix.r[3] = DirectX::XMLoadFloat4(&position4); 
			return resMatrix;
		}

		// AngleDirectedPosition impl
		inline AngleDirectedPosition::AngleDirectedPosition
		(
			const DirectX::XMFLOAT3& positionIn, const AnglePair& directionIn
		) : position(positionIn), direction(directionIn) {}

		inline AngleDirectedPosition::AngleDirectedPosition
		(
			const DirectX::XMFLOAT3& positionIn, float horzAngleIn, float verticalAngleIn
		) : position(positionIn), direction(AnglePair(horzAngleIn,verticalAngleIn))
		{
		}
	} // Math
} // DvUtils

#endif // _MathUtil_3DMath_AngleDirectedPosition_h_