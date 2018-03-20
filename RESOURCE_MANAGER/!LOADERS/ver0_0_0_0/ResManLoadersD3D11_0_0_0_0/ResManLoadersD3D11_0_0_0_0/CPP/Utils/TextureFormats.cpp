#include "../../H/Utils/TextureFormat.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			BYTE *GetPixelPtr(const TextureFormatBase *pInFormat, void *pRowPointer, UINT InX)
			{
				assert(pRowPointer != nullptr);
				auto pRowBytes = static_cast<uint8_t *>(pRowPointer);
				auto pPixelBytes = pRowBytes + pInFormat->GetPixelSize() * InX;
				return pPixelBytes;
			}

			void SetRGB_Only(BYTE *pPixelBytes, float InRed, float InGreen, float InBlue)
			{
				assert(pPixelBytes != nullptr);
				pPixelBytes[0] = static_cast<BYTE>(InRed);
				pPixelBytes[1] = static_cast<BYTE>(InGreen);
				pPixelBytes[2] = static_cast<BYTE>(InBlue);
			}
			void SetPtr_Float(BYTE *pPixelBytes, float InValue)
			{
				assert(pPixelBytes);
				auto pPixelBytesVoid = static_cast<void *>(pPixelBytes);
				*(static_cast<float *>(pPixelBytesVoid)) = InValue;
			}
			void SetPtr_UInt8_WithOffset(BYTE *pPixelBytes, size_t InOffset, float InValue)
			{
				assert(pPixelBytes);
				auto pPixelBytesVoid_WithOffset = static_cast<void *>(pPixelBytes + InOffset);
				*(static_cast<uint8_t *>(pPixelBytesVoid_WithOffset)) = static_cast<uint8_t>(InValue);
			}
			void SetPtr_UInt16_WithOffset(BYTE *pPixelBytes, size_t InOffset, float InValue)
			{
				assert(pPixelBytes);
				auto pPixelBytesVoid_WithOffset = static_cast<void *>(pPixelBytes + InOffset);
				*(static_cast<uint16_t *>(pPixelBytesVoid_WithOffset)) = static_cast<uint16_t>(InValue);
			}

			void SetRGBA(BYTE *pPixelBytes, float InRed, float InGreen, float InBlue, float InAlpha)
			{
				SetRGB_Only(pPixelBytes, InRed, InGreen, InBlue);
				pPixelBytes[3] = static_cast<BYTE>(InAlpha);
			}

			void TextureFormat_R8G8B8A8::SetRowPixel
			(
				void                    *pRowPointer,
				UINT                     InX,
				float                    InRed,
				float                    InGreen,
				float                    InBlue,
				float                    InAlpha
			) const
			{
				auto pPixelBytes = GetPixelPtr(this, pRowPointer, InX);
				SetRGBA(pPixelBytes, InRed, InGreen, InBlue, InAlpha);
			}

			/**************************************************************************************
			* TextureFormatBase
			**************************************************************************************/
			size_t TextureFormatBase::CalcPitchBetweenRows(size_t InWidth) const
			{
				return GetPixelSize() * InWidth;
			}

			const TextureFormat *TextureFormatBase::CastToTextureFormat() const
			{
				assert(false == IsDepthStencil()); 
				return dynamic_cast<const TextureFormat *>(this); 
			}

			const DepthStencilTextureFormat *TextureFormatBase::CastToDepthStencilTextureFormat() const
			{
				assert(IsDepthStencil());
				return dynamic_cast<const DepthStencilTextureFormat *>(this);
			}

			TextureFormat *TextureFormatBase::CastToTextureFormat()
			{
				assert(false == IsDepthStencil());
				return dynamic_cast<TextureFormat *>(this);
			}

			DepthStencilTextureFormat *TextureFormatBase::CastToDepthStencilTextureFormat()
			{
				assert(IsDepthStencil());
				return dynamic_cast<DepthStencilTextureFormat *>(this);
			}

			/**************************************************************************************
			* DepthStencil texture formats
			**************************************************************************************/
			void DepthStencilTextureFormat_D32_FLOAT_S8X24_UINT::SetRowDepthStencilPixel(void *pRowPointer, UINT InX, float InDepth, float InStencil) const
			{
				auto pPixelBytes = GetPixelPtr(this, pRowPointer, InX);
				const size_t component_offset_stencil = 32;
				SetPtr_Float(pPixelBytes, InDepth);
				SetPtr_UInt8_WithOffset(pPixelBytes, component_offset_stencil, InStencil);
			}

			void DepthStencilTextureFormat_D24_UNORM_S8_UINT::SetRowDepthStencilPixel
			(
				void *pRowPointer,
				UINT InX,
				float InDepth,
				float InStencil
			) const
			{
				Util::NotYetImplVoid("How to convert float InDepth to 24-bit float?");
			}

			void DepthStencilTextureFormat_D32_FLOAT::SetRowDepthStencilPixel
			(
				void *pRowPointer,
				UINT InX,
				float InDepth,
				float InStencil
			) const
			{
				auto pPixelBytes = GetPixelPtr(this, pRowPointer, InX);
				SetPtr_Float(pPixelBytes, InDepth);
			}

			/**************************************************************************************
			* TextureFormats
			***************************************************************************************/
			TextureFormats::TextureFormats() :
				_rgba8_unorm(DXGI_FORMAT_R8G8B8A8_UNORM),
				_rgba8_uint(DXGI_FORMAT_R8G8B8A8_UINT),
				_d32s8(),
				_d24s8(),
				_d32()
			{
			}
		}
	}
}