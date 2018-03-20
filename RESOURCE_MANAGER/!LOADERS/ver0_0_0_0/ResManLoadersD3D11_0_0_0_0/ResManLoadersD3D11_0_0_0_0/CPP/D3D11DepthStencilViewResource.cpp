#include "../H/D3D11DepthStencilViewResource.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/***********************************************************************************************
			* D3D11DepthStencilViewLoader implemenetation
			************************************************************************************************/
			D3D11DepthStencilViewLoader::D3D11DepthStencilViewLoader
			(
				D3D11Texture2DResource                         InResource,
				const D3D11_DEPTH_STENCIL_VIEW_DESC    *pInNullOrDesc,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11DepthStencilViewResourceType      *pInTypedResourceType
			) : 
				TypedD3D11ResourceLoaderBase(pInDev, pInRole, pInTypedResourceType),
				_resource(InResource),
				_bPassNullptrToDesc(pInNullOrDesc == nullptr)
			{
				if(!_bPassNullptrToDesc)
				{
					_desc = *pInNullOrDesc;
				}
				_ResetDependencyLists();
			}

			D3D11DepthStencilViewLoader::RealResourceType *D3D11DepthStencilViewLoader::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				if (pOldPtrOrNull != nullptr)
				{
					pOldPtrOrNull->AddRef();
					return pOldPtrOrNull;
				}
				else
				{
					ID3D11DepthStencilView *pResDepthStencilView = nullptr;
					D3D11_DEPTH_STENCIL_VIEW_DESC *pDescArg = nullptr;
					if (!_bPassNullptrToDesc)
					{
						pDescArg = &_desc;
					}
					HRESULT hr = _GetDev()->CreateDepthStencilView(_resource.GetTypedRealResource(), pDescArg, &pResDepthStencilView);
					_SetHResult_Create(hr, "ID3D11DeviceContext::CreateDepthStencilView failed");
					if (FAILED(hr))
					{
						return nullptr;
					}
					else
					{
						return pResDepthStencilView;
					}
				}
			}

			void D3D11DepthStencilViewLoader::_ResetDependencyLists()
			{
				const auto& roles = _GetParams_MainSubresource().GetResourceType()->GetParams().GetSubresourceRoles();
				auto subresourceRoleSet = roles.GetApplicableSubresourceRoles();
				_GetDependencyList()->NotifyLoadBegin(subresourceRoleSet);
				_GetDependencyList()->RegisterDependency(GetRoles().pStd->GetRoleIndex(), _resource.GetManagedResource());
				_GetDependencyList()->NotifyLoadEnd(subresourceRoleSet);
			}

			/***********************************************************************************************
			* ResTypeDescr_DepthStencilView implemenetation
			************************************************************************************************/
			ResTypeDescr_DepthStencilView::ResTypeDescr_DepthStencilView() :
				TypedD3D11ResTypeDescrBase("D3D11DepthStencilView", "DepthStencilView")
			{
			}
		}
	}
} // Dv