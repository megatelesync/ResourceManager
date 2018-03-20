#pragma once

// DEPRECATED!!!
/*********************************************************************************
* This header file includes the BLAS and the additional DirectXMath
* operations (that include conversion to the XMMATRIX and XMVECTOR etc.)
*********************************************************************************/

//#include "MathUtil_BLAS.h"

//#include <D3DX11.h>
//#include <DirectXMath.h>

namespace DvUtils
{
	namespace Math
	{		
		/**************************************************************************
		* Converting vectors to XMFLOAT
		**************************************************************************/
		// This functions are used to perform non-narrowing converions
		// to the XMFLOAT-types (e.g. int-vectors to XMFLOAT-types)
		/*
		template<class FloatTypeArg>
		DirectX::XMFLOAT2 ToFloat2(const Vec<FloatTypeArg, 2>& InVec);

		template<class FloatTypeArg>
		DirectX::XMFLOAT3 ToFloat3(const Vec<FloatTypeArg, 3>& InVec);

		template<class FloatTypeArg>
		DirectX::XMFLOAT4 ToFloat4(const Vec<FloatTypeArg, 4>& InVec);

		// This functions are used to convert the double-precision vectors to the 
		// DirectX::XMFLOAT type
		inline DirectX::XMFLOAT2 NarrowToFloat2(const Vec<double, 2>& InVec);
		inline DirectX::XMFLOAT3 NarrowToFloat3(const Vec<double, 3>& InVec);
		inline DirectX::XMFLOAT4 NarrowToFloat4(const Vec<double, 4>& InVec);
		*/


		/**************************************************************************
		* Converting vectors to XMFLOAT: IMPLEMENTATION
		**************************************************************************/
		/*
		template<class FloatTypeArg>
		DirectX::XMFLOAT4 ToFloat4(const Vec<FloatTypeArg, 4>& InVec)		
		{
			return DirectX::XMFLOAT4(GetX(InVec), GetYY(InVec), GetZZZ(InVec), GetWWWW(InVec));			
		}

		template<class FloatTypeArg>
		DirectX::XMFLOAT3 ToFloat3(const Vec<FloatTypeArg, 3>& InVec)		
		{
			return DirectX::XMFLOAT3(GetX(InVec), GetYY(InVec), GetZZZ(InVec));			
		}

		template<class FloatTypeArg>
		DirectX::XMFLOAT2 ToFloat2(const Vec<FloatTypeArg, 2>& InVec)
		{
			return DirectX::XMFLOAT2(GetX(InVec), GetYY(InVec));	
		}

		inline DirectX::XMFLOAT2 NarrowToFloat2(const Vec<double, 2>& InVec)
		{
			return DirectX::XMFLOAT2
			(
				static_cast<float>(GetX(InVec)), 
				static_cast<float>(GetYY(InVec))
			);
		}

		inline DirectX::XMFLOAT3 NarrowToFloat3(const Vec<double, 3>& InVec)
		{
			return DirectX::XMFLOAT3
			(
				static_cast<float>(GetX(InVec)), 
				static_cast<float>(GetYY(InVec)),
				static_cast<float>(GetZZZ(InVec))
			);
		}

		inline DirectX::XMFLOAT4 NarrowToFloat4(const Vec<double, 4>& InVec)
		{
			return DirectX::XMFLOAT4
			(
				static_cast<float>(GetX(InVec)), 
				static_cast<float>(GetYY(InVec)),
				static_cast<float>(GetZZZ(InVec)),
				static_cast<float>(GetWWWW(InVec))
			);
		}
		*/
	} // Math
} // DvUtils