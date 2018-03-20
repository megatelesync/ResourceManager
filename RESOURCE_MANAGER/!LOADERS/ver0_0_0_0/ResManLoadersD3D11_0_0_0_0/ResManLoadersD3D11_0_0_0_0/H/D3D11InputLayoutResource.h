#pragma once

#include "ResManLoadersD3D11_System.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/******************************************************************************************
			* Typed resource
			******************************************************************************************/
			using D3D11InputLayoutResource = ResMan::TypedManagedResource<ID3D11InputLayout>;

			/******************************************************************************************
			* Input layout set of subresource role types
			******************************************************************************************/
			struct ResTypeDescr_InputLayout_Roles : public ResTypeDescr_Roles_Base
			{
			};

			class ResTypeDescr_InputLayout : 
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_InputLayout_Roles, ID3D11InputLayout>
			{ 
			public:
				ResTypeDescr_InputLayout();

			private:
			};

			/*****************************************************************************************
			* Typed resource type
			******************************************************************************************/
			class D3D11InputLayoutResourceType : public ResMan::TypedResourceType<ResTypeDescr_InputLayout>
			{
			public:
			};

			/******************************************************************************************
			* Loads the given input layout from the given layout description
			*
			* TODO:
			* 1. How to return functions that should not return value:
			* - GetAddressableSubresourceRoles();
			* - GetDependencyList();
			* 2. The input loader constructor must initialize the _mainSubresourceParams
			* with pointer to the registered resource type:
			* 2.1. SOLUTION: Pass the D3D11InputLayoutResourceType
			* 3. The input layout is dependent on vertex shader bytecode, 
			*    but how to keep it?
			******************************************************************************************/
			class D3D11InputLayoutLoader : public TypedD3D11ResourceLoaderBase<D3D11InputLayoutResourceType>
			{
			public:
				/***************************************************************************************
				* CONSTRUCTION:
				*    The given input layout description will be copied;
				*    The given shader bytecode will be copied;
				***************************************************************************************/
				D3D11InputLayoutLoader
				(
					const D3D11_INPUT_ELEMENT_DESC         *pInLayoutDesc,
					UINT                                    InNumDescElems,
					const void                             *pInShaderBytecode,
					SIZE_T                                  InByteCodeLength,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11InputLayoutResourceType           *pInTypedResourceType
				);
				

				virtual RealResourceType *Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull) override;
				

			private:				
				std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]>             _pLayoutDesc;
				UINT                                                    _numDescElems;
				std::unique_ptr<BYTE[]>                                  _pShaderBytecode;
				SIZE_T                                                   _shaderBytecodeLength;
			};
		}
	}
} // Dv