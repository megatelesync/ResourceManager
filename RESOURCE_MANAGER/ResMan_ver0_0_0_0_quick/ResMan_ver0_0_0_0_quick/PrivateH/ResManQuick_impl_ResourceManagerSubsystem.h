#pragma once

#include "../H/ResManQuick_SubsystemParams.h"

#include "ResManQuick_impl_access_ResourceManager.h"
#include "ResManQuick_impl_ResourceManager.h"
#include "../H/ResManQuick_ResourceType.h"
#include "../H/ResManQuick_SubresourceRole.h"
#include "../PrivateH/Loader/ResManQuick_impl_LoaderMain.h"

#include "Fwd\ResManQuick_Fwd_ManagedResource.h"

#include "../PrivateH/Log/ResManQuick_impl_Log.h"

namespace Dv
{
	namespace ResManQuick
	{

		namespace Impl
		{
			/**************************************************************************************************************************************
			* Contains resource types and performs logging of resource type registrations.
			**************************************************************************************************************************************/
			class Impl_ResourceTypeContainer
			{
			public:
				/**********************************************************************************************************************************
				* Typedefs
				**********************************************************************************************************************************/
				using ContType = std::vector<std::unique_ptr<ResMan::IResourceType>>;

				/**********************************************************************************************************************************
				* Construction
				**********************************************************************************************************************************/
				// user dtor: for logging
				~Impl_ResourceTypeContainer();
				Impl_ResourceTypeContainer                                                                                     (Impl_ResourceManagerSubsystem *pInOwnerSubsystem);

				/**********************************************************************************************************************************
				* Getters
				**********************************************************************************************************************************/
				Impl_ResourceManagerSubsystem                                   *GetOwnerSubsystem                             () const { return _pOwnerSubsystem; }

				/**********************************************************************************************************************************
				* Registering types
				**********************************************************************************************************************************/
				ResMan::IResourceType                                           *RegisterResourceType                          (ResMan::IResourceTypeDescr *pInType);

				/**********************************************************************************************************************************
				* Enumeration
				**********************************************************************************************************************************/
				size_t                                                           GetCount                                      () const { return _resourceTypes.size(); }
				const ContType&                                                  GetResourceTypes                              () const { return _resourceTypes; }

			private:
				ContType                                                         _resourceTypes;
				Impl_ResourceManagerSubsystem                                    *_pOwnerSubsystem;
			};

			/**************************************************************************************************************************************
			* Contains subresource roles and performs logging of subresource role registrations.
			**************************************************************************************************************************************/
			class Impl_SubresourceRoleContainer
			{
			public:
				/**************************************************************************************************************************************
				* Typedefs
				**************************************************************************************************************************************/
				using ContType = std::vector<std::unique_ptr<SubresourceRole>>;

				/**************************************************************************************************************************************
				* Construction and destruction
				**************************************************************************************************************************************/
				~Impl_SubresourceRoleContainer                                                                                  ();
				Impl_SubresourceRoleContainer                                                                                   (Impl_ResourceManagerSubsystem *pInOwnerSubsystem);

				/**************************************************************************************************************************************
				* Getters
				**************************************************************************************************************************************/
				Impl_ResourceManagerSubsystem                                   *GetOwnerSubsystem                              () const { return _pOwnerSubsystem; }

				/**************************************************************************************************************************************
				* Registration
				**************************************************************************************************************************************/
				ResMan::ISubresourceRole                                        *RegisterSubresourceRole                        (const ResMan::SubresourceRolePublicParams& InParams);

				/**************************************************************************************************************************************
				* Enumeration
				**************************************************************************************************************************************/
				size_t                                                           GetCount                                       () const { return _subresourceRoles.size(); }
				const ContType&                                                  GetSubresourceRoles                            () const { return _subresourceRoles; }

			private:
				ContType                                                         _subresourceRoles;
				Impl_ResourceManagerSubsystem                                    *_pOwnerSubsystem;
			};

			class Impl_ResourceManagerSubsystem
			{
			public:
				/*********************************************************************************************************
				* SPECIAL FUNCTIONS
				*********************************************************************************************************/
				~Impl_ResourceManagerSubsystem                                                               ();
				Impl_ResourceManagerSubsystem                                                                (const ResManQuick_SubsystemParams& InParams, ResMan::IResourceManagerSubsystem *pInSubsystem);

				/*********************************************************************************************************
				* SYSTEM GETTERS
				*********************************************************************************************************/
				ResMan::IResourceManagerSubsystem                              *GetSubsystem                  () const { return _pOwnerSubsystem; }
				Impl::Impl_ResourceManager                                     *GetManager                    ()       { return &_manager; }
				const Impl::Impl_ResourceManager                               *GetManager                    () const { return &_manager; }

				/*********************************************************************************************************
				* INITIALIZATION
				*********************************************************************************************************/
				void                                                            Initialize                     ();
				void                                                            Stop                           ();

				/*********************************************************************************************************
				* GETTERS
				*********************************************************************************************************/

				double                                                          GetSimulationTime              () const                 { return _simulationTime; }
				size_t                                                          GetTickIndex                   () const                 { return _tickIndex; }

