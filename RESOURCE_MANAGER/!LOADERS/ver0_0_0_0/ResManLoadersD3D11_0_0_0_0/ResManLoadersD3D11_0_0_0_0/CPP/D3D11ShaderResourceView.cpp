#include "../H/D3D11ShaderResourceView.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/***********************************************************************************************
			* D3D11ShaderViewLoader implemenetation
			************************************************************************************************/
			D3D11ShaderViewLoader::D3D11ShaderViewLoader
			(				
				ResMan::IManagedResource                 *pInResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC    *pInNullOrDesc,
				ID3D11Device                             *pInDev,
				ResMan::ISubresourceRole                 *pInRole,
				D3D11ShaderViewResourceType              *pInTypedResourceType
			) :
				TypedD3D11ResourceLoaderBase(pInDev, pInRole, pInTypedResourceType),
				_pResource(pInResource),
				_bPassNullptrToDesc(pInNullOrDesc == nullptr)
			{
				assert(pInResource);
				if (!_bPassNullptrToDesc)
				{
					_desc = *pInNullOrDesc;
				}
				_ResetDependencyLists();
			}

			D3D11ShaderViewLoader::RealResourceType *D3D11ShaderViewLoader::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				if (pOldPtrOrNull != nullptr)
				{
					pOldPtrOrNull->AddRef();
					return pOldPtrOrNull;
				}
				else
				{
					ID3D11ShaderResourceView *pResView = nullptr;
					D3D11_SHADER_RESOURCE_VIEW_DESC *pDescArg = nullptr;
					if (!_bPassNullptrToDesc)
					{
						pDescArg = &_desc;
					}
					auto pD3D11Resource = _pResource->GetResource();
					assert(pD3D11Resource);
					HRESULT hr = _GetDev()->CreateShaderResourceView(static_cast<ID3D11Resource *>(pD3D11Resource), pDescArg, &pResView);
					_SetHResult_Create(hr, "ID3D11DeviceContext::CreateShaderResourceView failed");
					if (FAILED(hr))
					{
						return nullptr;
					}
					else
					{
						return pResView;
					}
				}
			}

			void D3D11ShaderViewLoader::LoadDirectDepdendencyListOf(ResMan::SubresourceRoleSet InRoleSet)
			{
				if (InRoleSet.IsSet(_GetDependencyListRoleIndex()))
				{
					_GetDependencyList()->RegisterDependency(_GetDependencyListRoleIndex(), _pResource);
				}
			}

			void D3D11ShaderViewLoader::_ResetDependencyLists()
			{
				const auto& roles = _GetParams_MainSubresource().GetResourceType()->GetParams().GetSubresourceRoles();
				auto subresourceRoleSet = roles.GetApplicableSubresourceRoles();
				_GetDependencyList()->NotifyLoadBegin(subresourceRoleSet);
				_GetDependencyList()->RegisterDependency(_GetDependencyListRoleIndex(), _pResource);
				_GetDependencyList()->NotifyLoadEnd(subresourceRoleSet);
			}
			ResMan::SubresourceRoleIndex D3D11ShaderViewLoader::_GetDependencyListRoleIndex() const
			{
				return GetRoles().pStd->GetRoleIndex();
			}

			/***********************************************************************************************
			* ResTypeDescr_ShaderView implemenetation
			************************************************************************************************/
			ResTypeDescr_ShaderView::ResTypeDescr_ShaderView() :
				TypedD3D11ResTypeDescrBase("D3D11ShaderView", "ShaderView")
			{
			}
		}
	}
}