#ifndef _MathUtil_Core_AnglePair_h_
#define _MathUtil_Core_AnglePair_h_

#include "MathUtil_Core_LinearAlgebraUtils.h"
#include "MathUtil_DirectXMath.h"
#include "../3DMath/MathUtil_3DMath_Utils.h"
#include <boost/property_tree/ptree.hpp>

namespace DvUtils
{
	namespace Math
	{
		// Pair of angles
		struct AnglePair
		{
			/*******************************************************************************
			* public variables
			********************************************************************************/
			float							horz;
			float							vertical;

			/*******************************************************************************
			* ctors
			********************************************************************************/
			// default ctor: initialize angle pair with both vectors equal to zero
			inline							AnglePair						() : 
				horz(0.0F), 
				vertical(0.0F) {}
			inline							AnglePair						(float horzIn, float verticalIn);
			
			/*******************************************************************************
			* static constants
			********************************************************************************/
			const static AnglePair DirectionX;
			const static AnglePair DirectionY;
			const static AnglePair DirectionZ;
		};		

		// Calculate the direction vector from the angle pair
		template<class FloatType>
		void									CalcDirection3D				(FloatType *pOutDirection3D, const AnglePair& InAnglePair, size_t DestStrideInBytes = sizeof(FloatType));
		inline DirectX::XMFLOAT3				CalcDirection3D_XM			(const AnglePair& InAnglePair);

		inline DirectX::XMMATRIX RotationMatrixXVertZHorz		(const AnglePair& anglePair);
		inline DirectX::XMMATRIX RotationMatrixXVertYHorz		(const AnglePair& anglePair);
		AnglePair LoadAnglePair_FromPropertyTree				(const boost::property_tree::ptree& ptree);





		// AnglePair impl
		inline AnglePair::AnglePair(float horzIn, float verticalIn) : 
			horz(horzIn), vertical(verticalIn) {}

		// AnglePair function impl
		inline DirectX::XMFLOAT3 CalcDirection3D_XM(const AnglePair& InAnglePair)
		{
			DirectX::XMFLOAT3 res_direction;
			CalcDirection3D(&res_direction.x, InAnglePair);
			return res_direction;
		}

		template<class FloatType>
		void CalcDirection3D
		(
			FloatType				*pOutDirection3D, 			
			const AnglePair&		 InAnglePair,
			size_t					 DestStrideInBytes
		)
		{
			assert(pOutDirection3D != nullptr);
			CalcRadiusVector3D		(pOutDirection3D, InAnglePair.horz, InAnglePair.vertical, DestStrideInBytes);
			Transform_XYZ_to_XZY	(pOutDirection3D, DestStrideInBytes);			
		}

		inline DirectX::XMMATRIX RotationMatrixXVertZHorz(const AnglePair& anglePair)
		{
			return RotationMatrixXVertZHorz(anglePair.vertical, anglePair.horz);
		}

		inline DirectX::XMMATRIX RotationMatrixXVertYHorz(const AnglePair& anglePair)
		{
			return RotationMatrixXVertYHorz(anglePair.vertical, anglePair.horz);			
		}
	} // Math
} // DvUtils

#endif // _MathUtil_Core_AnglePair_h_