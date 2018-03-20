#ifndef _MathUtil_Core_DirectXMath_h_
#define _MathUtil_Core_DirectXMath_h_

#include <DirectXMath.h>

namespace DvUtils
{
	namespace Math
	{
		inline	DirectX::XMFLOAT4 ToXmFloat4			(const DirectX::XMFLOAT3& InFloat3);
		inline	DirectX::XMFLOAT4 ToXmFloat4			(const DirectX::XMFLOAT2& InFloat2);				
		inline	DirectX::XMFLOAT4 ToXmFloat4			(float InFloat);
		
		inline	DirectX::XMFLOAT3 ToXmFloat3narrow		(const DirectX::XMFLOAT4& InFloat4);		
		inline	DirectX::XMFLOAT3 ToXmFloat3			(const DirectX::XMFLOAT2& InFloat2);
		inline	DirectX::XMFLOAT3 ToXmFloat3			(float InFloat);

		inline	DirectX::XMFLOAT2 ToXmFloat2narrow		(const DirectX::XMFLOAT4& InFloat4);		
		inline	DirectX::XMFLOAT2 ToXmFloat2narrow		(const DirectX::XMFLOAT3& InFloat3);				
		inline	DirectX::XMFLOAT2 ToXmFloat2			(float InFloat);


		// Impl
		inline	DirectX::XMFLOAT4 ToXmFloat4			(const DirectX::XMFLOAT3& InFloat3)
		{
			return DirectX::XMFLOAT4(InFloat3.x, InFloat3.y, InFloat3.z, 1.0F);
		}

		inline	DirectX::XMFLOAT4 ToXmFloat4			(const DirectX::XMFLOAT2& InFloat2)
		{
			return DirectX::XMFLOAT4(InFloat2.x, InFloat2.y, 0.0F, 1.0F);
		}

		inline	DirectX::XMFLOAT4 ToXmFloat4			(float InFloat)
		{
			return DirectX::XMFLOAT4(InFloat, 0.0F, 0.0F, 1.0F);
		}
						
		inline	DirectX::XMFLOAT3 ToXmFloat3narrow		(const DirectX::XMFLOAT4& InFloat4)
		{
			return DirectX::XMFLOAT3(InFloat4.x, InFloat4.y, InFloat4.z);
		}

		inline	DirectX::XMFLOAT3 ToXmFloat3			(const DirectX::XMFLOAT2& InFloat2)
		{
			return DirectX::XMFLOAT3(InFloat2.x, InFloat2.y, 0.0F);
		}
		
		inline	DirectX::XMFLOAT3 ToXmFloat3			(float InFloat)
		{
			return DirectX::XMFLOAT3(InFloat, 0.0F, 0.0F);
		}

		inline	DirectX::XMFLOAT2 ToXmFloat2narrow		(const DirectX::XMFLOAT4& InFloat4)
		{
			return DirectX::XMFLOAT2(InFloat4.x, InFloat4.y);
		}

		inline	DirectX::XMFLOAT2 ToXmFloat2narrow		(const DirectX::XMFLOAT3& InFloat3)
		{
			return DirectX::XMFLOAT2(InFloat3.x, InFloat3.y);
		}

		inline	DirectX::XMFLOAT2 ToXmFloat2			(float InFloat)
		{
			return DirectX::XMFLOAT2(InFloat, 0.0F);
		}
	} // Math
} // DvUtils

#endif // _MathUtil_Core_DirectXMath_h_