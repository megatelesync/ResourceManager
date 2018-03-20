#pragma once

#include "../ResManLoadersD3D11_System.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			class TextureFormat;
			class DepthStencilTextureFormat;

			/**********************************************************************************************
			* Base class for all texture formats
			**********************************************************************************************/
			class TextureFormatBase
			{
			public:
				TextureFormatBase(DXGI_FORMAT InFormat, size_t InPixelSize, bool bInIsDepthStencilFormat = false) : _format(InFormat), _pixelSize(InPixelSize), _bIsDepthStencil(bInIsDepthStencilFormat){}

				bool                               IsDepthStencil                                           () const { return _bIsDepthStencil; }

				const TextureFormat               *CastToTextureFormat                                      () const;
				TextureFormat                     *CastToTextureFormat                                      ();

				const DepthStencilTextureFormat   *CastToDepthStencilTextureFormat                          () const;
				DepthStencilTextureFormat         *CastToDepthStencilTextureFormat                          ();

				DXGI_FORMAT                        GetFormat                                                () const { return _format; }
				size_t                             GetPixelSize                                             () const { return _pixelSize; }

				// Calc pitch between rows for the given width for the given format
				virtual size_t                     CalcPitchBetweenRows                                     (size_t InWidth) const;

			private:
				DXGI_FORMAT                  _format;
				size_t                       _pixelSize;
				bool                         _bIsDepthStencil = false;
			};

			class TextureFormat : public TextureFormatBase
			{
			public:
				TextureFormat(DXGI_FORMAT InFormat, size_t InPixelSize) : TextureFormatBase(InFormat, InPixelSize) {}

				// Set pixel for the given row
				virtual void                 SetRowPixel
				(
					void                    *pRowPointer,
					UINT                     InX,
					float                    InRed,
					float                    InGreen,
					float                    InBlue,
					float                    InAlpha
				) const = 0;

			private:
			};

			class DepthStencilTextureFormat : public TextureFormatBase
			{
			public:
				DepthStencilTextureFormat(DXGI_FORMAT InFormat, size_t InPixelSize) : TextureFormatBase(InFormat, InPixelSize, true) {}

				virtual void                 SetRowDepthStencilPixel
				(
					void                    *pRowPointer,
					UINT                     InX,
					float                    InDepth,
					float                    InStencil
				) const = 0;

			private:
			};

			class DepthStencilTextureFormat_D32_FLOAT_S8X24_UINT : public DepthStencilTextureFormat
			{
			public:
				DepthStencilTextureFormat_D32_FLOAT_S8X24_UINT() :
					DepthStencilTextureFormat(DXGI_FORMAT_D32_FLOAT_S8X24_UINT, 64) {}

				virtual void                 SetRowDepthStencilPixel
				(
					void                    *pRowPointer,
					UINT                     InX,
					float                    InDepth,
					float                    InStencil
				) const override;

			private:
			};

			class DepthStencilTextureFormat_D24_UNORM_S8_UINT : public DepthStencilTextureFormat
			{
			public:
				DepthStencilTextureFormat_D24_UNORM_S8_UINT() :
					DepthStencilTextureFormat(DXGI_FORMAT_D24_UNORM_S8_UINT, 32) {}

				virtual void                 SetRowDepthStencilPixel
				(
					void                    *pRowPointer,
					UINT                     InX,
					float                    InDepth,
					float                    InStencil
				) const override;

			private:
			};

			class DepthStencilTextureFormat_D32_FLOAT : public DepthStencilTextureFormat
			{
			public:
				DepthStencilTextureFormat_D32_FLOAT() :
					DepthStencilTextureFormat(DXGI_FORMAT_D32_FLOAT, 32) {}

				virtual void                 SetRowDepthStencilPixel
				(
					void                    *pRowPointer,
					UINT                     InX,
					float                    InDepth,
					float                    InStencil
				) const override;
			};

			class TextureFormat_R8G8B8A8 : public TextureFormat
			{
			public:
				TextureFormat_R8G8B8A8(DXGI_FORMAT InFormat) : 
					TextureFormat(InFormat, 4) {}

				virtual void                 SetRowPixel
				(
					void                    *pRowPointer,
					UINT                     InX,
					float                    InRed,
					float                    InGreen,
					float                    InBlue,
					float                    InAlpha
				) const override;
			};

			class TextureFormats
			{
			public:
				TextureFormats();

				const TextureFormat_R8G8B8A8                                    *GetRGBA8_UNORM             () const               { return &_rgba8_unorm; }
				const TextureFormat_R8G8B8A8                                    *GetRGBA8_UINT              () const               { return &_rgba8_uint; }
				const DepthStencilTextureFormat_D32_FLOAT_S8X24_UINT            *GetD32_FLOAT_S8_UINT       () const               { return &_d32s8; }
				const DepthStencilTextureFormat_D24_UNORM_S8_UINT               *GetD24_FLOAT_S8_UINT       () const               { return &_d24s8; }
				const DepthStencilTextureFormat_D32_FLOAT                       *GetD32_FLOAT               () const               { return &_d32; }

			private:
				TextureFormat_R8G8B8A8                                          _rgba8_unorm;
				TextureFormat_R8G8B8A8                                          _rgba8_uint;
				DepthStencilTextureFormat_D32_FLOAT_S8X24_UINT                  _d32s8;
				DepthStencilTextureFormat_D24_UNORM_S8_UINT                     _d24s8;
				DepthStencilTextureFormat_D32_FLOAT                             _d32;
			};
		}
	}
}