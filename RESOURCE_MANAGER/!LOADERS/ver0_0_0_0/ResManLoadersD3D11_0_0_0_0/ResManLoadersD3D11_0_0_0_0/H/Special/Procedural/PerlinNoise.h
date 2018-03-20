#pragma once

#include "../../D3D11Texture2DResource.h"
#include "Gen/MathUtil_PerlinNoise.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*********************************************************************************************************
			* Params of perlin noise 2D texture
			*********************************************************************************************************/
			struct PerlinNoiseTexture2DParams
			{
				GeneratedTexture2DParams                                   texture;
				DvUtils::Math::PerlinNoiseParams                           noise;

				PerlinNoiseTexture2DParams(const GeneratedTexture2DParams& InTextureParams);
			};

			/*********************************************************************************************************
			* Base class of all loaders of perlin noise textures
			*
			* This generators are intended only for generation of ordinary texture maps (not depth stencil textures).
			*********************************************************************************************************/
			class D3D11PerlinNoiseLoaderBase : public D3D11SequentialGeneratedTextureLoaderBase
			{
			public:
				D3D11PerlinNoiseLoaderBase
				(
					const PerlinNoiseTexture2DParams&       InParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11Texture2DResourceType             *pInTypedResourceType
				);

				/*******************************************************************************************************
				* Getters
				*******************************************************************************************************/
				const PerlinNoiseTexture2DParams          *GetPerlinNoiseTexture2DParams() const                 { return &_params; }

			protected:
				/********************************************************************************************************
				* Implementation instruction:
				* 1. Implement _WriteCurrentNoiseTexel with code that actually writes actual value to texture
				********************************************************************************************************/
				virtual void                               _LoadCurrentTexel(void *pInRow, int InX, int InY) override final;
				virtual void                               _WriteCurrentNoiseTexel(void *pInRow, int InX, double InNoiseValue) =0;

				/********************************************************************************************************
				* Helper getters
				********************************************************************************************************/
				// Get typeds texture format (ordinary texture format)
				const TextureFormat                              *GetTypedTextureFormat() const                     { return GetTextureFormat()->CastToTextureFormat(); }

			private:
				PerlinNoiseTexture2DParams                  _params;
			};

			/********************************************************************************************************
			* Params of linear interpolator perlin noise generator
			*********************************************************************************************************/
			struct LinearPerlinNoiseParams
			{
				PerlinNoiseTexture2DParams noiseTex;

				// min and max value to interpolate with perlin noise for each of channels
				float            redChannel_minValue = 0;
				float            redChannel_maximValue = 255;

				float            greenChannel_minValue = 0;
				float            greenChannel_maximValue = 255;

				float            blueChannel_minValue = 0;
				float            blueChannel_maximValue = 255;

				float            alphaChannel_minValue = 0;
				float            alphaChannel_maximValue = 255;

				LinearPerlinNoiseParams(const PerlinNoiseTexture2DParams& InNoiseTex);
			};

			/*********************************************************************************************************
			* Greyscale perlin noise texture generator
			*
			* Fills Red channel of result texture with value of generated noise.
			*********************************************************************************************************/
			class D3D11LinearPerlinNoiseLoader : public D3D11PerlinNoiseLoaderBase
			{
			public:
				D3D11LinearPerlinNoiseLoader
				(
					const LinearPerlinNoiseParams&       InParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11Texture2DResourceType             *pInTypedResourceType
				);

				/*******************************************************************************************************
				* Getters
				*******************************************************************************************************/
				const LinearPerlinNoiseParams          *GetLinearPerlinNoiseDParams() const { return &_params; }

			protected:
				virtual void                           _WriteCurrentNoiseTexel(void *pInRow, int InX, double InNoiseValue) override;

			private:
				LinearPerlinNoiseParams                _params;
			};
		}
	}
}