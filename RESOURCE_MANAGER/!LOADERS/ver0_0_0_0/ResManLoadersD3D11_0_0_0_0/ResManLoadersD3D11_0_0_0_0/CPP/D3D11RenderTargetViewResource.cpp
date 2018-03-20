#include "../H/D3D11RenderTargetViewResource.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/***********************************************************************************************
			* D3D11RenderTargetViewLoader implemenetation
			************************************************************************************************/
			D3D11RenderTargetViewLoader::D3D11RenderTargetViewLoader
			(
				D3D11Texture2DResource                         InResource,
				const D3D11_RENDER_TARGET_VIEW_DESC           *pInNullOrDesc,
				ID3D11Device                                  *pInDev,
				ResMan::ISubresourceRole                      *pInRole,
				D3D11RenderTargetViewResourceType             *pInTypedResourceType
			) :
				TypedD3D11ResourceLoaderBase(pInDev, pInRole, pInTypedResourceType),
				_resource(InResource),
				_bPassNullptrToDesc(pInNullOrDesc == nullptr)
			{
				if (!_bPassNullptrToDesc)
				{
					_desc = *pInNullOrDesc;
				}
				_ResetDependencyLists();
			}

			D3D11RenderTargetViewLoader::RealResourceType *D3D11RenderTargetViewLoader::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				if (pOldPtrOrNull != nullptr)
				{
					pOldPtrOrNull->AddRef();
					return pOldPtrOrNull;
				}
				else
				{
					ID3D11RenderTargetView *pResRenderTargetView = nullptr;
					D3D11_RENDER_TARGET_VIEW_DESC *pDescArg = nullptr;
					if (!_bPassNullptrToDesc)
					{
						pDescArg = &_desc;
					}
					HRESULT hr = _GetDev()->CreateRenderTargetView(_resource.GetTypedRealResource(), pDescArg, &pResRenderTargetView);
					_SetHResult_Create(hr, "ID3D11DeviceContext::CreateRenderTargetView failed");
					if (FAILED(hr))
					{
						return nullptr;
					}
					else
					{
						return pResRenderTargetView;
					}
				}
			}

			void D3D11RenderTargetViewLoader::_ResetDependencyLists()
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
			ResTypeDescr_RenderTargetView::ResTypeDescr_RenderTargetView() :
				TypedD3D11ResTypeDescrBase("D3D11RenderTargetView", "RenderTargetView")
			{
			}
		}
	}
} // Dv