				/*******************************************************************************************
				* Logging
				*******************************************************************************************/
				Impl::Logger                                                    *GetLogger_MainThread          () { return &_mainThreadLogger; }

				/*********************************************************************************************************
				* PARAMETERS
				*********************************************************************************************************/
				const ResManQuick_SubsystemParams&                              GetParams                      () const                 { return _params; }
				const ResMan::ResourceManagerPublicRuntimeParams&               GetRuntimeParams               () const                 { return _runtimeParams; }
				void                                                            UpdateRuntimeParams            (const ResMan::ResourceManagerPublicRuntimeParams& InParams);

				/*********************************************************************************************************
				* MAIN LOOP
				*********************************************************************************************************/
				void                                                            Tick                           (double InDeltaSeconds);

				ResMan::IResourceType                                          *RegisterResourceType           (ResMan::IResourceTypeDescr *pInType);
				ResMan::ISubresourceRole                                       *RegisterRole                   (const ResMan::SubresourceRolePublicParams& InSubresourceRoleParams);

				const Impl_ResourceTypeContainer::ContType&                     GetResourceTypes              () const { return _resourceTypes.GetResourceTypes(); }
				const Impl_SubresourceRoleContainer::ContType&                  GetSubresourceRoles           () const { return _subresourceRoles.GetSubresourceRoles(); }

				/***************************************************************************************************************
				* Loading resources
				***************************************************************************************************************/
				void                                                            PrecacheGroupResource                           (Impl::Impl_ManagedGroup *pInGroup, ResMan::LoadGroupCommandID InGroupCommandID, Impl::Impl_ManagedResource *pInResource, ResMan::Urgency InUrgency, bool bIsManual);
				void                                                            PrecacheGroupResourceDependencies               (Impl::Impl_ManagedGroup *pInGroup, ResMan::LoadGroupCommandID InGroupCommandID, Impl::Impl_ManagedResource *pInResource, ResMan::Urgency InUrgency, bool bIsManual);
				void                                                            PrecacheGroupResourceSynchro                    (Impl::Impl_ManagedGroup *pInGroup, ResMan::LoadGroupCommandID InGroupCommandID, Impl::Impl_ManagedResource *pInResource, bool bIsManual);

				void                                                            PrecacheResource                                (Impl::Impl_ManagedResource *pInResource, const ResMan::LoadCommand& InCommand, bool bIsManual);
				void                                                            PrecacheResourceSynchro                         (Impl::Impl_ManagedResource *pInResource, const ResMan::LoadCommand& InCommand, bool bIsManual);
				void                                                            PrecacheResourceDependencyLists                 (Impl::Impl_ManagedResource *pInResource, const ResMan::PrecacheDependenciesCommand& InCommand, bool bIsManual);
				void                                                            PrecacheResourceDependencyListsSynchro          (Impl::Impl_ManagedResource *pInResource, const ResMan::PrecacheDependenciesCommand& InCommand, bool bIsManual);

				/*******************************************************************************************
				* Waiting for events
				*******************************************************************************************/
				// Wait until all commands running on all loading loops processed.
				void                                                            Wait_UntilAllCommandsProcessed                  ();
				void                                                            Wait_UntilAllResourceCommandsProcessed          (ResMan::IManagedResource *pInResource, bool bInRecurisve);
				// Wait until all commands for the given group are processed;
				// If bInRecurisve is true, then child commands are also waited for to complete.
				void                                                            Wait_UntilAllGroupCommandsProcessed             (ResMan::IManagedGroup *pInGroup, bool bInRecurisve);

				// Wait until all commands that run NOT on the main thread have been processed.
				// WARNING!!! The non-threaded child commands will not be waited for to complete.
				void                                                            Wait_UntilAllThreadedCommandsProcessed          ();
				// Wait until all commands connected with the given resource that run NOT on the main thread have been processed.
				// WARNING!!! The non-threaded child commands will not be waited for to complete.
				void                                                            Wait_UntilAllThreadedCommandsForResourceProcessed(ResMan::IManagedResource *pInResource, bool bInRecursive);

			private:
				const ResManQuick_SubsystemParams                                _params;

				ResMan::IResourceManagerSubsystem                                *_pOwnerSubsystem;
				std::unique_ptr<ResourceManager>                                  _pManager;

				Impl::Logger                                                     _mainThreadLogger;

				// Warning!!! Resource types contain subresource roles, so they must be destroyed last!!!
				Impl_ResourceTypeContainer                                       _resourceTypes;

				// WARNING: Resource types and subresource roles must be initialized BEFORE the manager!!!
				// (to be destroyed after the manager is destroyed)
				Impl_SubresourceRoleContainer                                    _subresourceRoles;

				Loader::LoaderMain                                               _loader;

				Impl::Impl_ResourceManager                                       _manager;

				size_t                                                           _tickIndex = 0;
				double                                                           _simulationTime = 0.0;

				ResMan::ResourceManagerPublicRuntimeParams                       _runtimeParams;
			};
		}
	}
}