#include "../../../H/ResManD3D11/Deployer/ResManD3D11_BasicDeployer.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			ResManD3D11_BasicDeployer::ResManD3D11_BasicDeployer() :
				_pDeployer_ResourceTypes(&_basicDeployer_resourceTypes)
			{

			}

			const ResManD3D11_ResourceTypes_IDeployer *ResManD3D11_BasicDeployer::Virt_GetDeployer_ResourceTypes() const
			{
				return _pDeployer_ResourceTypes;
			}

			ResManD3D11_ResourceTypes_IDeployer *ResManD3D11_BasicDeployer::Virt_GetDeployer_ResourceTypes()
			{
				return _pDeployer_ResourceTypes;
			}

			const ResManD3D11_ResourceTypes_IDeployer *ResManD3D11_BasicDeployer::Virt_GetDefaultDeployer_ResourceTypes() const
			{
				return &_basicDeployer_resourceTypes;
			}
		}
	}
}