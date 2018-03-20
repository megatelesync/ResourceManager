#include "../../../H/Special/Procedural/PerlinNoise.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/**********************************************************************************************************
			* D3D11PerlinNoiseLoaderBase implementation
			**********************************************************************************************************/
			D3D11PerlinNoiseLoaderBase::D3D11PerlinNoiseLoaderBase
			(
				const PerlinNoiseTexture2DParams&       InParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11Texture2DResourceType             *pInTypedResourceType
			) :
				D3D11SequentialGeneratedTextureLoaderBase(&InParams.texture, pInDev, pInRole, pInTypedResourceType),
				_params(InParams)
			{

			}

			void D3D11PerlinNoiseLoaderBase::_LoadCurrentTexel(void *pInRow, int InX, int InY)
			{
				double perlinX = ((InX+1)/_params.texture.width);
				double perlinY = ((InY+1)/_params.texture.height);
				const double perlinZ = 0.0F;
				double noiseValue = DvUtils::Math::octavePerlin3D(perlinX, perlinY, perlinZ, &_params.noise);
				_WriteCurrentNoiseTexel(pInRow, InX, noiseValue);
			}

			/**********************************************************************************************************
			* D3D11LinearPerlinNoiseLoader implementation
			**********************************************************************************************************/
			D3D11LinearPerlinNoiseLoader::D3D11LinearPerlinNoiseLoader
			(
				const LinearPerlinNoiseParams&       InParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11Texture2DResourceType             *pInTypedResourceType
			) :
				D3D11PerlinNoiseLoaderBase(InParams.noiseTex, pInDev, pInRole, pInTypedResourceType),
				_params(InParams)
			{
			}

			void D3D11LinearPerlinNoiseLoader::_WriteCurrentNoiseTexel(void *pInRow, int InX, double InNoiseValue)
			{
				float R = DvUtils::Math::Lerp(_params.redChannel_minValue, _params.redChannel_maximValue, static_cast<float>(InNoiseValue));
				float G = DvUtils::Math::Lerp(_params.greenChannel_minValue, _params.greenChannel_maximValue, static_cast<float>(InNoiseValue));
				float B = DvUtils::Math::Lerp(_params.blueChannel_minValue, _params.blueChannel_maximValue, static_cast<float>(InNoiseValue));
				float A = DvUtils::Math::Lerp(_params.alphaChannel_minValue, _params.alphaChannel_maximValue, static_cast<float>(InNoiseValue));
				GetTypedTextureFormat()->SetRowPixel(pInRow, InX, R, G, B, A);
			}

			/*********************************************************************************************************
			* PerlinNoiseTexture2DParams implementation
			*********************************************************************************************************/
			PerlinNoiseTexture2DParams::PerlinNoiseTexture2DParams(const GeneratedTexture2DParams& InTextureParams) :
				texture(InTextureParams)
			{
			}

			/*********************************************************************************************************
			* LinearPerlinNoiseParams implementation
			*********************************************************************************************************/
			LinearPerlinNoiseParams::LinearPerlinNoiseParams(const PerlinNoiseTexture2DParams& InNoiseTex) :
				noiseTex(InNoiseTex)
			{
			}
		}
	}
}