#include "../../../H/ResManD3D11/Groups/ResManD3D11_Groups.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			void ResManD3D11_Groups::Initialize(const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain)
			{
				assert(&InWindowSize);
				assert(pInOwnerMain);
				_pOwnerMain = pInOwnerMain;
				_group_BasicResources.Initialize(InWindowSize, pInOwnerMain);
			}
		}
	}
}