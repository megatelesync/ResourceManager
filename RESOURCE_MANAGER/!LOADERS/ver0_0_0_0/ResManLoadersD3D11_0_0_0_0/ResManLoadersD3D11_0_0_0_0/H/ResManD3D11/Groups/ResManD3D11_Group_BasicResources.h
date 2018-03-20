#pragma once

#include "../ResourceTypes/ResManD3D11_ResourceTypes.h"
#include "deploy/ResManD3D11_BasicDeployer_Group_BasicResources.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			class ResManD3D11_Main;

			/***********************************************************************************************************
			* Group of all resources
			*
			* This group contains basic resources (depth stencil buffers, render targets).
			*
			* Resources are to be initialized using the deployer.
			* Deployer is also responsible for reconfiguration of resources (for example, when the window size is changed),
			* that's why the deployer must be kept during the lifetime of the class.
			* By default basic deployer is used.
			***********************************************************************************************************/
			class ResManD3D11_Group_BasicResources
			{
			public:
				/*********************************************************************************************************
				* Initialization using the given deployer
				*
				* By default (if no deployer is provided = nullptr is passed) basic deployer is to be used.
				*********************************************************************************************************/
				void                                                              Initialize                          (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain, IDeployer_Group_BasicResources *pInDeployer = nullptr);

				/*********************************************************************************************************
				* Deployment
				*********************************************************************************************************/
				IDeployer_Group_BasicResources                                  *GetDeployer                          () const { return _pDeployer; }

				/*********************************************************************************************************
				* Event processing
				*********************************************************************************************************/
				void                                                              Notify_WindowSizeChanged            (const DvUtils::Math::IntSize& InNew);

				/*********************************************************************************************************
				* Group getters
				*********************************************************************************************************/
				ResMan::IManagedGroup                                            *GetGroup                            () const { return _pGroup; }

				/*********************************************************************************************************
				* Depth-stencil related
				*********************************************************************************************************/
				D3D11Texture2DResource                                            GetDepthStencilTexture              () const { return _pDepthStencilTexture; }
				D3D11DepthStencilViewResource                                     GetDepthStencilView                 () const { return _pDepthStencilView; }
				D3D11RasterizerStateResource                                      GetRasterizerState                  () const { return _pRasterizerState; }

			private:
				void                                                              _Begin_GroupRegistration(IDeployer_Group_BasicResources *pInDeployer);
				void                                                              _AttachResourceToGroup(ResMan::IManagedResource *pInResource);
				void                                                              _End_GroupRegistration();

				void                                                              _RegisterResources(const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain);

				ResManD3D11_Main                                                 *_pOwnerMain;
				ResMan::IManagedGroup                                            *_pGroup;
				ResMan::CombinableParams                                          _groupCombinableParams;

				D3D11Texture2DResource                                            _pDepthStencilTexture;
				D3D11DepthStencilViewResource                                     _pDepthStencilView;
				D3D11RasterizerStateResource                                      _pRasterizerState;
				BasicDeployer_Group_BasicResources                                _basicDeployer;
				IDeployer_Group_BasicResources                                   *_pDeployer = nullptr;
			};
		}
	}
}
