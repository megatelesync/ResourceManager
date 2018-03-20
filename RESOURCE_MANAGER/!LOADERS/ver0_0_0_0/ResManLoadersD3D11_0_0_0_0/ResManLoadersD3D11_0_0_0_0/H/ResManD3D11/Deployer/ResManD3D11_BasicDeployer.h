#pragma once

#include "ResManD3D11_IDeployer.h"
#include "../ResourceTypes/Deployer/ResManD3D11_BasicDeployer_ResourceTypes.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			class ResManD3D11_BasicDeployer : public ResManD3D11_IDeployer
			{
			public:
				ResManD3D11_BasicDeployer();

			protected:
				virtual const ResManD3D11_ResourceTypes_IDeployer        *Virt_GetDeployer_ResourceTypes                     () const override;
				virtual ResManD3D11_ResourceTypes_IDeployer              *Virt_GetDeployer_ResourceTypes                     () override;
				virtual const ResManD3D11_ResourceTypes_IDeployer        *Virt_GetDefaultDeployer_ResourceTypes              () const override;

			private:
				ResManD3D11_ResourceTypes_BasicDeployer                  _basicDeployer_resourceTypes;
				ResManD3D11_ResourceTypes_IDeployer                     *_pDeployer_ResourceTypes;
			};
		}
	}
}