#pragma once

#include "../H/D3D11Texture2DResource.h"
#include <wx/wx.h>
#include <wx/image.h>

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*****************************************************************************************
			* Description utilities
			*****************************************************************************************/
			void Initialize_Texture2D_Desc(D3D11_TEXTURE2D_DESC *pDesc, const Texture2DParams &InTextureParams)
			{
				ZeroMemory(pDesc, sizeof(D3D11_TEXTURE2D_DESC));
				// Set description data independent of the loaded data
				pDesc->Usage = InTextureParams.usage;
				pDesc->ArraySize = 1; // as texture arrays are not supported by the loader
				pDesc->BindFlags = InTextureParams.bindFlags;
				pDesc->SampleDesc = InTextureParams.sample;
				pDesc->CPUAccessFlags = InTextureParams.CPUAccessFlags;
				pDesc->MiscFlags = InTextureParams.miscFlags;
				pDesc->MipLevels = 1;
				pDesc->Format = InTextureParams.pTextureFormat->GetFormat();
			}

			/*****************************************************************************************
			* LoadedTextureSourceData utils
			*****************************************************************************************/
			void FillTextureSourceData_wxImage(LoadedTextureSourceData *pOutSourceData, const TextureFormat *pInTextureFormat, const wxImage *pSourceImg);

			/*****************************************************************************************
			* Texture 2D loader base
			******************************************************************************************/
			D3D11Texture2DLoaderBase::D3D11Texture2DLoaderBase
			(
				const std::string&                      InURL,
				const Texture2DParams                  &InTextureParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11Texture2DResourceType             *pInTypedResourceType
			) : TypedD3D11ResourceLoaderBase(InURL, pInDev, pInRole, pInTypedResourceType),
				_textureParams(InTextureParams)
			{
				Initialize_Texture2D_Desc(&_desc, InTextureParams);
			}

			D3D11Texture2DLoaderBase::D3D11Texture2DLoaderBase
			(
				const Texture2DParams                  &InTextureParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11Texture2DResourceType             *pInTypedResourceType
			) : TypedD3D11ResourceLoaderBase(pInDev, pInRole, pInTypedResourceType),
				_textureParams(InTextureParams)
			{
				Initialize_Texture2D_Desc(&_desc, InTextureParams);
			}

			typename D3D11Texture2DLoaderBase::RealResourceType *D3D11Texture2DLoaderBase::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				if (pOldPtrOrNull != nullptr)
				{
					pOldPtrOrNull->AddRef();
					return pOldPtrOrNull;
				}
				else
				{
					if (_IsLoaded(GetRoles().pSourceData))
					{
						assert(_sourceData.IsLoaded());
						ID3D11Texture2D *pResTexture = nullptr;
						_desc.Width = _sourceData.GetWidth();
						_desc.Height = _sourceData.GetHeight();
						auto pSubresourceDataArg = &(_sourceData.GetSubresourceData());
						HRESULT hr = _GetDev()->CreateTexture2D(&_desc, pSubresourceDataArg, &pResTexture);
						_SetHResult_Create(hr, "ID3D11DeviceContext::CreateTexture2D");
						if (FAILED(hr))
						{
							return nullptr;
						}
						else
						{
							if (_textureParams.bGenerateMipMaps)
							{
								assert(!"Not Yet impl");
							}
							return pResTexture;
						}
					}
					else
					{
						return nullptr;
					}
				}
			}

			bool D3D11Texture2DLoaderBase::LoadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				if (_ShouldLoadTempContent(InRoleSet, GetRoles().pSourceData))
				{
					if (_TryLoadSourceData(&_sourceData))
					{
						_MarkTempContentRole_Loaded(GetRoles().pSourceData);
						return true;
					}
					else
					{
						return false;
					}
				}
				return true;
			}
			void D3D11Texture2DLoaderBase::UnloadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				if (_ShouldTempContentBeUnloaded(InRoleSet, GetRoles().pSourceData))
				{
					_sourceData.Unload();
					_MarkTempContentRole_Unloaded(GetRoles().pSourceData);
				}
			}

			/*****************************************************************************************
			* D3D11GeneratedTexture2DLoaderBase implementation
			******************************************************************************************/
			D3D11GeneratedTexture2DLoaderBase::D3D11GeneratedTexture2DLoaderBase
			(
				const GeneratedTexture2DParams         *pInParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11Texture2DResourceType             *pInTypedResourceType
			) : D3D11Texture2DLoaderBase(pInParams->texture, pInDev, pInRole, pInTypedResourceType),
				_params(*pInParams)
			{
				_desc.Width = pInParams->width;
				_desc.Height = pInParams->height;
			}

			bool D3D11GeneratedTexture2DLoaderBase::_TryLoadSourceData(LoadedTextureSourceData *pOutTexture)
			{				
				size_t pitchBetweenRows = _params.texture.pTextureFormat->CalcPitchBetweenRows(_params.width);
				pOutTexture->Reset(pitchBetweenRows, _params.width, _params.height, _params.texture.pTextureFormat->GetPixelSize());

				return _TryGenerateSourceData(pOutTexture);
			}

			/*****************************************************************************************
			* D3D11SequentialGeneratedTextureLoaderBase implementation
			******************************************************************************************/
			D3D11SequentialGeneratedTextureLoaderBase::D3D11SequentialGeneratedTextureLoaderBase
			(
				const GeneratedTexture2DParams         *pInParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11Texture2DResourceType             *pInTypedResourceType
			) : D3D11GeneratedTexture2DLoaderBase(pInParams, pInDev, pInRole, pInTypedResourceType)
			{
			}

			bool D3D11SequentialGeneratedTextureLoaderBase::_TryGenerateSourceData(LoadedTextureSourceData *pOutTexture)
			{
				// AT THIS POINT:
				// 1. Reset is called, and width and height are properly set (see comment of generated texture loader base).
				size_t pitchBetweenRows = GetTextureFormat()->CalcPitchBetweenRows(pOutTexture->GetWidth());

				for (size_t Y = 0; Y < pOutTexture->GetHeight(); Y++)
				{
					BYTE *pRow = pOutTexture->GetData() + Y * pitchBetweenRows;
					for (size_t X = 0; X < pOutTexture->GetWidth(); X++)
					{
						_LoadCurrentTexel(pRow, X, Y);
					}
				}

				return true;
			}

			/*****************************************************************************************
			* D3D11UniformColorTexture2D implementation
			******************************************************************************************/
			D3D11UniformColorTexture2D::D3D11UniformColorTexture2D
			(
				const DvUtils::Math::Vec<float, 4>     *pInColor,
				const GeneratedTexture2DParams         *pInParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11Texture2DResourceType             *pInTypedResourceType
			) : D3D11SequentialGeneratedTextureLoaderBase(pInParams, pInDev, pInRole, pInTypedResourceType),
				_color(*pInColor)
			{
			}

			void D3D11UniformColorTexture2D::_LoadCurrentTexel(void *pInRow, int InX, int InY)
			{
				auto pTypedFormat = GetTextureFormat()->CastToTextureFormat();
				const float Red = DvUtils::Math::GetX(_color);
				const float Green = DvUtils::Math::GetYY(_color);
				const float Blue = DvUtils::Math::GetZZZ(_color);
				const float Alpha = DvUtils::Math::GetWWWW(_color);
				pTypedFormat->SetRowPixel(pInRow, InX, Red, Green, Blue, Alpha);
			}

			/*****************************************************************************************
			* Uninitialized texture creator loader
			******************************************************************************************/
			D3D11Texture2DLoader_CreateUninitialized::D3D11Texture2DLoader_CreateUninitialized
			(
				const GeneratedTexture2DParams          &InTextureParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11Texture2DResourceType             *pInTypedResourceType
			) : D3D11GeneratedTexture2DLoaderBase(&InTextureParams, pInDev, pInRole, pInTypedResourceType),
				_params(InTextureParams)
			{
				Util::EmptyBlockVoid("WE should NOT initialize desc here, as it's already initialized inside the generated texture base class");
				//_desc.Width = InTextureParams.width;
				//_desc.Height = InTextureParams.height;
			}

			bool D3D11Texture2DLoader_CreateUninitialized::_TryGenerateSourceData(LoadedTextureSourceData *pOutTexture)
			{
				Util::EmptyBlockVoid("This'is uninitialized loader, so we should not load data");
				return true;
			}			

			/*****************************************************************************************
			* Texture 2D loader from file
			******************************************************************************************/
			D3D11Texture2DLoaderFromFile::D3D11Texture2DLoaderFromFile
			(
				const std::string&                      InFilename,
				const Texture2DParams                  &InTextureParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11Texture2DResourceType             *pInTypedResourceType
			) : D3D11Texture2DLoaderBase(InFilename, InTextureParams, pInDev, pInRole, pInTypedResourceType)
			{
				Initialize_Texture2D_Desc(&_desc, InTextureParams);
			}

			bool D3D11Texture2DLoaderFromFile::_TryLoadSourceData(LoadedTextureSourceData *pOutTexture)
			{
				wxImage img;
				img.LoadFile(_GetURL());
				if (!img.IsOk())
				{
					return false;
				}
				FillTextureSourceData_wxImage(pOutTexture, _textureParams.pTextureFormat->CastToTextureFormat(), &img);
				return true;
			}

			/*****************************************************************************************
			* LoadedTextureSourceData utils implementation
			*****************************************************************************************/
			void FillTextureSourceData_wxImage(LoadedTextureSourceData *pOutSourceData, const TextureFormat *pInTextureFormat, const wxImage *pSourceImg)
			{
				assert(pOutSourceData != nullptr);
				assert(pSourceImg != nullptr);
				
				size_t pitchBetweenRows = pInTextureFormat->CalcPitchBetweenRows(pSourceImg->GetWidth());
				
				pOutSourceData->Reset(pitchBetweenRows, pSourceImg->GetWidth(), pSourceImg->GetHeight(), pInTextureFormat->GetPixelSize());
				for (size_t Y = 0; Y < pSourceImg->GetHeight(); Y++)
				{
					BYTE *pRow = pOutSourceData->GetData() + Y * pitchBetweenRows;
					for (size_t X = 0; X < pSourceImg->GetWidth(); X++)
					{
						float red           = pSourceImg->GetRed(X, Y);
						float green         = pSourceImg->GetGreen(X, Y);
						float blue          = pSourceImg->GetBlue(X, Y);
						float alpha         = 1.0F;
						if (pSourceImg->HasAlpha())
						{
							alpha = pSourceImg->GetAlpha(X, Y);
						}
						pInTextureFormat->SetRowPixel(pRow, X, red, green, blue, alpha);
					}
				}
			}

			/**********************************************************************************************************
			* LoadedTextureSourceData implemetation
			**********************************************************************************************************/
			LoadedTextureSourceData::LoadedTextureSourceData() :
				_width(0), _height(0), _pSourceData(nullptr)
			{
				ZeroMemory(&_initialSubresourceData, sizeof(_initialSubresourceData));
				_initialSubresourceData.pSysMem = nullptr;
				_initialSubresourceData.SysMemPitch = 0;
				_initialSubresourceData.SysMemSlicePitch = 0;
			}

			void LoadedTextureSourceData::Reset(size_t InPitchBetweenRows, size_t InWidth, size_t InHeight, size_t InPixelSize)
			{
				_width = InWidth;
				_height = InHeight;
				_initialSubresourceData.SysMemPitch = InPitchBetweenRows;
				// WARNING!!! Here we should NOT multiply with the pixel size, because PitchBetweenRows already contains pixelSize factor accounted!!!
				size_t destDataSize = InWidth * InPitchBetweenRows;
				_pSourceData.reset(new BYTE[destDataSize]);
				_initialSubresourceData.pSysMem = _pSourceData.get();
			}

			void LoadedTextureSourceData::Unload()
			{
				_width                                         = 0;
				_height                                        = 0;
				_pSourceData.reset(nullptr);
				_initialSubresourceData.pSysMem                = nullptr;
				_initialSubresourceData.SysMemPitch            = 0;
				_initialSubresourceData.SysMemSlicePitch       = 0;
			}

			/**********************************************************************************************************
			* ResTypeDescr_Texture2D implemetation
			**********************************************************************************************************/
			ResTypeDescr_Texture2D::ResTypeDescr_Texture2D() :
				TypedD3D11ResTypeDescrBase("Texture2D", "Texture")
			{
			}

			void ResTypeDescr_Texture2D::RegisterApplicableRoles(ResMan::IResourceManagerSubsystem *pInSubsys)
			{
				_RegisterDefaultRoles(pInSubsys);
				_roles.pSourceData = ResMan::RegisterSubresourceRole_ThatMustBeLoaded(pInSubsys, _params.GetSubresourceRolesPtr(), "SourceData");
			}

			/****************************************************************************
			* INITIALIZATION
			******************************************************************************/
			void Initialize_Texture2DResource_Class()
			{
				wxInitAllImageHandlers();
			}
		}
	}
} // Dv