#pragma once

#include "ResManLoadersD3D11_System.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*********************************************************************************************
			* Pair of compiled vertex and pixel shader
			**********************************************************************************************/
			class D3D11ShaderPair
			{
			public:
				~D3D11ShaderPair();
				D3D11ShaderPair(ID3D11VertexShader *pInVertex, ID3D11PixelShader *pInPixel);

				ID3D11VertexShader                    *GetVertex() const { return _pVertex; }
				ID3D11PixelShader                     *GetPixel() const { return _pPixel; }

			private:
				ID3D11VertexShader                   *_pVertex;
				ID3D11PixelShader                    *_pPixel;
			};

			/*********************************************************************************************
			* Concrete roles class for the shader pair
			**********************************************************************************************/
			struct ResTypeDescr_ShaderPair_Roles : public ResTypeDescr_Roles_Base
			{				
				// Subresource role for loaded blob data
				ResMan::ISubresourceRole             *pTempBlob;
				// Subresource role for loaded source code
				ResMan::ISubresourceRole             *pTempSourceCode;
			};

			/*********************************************************************************************
			* Concrete resource description for the shader pair
			**********************************************************************************************/
			class ResTypeDescr_ShaderPair :
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_ShaderPair_Roles, D3D11ShaderPair>
			{
			public:
				ResTypeDescr_ShaderPair();

				virtual void                        RegisterApplicableRoles(ResMan::IResourceManagerSubsystem *pInSubsys)    override;
				virtual void                        Delete(void *pInResource) override;

			private:
			};


			/*****************************************************************************************
			* Typed resource
			******************************************************************************************/
			class D3D11ShaderPairResourceType : public ResMan::TypedResourceType<ResTypeDescr_ShaderPair>
			{
			public:
			};

			/********************************************************************************************
			* ShaderPair loader base class
			*
			* Subresources
			* All subresources are NOT addressable because they are not loaded from external storage
			* (except source code).
			* The blob temporary subresource data must be loaded to create the shader pair instance.
			********************************************************************************************/
			class D3D11ShaderPairLoaderBase: public TypedD3D11ResourceLoaderBase<D3D11ShaderPairResourceType>
			{
			public:
				D3D11ShaderPairLoaderBase
				(
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11ShaderPairResourceType            *pInResourceType
				);

				virtual RealResourceType *Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull) override;

			private:
			};

			/********************************************************************************************
			* ShaderPair loader that loads shader from the given source code from memory
			*
			* TODO:
			*      Code the class
			********************************************************************************************/
			class D3D11ShaderPairLoaderFromMemory : public D3D11ShaderPairLoaderBase
			{
			public:				
				D3D11ShaderPairLoaderFromMemory
				(
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11ShaderPairResourceType            *pInResourceType
				);

				virtual RealResourceType *Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull) override;

			private:
			};
		}
	}
}