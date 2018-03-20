#pragma once

#include "ResManQuick_Fwd.h"
#include "ResManQuick_System.h"
#include "interface/IResourceType.h"

namespace Dv
{
	namespace ResManQuick
	{
		/*********************************************************************************************************
		* TODO:
		* 1. Interface:
		* 1.1. GetApplicableSubresources:
		* 1.1.1. Where to get the applicable subresource role set?
		* 1.1.2. Where to get the what must be loaded role set?
		*********************************************************************************************************/
		class ResourceType : public ResMan::IResourceType
		{
		public:
			ResourceType(ResMan::IResourceTypeDescr *pInDescr, const ResMan::ResourceTypePublicParams& InParams, ResourceTypeIndex InIndex);

			virtual const ResMan::ResourceTypePublicParams&             GetParams                                   () const override        { return _params; }
			virtual ResourceTypeIndex                                   GetIndex                                    () const override        { return _resourceIndex; }
			virtual ResMan::IResourceTypeDescr                         *GetDescr                                    () const override        { return _pDescr; }

		private:
			ResMan::ResourceTypePublicParams                            _params;
			ResourceTypeIndex                                           _resourceIndex;
			ResMan::IResourceTypeDescr                                  *_pDescr;
		};
	}
}