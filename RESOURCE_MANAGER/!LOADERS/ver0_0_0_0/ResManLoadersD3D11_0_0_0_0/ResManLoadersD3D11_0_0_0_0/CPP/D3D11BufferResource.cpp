#pragma once

#include "../H/D3D11BufferResource.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/**************************************************************************************
			* D3D11BufferLoader implementation
			**************************************************************************************/
			D3D11BufferLoader::D3D11BufferLoader
			(
				const D3D11_SUBRESOURCE_DATA           *pInNullOrInitialContent,
				const D3D11_BUFFER_DESC                &InBufferDesc,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11BufferResourceType                *pInResourceType
			) :
				TypedD3D11ResourceLoaderBase(pInDev, pInRole, pInResourceType)
			{
				assert(&InBufferDesc);
				_buffer_desc = InBufferDesc;

				if (pInNullOrInitialContent != nullptr)
				{
					_initial_subresource_data = *pInNullOrInitialContent;
					_pInitialContent.reset(new BYTE[InBufferDesc.ByteWidth]);
					_initial_subresource_data.pSysMem = _pInitialContent.get();
					_MarkTempContentRole_Loaded(GetRoles().pSourceData);
				}
				else
				{
					ZeroMemory(&_initial_subresource_data, sizeof(_initial_subresource_data));
				}
			}

			typename D3D11BufferLoader::RealResourceType *D3D11BufferLoader::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				if (pOldPtrOrNull != nullptr)
				{
					// We have a working object already, so we just use it;
					// Because the old object will be unloaded, we increment the ref counter
					// to prevent it from being destroyed.
					pOldPtrOrNull->AddRef();
					return pOldPtrOrNull;
				}
				else
				{
					ID3D11Buffer *pResBuffer;
					D3D11_SUBRESOURCE_DATA *pSubresourceDataArg = nullptr;
					if (_ShouldLoadInitialData())
					{
						pSubresourceDataArg = &_initial_subresource_data;
					}
					HRESULT hr_create = _GetDev()->CreateBuffer(&_buffer_desc, pSubresourceDataArg, &pResBuffer);
					_SetHResult_Create(hr_create, "ID3D11DeviceContext::CreateBuffer failed");
					if (FAILED(_GetHResult_Create()))
					{
						return nullptr;
					}
					return pResBuffer;
				}
			}

			void D3D11BufferLoader::UnloadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				auto pRole = GetRoles().pSourceData;
				if (_ShouldLoadInitialData())
				{
					if (InRoleSet.IsSet(pRole))
					{
						_pInitialContent.reset();
						_MarkTempContentRole_Unloaded(pRole);
					}
				}
			}

			bool D3D11BufferLoader::_ShouldLoadInitialData() const
			{
				return _pInitialContent.get() != nullptr;
			}

			/**************************************************************************************
			* ResTypeDescr_Buffer implementation
			**************************************************************************************/
			ResTypeDescr_Buffer::ResTypeDescr_Buffer() :
				TypedD3D11ResTypeDescrBase("ID3D11BufferResource", "Buffer")
			{
			}

			void ResTypeDescr_Buffer::RegisterApplicableRoles(ResMan::IResourceManagerSubsystem *pInSubsys)
			{
				_RegisterDefaultRoles(pInSubsys);
				_roles.pSourceData   = ResMan::RegisterSubresourceRole_ThatMustBeLoaded(pInSubsys, _params.GetSubresourceRolesPtr(), "std");
			}
		}
	}
} // Dv