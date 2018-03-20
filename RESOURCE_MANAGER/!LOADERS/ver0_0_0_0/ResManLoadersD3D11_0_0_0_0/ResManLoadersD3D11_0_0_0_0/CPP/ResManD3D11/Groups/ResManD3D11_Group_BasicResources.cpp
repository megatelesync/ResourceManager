#include "../../../H/ResManD3D11/Groups/ResManD3D11_Group_BasicResources.h"
#include "../../../H/ResManD3D11/ResManD3D11.h"


namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			void ResManD3D11_Group_BasicResources::Initialize(const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain, IDeployer_Group_BasicResources *pInDeployer)
			{
				assert(pInOwnerMain);
				_pOwnerMain = pInOwnerMain;

				// WARNING: Deployer can be nullptr
				if (pInDeployer)
				{
					_pDeployer = pInDeployer;
				}
				else
				{
					_pDeployer = &_basicDeployer;
				}

				_Begin_GroupRegistration(_pDeployer);
				_RegisterResources(InWindowSize, pInOwnerMain);
				_End_GroupRegistration();

				_pGroup->PrecacheSynchro();
			}
			void ResManD3D11_Group_BasicResources::Notify_WindowSizeChanged(const DvUtils::Math::IntSize& InNew)
			{
				assert(&InNew);
				_pDeployer->UpdateResources(InNew, _pOwnerMain, this);
			}

			void ResManD3D11_Group_BasicResources::_RegisterResources(const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain)
			{
				_pDepthStencilTexture = _pDeployer->RegisterDepthStencilTexture(InWindowSize, pInOwnerMain);
				_AttachResourceToGroup(_pDepthStencilTexture.GetManagedResource());
				_pDepthStencilView = _pDeployer->RegisterDepthStencilView(InWindowSize, _pDepthStencilTexture, pInOwnerMain);
				_AttachResourceToGroup(_pDepthStencilView.GetManagedResource());
				_pRasterizerState = _pDeployer->RegisterRasterizerState(InWindowSize, pInOwnerMain);
				_AttachResourceToGroup(_pRasterizerState.GetManagedResource());
			}

			void ResManD3D11_Group_BasicResources::_Begin_GroupRegistration(IDeployer_Group_BasicResources *pInDeployer)
			{
				ResMan::GroupParams groupParams = pInDeployer->GetGroupParams();
				// WARNING!!! We copy params to group combinable params
				_groupCombinableParams = *groupParams.GetCombinablePtr();
				_pGroup = _pOwnerMain->GetSubsystem()->GetManager()->RegisterGroup(groupParams);
			}
			void ResManD3D11_Group_BasicResources::_AttachResourceToGroup(ResMan::IManagedResource *pInResource)
			{
				_pGroup->AttachResource(pInResource);

				auto rolesToAdd = ResMan::GetApplicableSubresourceRoles(pInResource);

				auto subresourceRolesToKeepLoaded                             = _groupCombinableParams.GetSubresourceRolesToKeepLoaded();
				subresourceRolesToKeepLoaded.MergeWith(rolesToAdd);
				_groupCombinableParams.SetSubresourceRolesToKeepLoaded(subresourceRolesToKeepLoaded);

				auto subresourceRolesToLoad                                   = _groupCombinableParams.GetSubresourceRolesToLoad();
				subresourceRolesToLoad.MergeWith(rolesToAdd);
				_groupCombinableParams.SetSubresourceRolesToLoad(subresourceRolesToLoad);
			}
			void ResManD3D11_Group_BasicResources::_End_GroupRegistration()
			{
				_pGroup->UpdateCombinableParams(_groupCombinableParams);
			}
		}
	}
}