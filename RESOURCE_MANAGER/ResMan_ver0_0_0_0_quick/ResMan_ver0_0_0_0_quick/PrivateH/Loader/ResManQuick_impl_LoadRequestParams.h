#pragma once

#include "../ResManQuick_impl_System.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			class Impl_ManagedResource;
			class Impl_ManagedGroup;
		}

		namespace Loader
		{
			/****************************************************************************************
			* Parameters of the load request
			****************************************************************************************/
			class LoadRequestParams
			{
			public:
				// Default ctor: initialize with default parameters
				LoadRequestParams                                                                () = default;

				boost::optional<ResMan::LoadGroupCommandID> GetGroupCommandID                 () const { return _groupCommandId; }

				// Resource: cannot be nullptr
				// By default: uninitialized, nullptr
				Impl::Impl_ManagedResource              *GetResource                             () const { return _pResource; }
				void                                     SetResource                             (Impl::Impl_ManagedResource *pInNewValue) { assert(pInNewValue != nullptr);  _pResource = pInNewValue; }

				/***************************************************************************************************************************************
				* Working with groups
				***************************************************************************************************************************************/
				// Group: 
				// If this resource load request is issued to make the group load request, group is NOT nullptr;
				// Otherwise it must be nullptr;
				// By default: nullptr.
				Impl::Impl_ManagedGroup                 *GetGroup                                () const { return _pGroup; }
				void                                     SetGroup                                (Impl::Impl_ManagedGroup *pInNewGroup) { _pGroup = pInNewGroup; }

				void                                     BindGroupCommand                        (ResMan::LoadGroupCommandID InGroupCommandID, Impl::Impl_ManagedGroup *pInGroup);				

				// By default: unitialized value
				ResMan::Urgency                          GetUrgency                              () const { return _urgency; }
				void                                     SetUrgency                              (ResMan::Urgency InNewValue) { _urgency = InNewValue;  }

				// Set of roles to load: can NOT be empty
				// By default: uninitialize state, empty
				ResMan::SubresourceRoleSet               GetSubresourceRolesToLoad                          () const { return _rolesToLoad; }
				void                                     SetRolesToLoad                          (ResMan::SubresourceRoleSet InNewValue) { assert(!InNewValue.IsEmpty()); _rolesToLoad = InNewValue; }
				
				// Should we load only dependency lists or load the subresources also?
				// By default: false
				bool                                    ShouldLoadOnlyDepenendencyLists              () const { return _bLoadOnlyDependencyLists; }
				void                                    SetEnableState_LoadOnlyDependenciyLists      (bool bInValue) { _bLoadOnlyDependencyLists = bInValue; }

				bool                                    IsManual                                     () const { return _bIsManual; }
				void                                    SetIsManual                                  (bool bInIsManual) { _bIsManual = bInIsManual; }

			private:
				void                                     _InitializeGroupCommandID                   (ResMan::LoadGroupCommandID InGroupCommandID) { _groupCommandId = InGroupCommandID; }

				bool                                     _bIsManual = true;
				Impl::Impl_ManagedResource              *_pResource = nullptr;
				Impl::Impl_ManagedGroup                 *_pGroup = nullptr;
				ResMan::Urgency                          _urgency = 0;
				ResMan::SubresourceRoleSet               _rolesToLoad = ResMan::SubresourceRoleSet();
				bool                                     _bLoadOnlyDependencyLists = false;

				boost::optional<ResMan::LoadGroupCommandID> _groupCommandId;
			};
			inline void LoadRequestParams::BindGroupCommand(ResMan::LoadGroupCommandID InGroupCommandID, Impl::Impl_ManagedGroup *pInGroup)
			{
				assert(pInGroup != nullptr);
				_InitializeGroupCommandID(InGroupCommandID);
				_pGroup = pInGroup;
			}
		}
	}
}