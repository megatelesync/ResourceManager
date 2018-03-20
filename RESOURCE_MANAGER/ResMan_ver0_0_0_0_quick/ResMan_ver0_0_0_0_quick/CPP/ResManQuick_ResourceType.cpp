#pragma once

#include "../H/ResManQuick_ResourceType.h"

namespace Dv
{
	namespace ResManQuick
	{
		ResourceType::ResourceType(ResMan::IResourceTypeDescr *pInDescr, const ResMan::ResourceTypePublicParams& InParams, ResourceTypeIndex InIndex) :
			_pDescr(pInDescr),
			_params(InParams),
			_resourceIndex(InIndex)
		{
			assert(pInDescr != nullptr);
		}
	}
}