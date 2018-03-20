#pragma once

#include "ResManLoadersD3D11_System.h"
#include "!UTILS/H/utils_com.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*****************************************************************************************
			* Typed resource
			******************************************************************************************/
			using D3D11CommandListResource = ResMan::TypedManagedResource<ID3D11CommandList>;

			/*****************************************************************************************
			* Roles
			******************************************************************************************/
			struct ResTypeDescr_CommandList_Roles : public ResTypeDescr_Roles_Base
			{
			};

			class ResTypeDescr_CommandList : public TypedD3D11ResTypeDescrBase<ResTypeDescr_CommandList_Roles, ID3D11CommandList>
			{
			public:
				ResTypeDescr_CommandList();				

			private:
			};

			/*****************************************************************************************
			* Typed resource type
			******************************************************************************************/
			class D3D11CommandListResourceType : public ResMan::TypedResourceType<ResTypeDescr_CommandList>
			{
			public:
			};

			/*******************************************************************************************
			* Command list loader:
			*
			* Creates new command list by copying the given reference (templ) command list;
			* WARNING!!! The templ command list is OWNED by command list, but NOT AddRef is called before;
			*
			* NOTE:
			* In D3d11 command list interface instances are NOT to be created, just used as-is.
			*******************************************************************************************/
			class D3D11CommandListLoader : public TypedD3D11ResourceLoaderBase<D3D11CommandListResourceType>
			{
			public:
				D3D11CommandListLoader
				(
					ID3D11Device                           *pInDev,		
					ID3D11CommandList                      *pInTemplCommandList,
					ResMan::ISubresourceRole               *pInRole,
					D3D11CommandListResourceType           *pInResourceType
				);

				virtual RealResourceType                  *Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull) override;

			private:
				std::unique_ptr<ID3D11CommandList, Dv::d3d_utils::Releaser> _pTemplCommandList = nullptr;
			};
		}
	}
}