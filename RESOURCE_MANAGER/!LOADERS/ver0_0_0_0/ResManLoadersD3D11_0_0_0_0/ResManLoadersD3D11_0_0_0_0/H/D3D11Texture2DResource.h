#pragma once

#include "ResManLoadersD3D11_System.h"
#include "utils/TextureFormat.h"
#include "MathTypes/BLAS/MathUtil_BLAS.h"

/*****************************************************************************************************
* TODO:
* 1. Bugs:
* 1.1. Creating uninitialized texture: width and height are not set;
* 1.2. Automatic mip map generation is not performed
* 1.3. procedural flag is not
*****************************************************************************************************/

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{

			/*****************************************************************************************
			* WARNING!!! This function must be called before using the Texture2D resource
			* to initialize the loaders
			*****************************************************************************************/
			void Initialize_Texture2DResource_Class();

			/*****************************************************************************************
			* Resource typedefs
			******************************************************************************************/
			using D3D11Texture2DResource = ResMan::TypedManagedResource<ID3D11Texture2D>;

			/******************************************************************************************
			* Texture2D set of subresource role types
			******************************************************************************************/
			struct ResTypeDescr_Texture2D_Roles : public ResTypeDescr_Roles_Base
			{
				ResMan::ISubresourceRole                 *pSourceData;
			};

			class ResTypeDescr_Texture2D :
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_Texture2D_Roles, ID3D11Texture2D>
			{
			public:
				ResTypeDescr_Texture2D();

				virtual void  RegisterApplicableRoles(ResMan::IResourceManagerSubsystem *pInSubsys)    override;

			private:
			};

			/*****************************************************************************************
			* Typed resource
			******************************************************************************************/
			class D3D11Texture2DResourceType : public ResMan::TypedResourceType<ResTypeDescr_Texture2D>
			{
			public:
			};

			struct Texture2DParams
			{
				bool                         bGenerateMipMaps;
				const TextureFormatBase     *pTextureFormat;
				D3D11_USAGE                  usage;
				UINT                         bindFlags;
				DXGI_SAMPLE_DESC             sample;
				UINT                         CPUAccessFlags;
				UINT                         miscFlags;

				/*************************************************************************************
				* Construction
				**************************************************************************************/
				Texture2DParams(const TextureFormatBase *pInFormat) :
					bGenerateMipMaps(false),
					pTextureFormat(pInFormat),
					usage(D3D11_USAGE_DEFAULT),
					bindFlags(D3D11_BIND_SHADER_RESOURCE),
					CPUAccessFlags(0),
					miscFlags(0)
				{
					sample.Count = 1;
					sample.Quality = 0;
				}
			};

			/*****************************************************************************************
			* Loaded texture source data
			*****************************************************************************************/
			class LoadedTextureSourceData
			{
			public:
				// default ctor: initializes texture data as unloaded
				LoadedTextureSourceData();

				const BYTE                                 *GetData                   () const                                  { return _pSourceData.get(); }
				BYTE                                       *GetData                   ()                                        { return _pSourceData.get(); }

				// Pitch between rows: pitch in BYTES between rows
				size_t                                      GetPitchBetweenRows       () const { return _initialSubresourceData.SysMemPitch; }
				size_t                                      GetWidth                  () const { return _width; }
				size_t                                      GetHeight                 () const { return _height; }
				const D3D11_SUBRESOURCE_DATA                GetSubresourceData        () const { return _initialSubresourceData;  }

				bool                                        IsLoaded                  () const { return _pSourceData.get() != nullptr; }
				void                                        Reset                     (size_t InPitchBetweenRows, size_t InWidth, size_t InHeight, size_t InPixelSize);
				void                                        Unload                    ();

			private:				
				size_t                                      _width;
				size_t                                      _height;
				D3D11_SUBRESOURCE_DATA                      _initialSubresourceData;
				std::unique_ptr<BYTE>                       _pSourceData;
			};

			/*****************************************************************************************
			* Texture 2D loader base
			******************************************************************************************/
			class D3D11Texture2DLoaderBase : public TypedD3D11ResourceLoaderBase<D3D11Texture2DResourceType>
			{
			public:
				/***************************************************************************************
				* CONSTRUCTION 
				***************************************************************************************/
				// Construct texture loader for texture with URL
				D3D11Texture2DLoaderBase
				(
					const std::string&                      InURL,
					const Texture2DParams                  &InTextureParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11Texture2DResourceType             *pInTypedResourceType
				);

				// Construct texture loader for texture without URL
				D3D11Texture2DLoaderBase
				(
					const Texture2DParams                  &InTextureParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11Texture2DResourceType             *pInTypedResourceType
				);

				virtual RealResourceType                  *Typed_CreateResourceAndGet       (RealResourceType *pOldPtrOrNull) override final;

				virtual bool                               LoadTemporaryOwnContent          (ResMan::SubresourceRoleSet InRoleSet) override final;
				virtual void                               UnloadTemporaryOwnContent        (ResMan::SubresourceRoleSet InRoleSet) override final;

			protected:
				/**********************************************************************************************************
				* Helper getters
				**********************************************************************************************************/
				const TextureFormatBase                     *GetTextureFormat              () const { return _textureParams.pTextureFormat; }

				/*********************************************************************************************************
				* This function must be overloaded in the concrete loader subclass,
				* updating the output texture source data by the given pointer,
				* and returning code (false if failure occured)
				**********************************************************************************************************/
				virtual bool                                _TryLoadSourceData              (LoadedTextureSourceData *pOutTexture) =0;

				Texture2DParams                             _textureParams;
				D3D11_TEXTURE2D_DESC                        _desc;
				LoadedTextureSourceData                     _sourceData;
			};

			/*****************************************************************************************
			* Params of all 2D textures that are generatable
			******************************************************************************************/
			struct GeneratedTexture2DParams
			{
				Texture2DParams                      texture;
				UINT                                 width;
				UINT                                 height;

				GeneratedTexture2DParams(const Texture2DParams& InTextureParams) :
					texture(InTextureParams),
					width(0),
					height(0)
				{
				}

				GeneratedTexture2DParams(const TextureFormatBase *pInFormat) :
					texture(pInFormat),
					width(0),
					height(0)
				{
				}
			};

			/*****************************************************************************************
			* Base class of all 2D textures generated dynamically
			******************************************************************************************/
			class D3D11GeneratedTexture2DLoaderBase : public D3D11Texture2DLoaderBase
			{
			public:
				/***************************************************************************************************************
				* Construction
				***************************************************************************************************************/
				D3D11GeneratedTexture2DLoaderBase
				(
					const GeneratedTexture2DParams         *pInParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11Texture2DResourceType             *pInTypedResourceType
				);

				/***************************************************************************************************************
				* Getters
				***************************************************************************************************************/
				const GeneratedTexture2DParams                       *GetGeneratedParams          () const               { return &_params; }

			protected:
				/**********************************************************************************************************
				* Instruction for overloading of data loading:
				*
				* 1. Always override _TryGenerateSourceData and ÒNOT _TryLoadSourceData
				* 2. _TryLoadSourceData automatically setups width and height of the generated texture,
				* so you should not do it manually
				***********************************************************************************************************/
				// WARNING!!! 
				virtual bool                                         _TryLoadSourceData           (LoadedTextureSourceData *pOutTexture) override final;
				virtual bool                                         _TryGenerateSourceData       (LoadedTextureSourceData *pOutTexture) =0;

			private:
				GeneratedTexture2DParams                             _params;
			};

			/****************************************************************************************
			* Base class of all procedural textures that are generated in sequential manner
			* (i.e. each pixel can be calculated directly knowing X and Y)
			*****************************************************************************************/
			class D3D11SequentialGeneratedTextureLoaderBase : public D3D11GeneratedTexture2DLoaderBase
			{
			public:
				/***************************************************************************************************************
				* Construction
				***************************************************************************************************************/
				D3D11SequentialGeneratedTextureLoaderBase
				(
					const GeneratedTexture2DParams         *pInParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11Texture2DResourceType             *pInTypedResourceType
				);

			protected:
				/**************************************************************************************************************************************
				* implementation instruction:
				* 1. Override _LoadCurrentTexel function
				**************************************************************************************************************************************/
				virtual bool                                         _TryGenerateSourceData    (LoadedTextureSourceData *pOutTexture) override final;
				virtual void                                         _LoadCurrentTexel         (void *pInRow, int InX, int InY) =0;

			private:
			};

			/*****************************************************************************************
			* Uniformly-colored generated texture loader
			******************************************************************************************/
			class D3D11UniformColorTexture2D : public D3D11SequentialGeneratedTextureLoaderBase
			{
			public:
				D3D11UniformColorTexture2D
				(
					const DvUtils::Math::Vec<float, 4>     *pInColor,
					const GeneratedTexture2DParams         *pInParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11Texture2DResourceType             *pInTypedResourceType
				);

			protected:
				virtual void                   _LoadCurrentTexel(void *pInRow, int InX, int InY) override final;

			private:
				DvUtils::Math::Vec<float, 4>   _color;
			};

			/*****************************************************************************************
			* Texture 2D loader from file
			******************************************************************************************/
			class D3D11Texture2DLoaderFromFile : public D3D11Texture2DLoaderBase
			{
			public:
				/***************************************************************************************
				* CONSTRUCTION
				***************************************************************************************/
				D3D11Texture2DLoaderFromFile
				(
					const std::string&                      InFilename,
					const Texture2DParams                  &InTextureParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11Texture2DResourceType             *pInTypedResourceType
				);

			protected:
				virtual bool                               _TryLoadSourceData               (LoadedTextureSourceData *pOutTexture) override final;
			};

			/*****************************************************************************************
			* Unitialized texture loader
			******************************************************************************************/
			class D3D11Texture2DLoader_CreateUninitialized : public D3D11GeneratedTexture2DLoaderBase
			{
			public:
				/***************************************************************************************
				* CONSTRUCTION
				***************************************************************************************/
				D3D11Texture2DLoader_CreateUninitialized
				(
					const GeneratedTexture2DParams          &InTextureParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11Texture2DResourceType             *pInTypedResourceType
				);

			protected:
				virtual bool                               _TryGenerateSourceData(LoadedTextureSourceData *pOutTexture) override final;

			private:
				GeneratedTexture2DParams                   _params;
			};
		}
	}
}