#include "../H/D3D11UnorderedAccessView.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/***********************************************************************************************
			* D3D11DepthStencilViewLoader implemenetation
			************************************************************************************************/
			D3D11UnorderedAccessViewLoader::D3D11UnorderedAccessViewLoader
			(
				D3D11Texture2DResource                     InResource,
				const D3D11_UNORDERED_ACCESS_VIEW_DESC    *pInNullOrDesc,
				ID3D11Device                              *pInDev,
				ResMan::ISubresourceRole                  *pInRole,
				D3D11UnorderedAccessViewResourceType      *pInTypedResourceType
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

			D3D11UnorderedAccessViewLoader::RealResourceType *D3D11UnorderedAccessViewLoader::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				if (pOldPtrOrNull != nullptr)
				{
					pOldPtrOrNull->AddRef();
					return pOldPtrOrNull;
				}
				else
				{
					ID3D11UnorderedAccessView *pView = nullptr;
					D3D11_UNORDERED_ACCESS_VIEW_DESC *pDescArg = nullptr;
					if (!_bPassNullptrToDesc)
					{
						pDescArg = &_desc;
					}
					HRESULT hr = _GetDev()->CreateUnorderedAccessView(_resource.GetTypedRealResource(), pDescArg, &pView);
					_SetHResult_Create(hr, "ID3D11DeviceContext::CreateUnorderedAccessView failed");
					if (FAILED(hr))
					{
						return nullptr;
					}
					else
					{
						return pView;
					}
				}
			}

			void D3D11UnorderedAccessViewLoader::_ResetDependencyLists()
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
			ResTypeDescr_UnorderedAccessView::ResTypeDescr_UnorderedAccessView() :
				TypedD3D11ResTypeDescrBase("D3D11UnorderedAccessView", "UnorderedAccessView")
			{
			}
		}
	}
}