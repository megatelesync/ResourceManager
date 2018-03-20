#pragma once

#include "ResManLoadersD3D11_System.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*****************************************************************************************
			* Typed resource
			******************************************************************************************/
			using D3D11BufferResource = ResMan::TypedManagedResource<ID3D11Buffer>;

			/*****************************************************************************************
			* Roles
			******************************************************************************************/
			struct ResTypeDescr_Buffer_Roles : public ResTypeDescr_Roles_Base
			{
				// role of the source data from RAM
				// (source data can be unloaded)
				ResMan::ISubresourceRole             *pSourceData;
			};

			class ResTypeDescr_Buffer : public TypedD3D11ResTypeDescrBase<ResTypeDescr_Buffer_Roles,ID3D11Buffer>
			{
			public:
				ResTypeDescr_Buffer();

				virtual void                        RegisterApplicableRoles(ResMan::IResourceManagerSubsystem *pInSubsys)    override;

			private:
			};

			/*****************************************************************************************
			* Typed resource type
			******************************************************************************************/
			class D3D11BufferResourceType : public ResMan::TypedResourceType<ResTypeDescr_Buffer>
			{
			public:
			};

			/*******************************************************************************************
			* Buffer loader:
			*
			* Creates buffer from the given arguments;
			* Initial content may be specified; 
			* If initial content is specified, it will be copied and owned by the loader.
			*
			* TODO:
			* 1. Should "WhatMustBeLoaded" return role of the memory content?
			* 1.1. Yes, it must, because it would be wrong to create an empty buffer
			* if the the resource was created as containing content
			*******************************************************************************************/
			class D3D11BufferLoader : public TypedD3D11ResourceLoaderBase<D3D11BufferResourceType>
			{
			public:
				D3D11BufferLoader
				(
					const D3D11_SUBRESOURCE_DATA           *pInNullOrInitialContent,
					const D3D11_BUFFER_DESC                &InBufferDesc,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11BufferResourceType                *pInResourceType
				);

				virtual RealResourceType                  *Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull) override;

				/******************************************************************************************************
				* Unloading data: temporary memory source data can be unloaded.
				******************************************************************************************************/
				virtual void                               UnloadTemporaryOwnContent             (ResMan::SubresourceRoleSet InRoleSet) override;


			private:
				bool                                        _ShouldLoadInitialData() const;
				
				D3D11_SUBRESOURCE_DATA                      _initial_subresource_data;
				D3D11_BUFFER_DESC                           _buffer_desc;
				// if _pInitialContent is nullptr, then should not load initial data
				std::unique_ptr<BYTE[]>                     _pInitialContent;
			};
		}
	}
}