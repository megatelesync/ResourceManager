#pragma once

#include "../PrivateH/ResManQuick_impl_ResourceManager.h"
#include "../PrivateH/ResManQuick_impl_access_ResourceManagerSubsystem.h"
#include "../PrivateH/Loader/ResManQuick_impl_utils_LoadingLoopCommandQueues.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			/*****************************************************************************************************
			* utils fwd declarations
			*****************************************************************************************************/
			Loader::LoadCommandParams GetLoaderCommandParams
			(
				bool                                       bIsManual,
				Impl::Impl_ManagedGroup                    *pInNullOrGroup,
				Impl::Impl_ManagedResource                 *pInResource,
				const ResMan::ResourceCommandArgs          &InCommand,
				size_t                                      InActualPriority,
				bool                                        bInLoadOnlyDependencies,
				bool                                        bInPerformRecursive
			);
			Loader::LoadCommandParams ComputeLoaderCommandParams_ForResource
			(
				bool                                       bIsManual,
				Impl::Impl_ManagedResource                 *pInResource,
				const ResMan::ResourceCommandArgs          &InCommand,
				bool bInLoadOnlyDependencies
			);
			boost::optional<Loader::LoadCommandParams> ComputeLoaderCommandParams_ForResource_OfGroup
			(
				bool                                       bIsManual,
				Impl::Impl_ManagedGroup                   *pInGroup,
				ResMan::LoadGroupCommandID                 InGroupLoadCommandID,
				Impl::Impl_ManagedResource                *pInResource,
				ResMan::Urgency                            InUrgency,
				bool                                       bInLoadOnlyDependencies
			);

			void Log_SubsystemInitialized_IfNecessary                                (Impl_ResourceManagerSubsystem *pSubsystem);
			void Log_SubsystemRuntimeParamsUpdated_IfNecessary                       (Impl_ResourceManagerSubsystem *pSubsystem, const ResMan::ResourceManagerPublicRuntimeParams *pInParams);
			void Log_SubsystemStopped_IfNecessary                                    (Impl_ResourceManagerSubsystem *pSubsystem);
			void Log_Wait_UntilAllCommandsProcessed_IfNecessary                      (Impl_ResourceManagerSubsystem *pSubsystem);
			void Log_Wait_UntilAllThreadedCommandsProcessed_IfNecessary              (Impl_ResourceManagerSubsystem *pSubsystem);
			void Log_Subsystem_ReturingFromDestructor_IfNecessary                    (Impl_ResourceManagerSubsystem *pSubsystem);

			void Log_ResourceTypeRegistered_IfNecessary                              (Impl_ResourceManagerSubsystem *pSubsystem, ResMan::IResourceType *pInRegisteredResourceType, ResMan::IResourceTypeDescr *pInTypeDescr);
			void Log_ResourceTypeContainer_ReturningFromDestructor_IfNecessary       (Impl_ResourceManagerSubsystem *pSubsystem);
			void Log_ResourceTypeContainer_Constructed_IfNecessary                   (Impl_ResourceManagerSubsystem *pSubsystem);

			void Log_SubresourceRoleRegistered_IfNecessary                           (Impl_ResourceManagerSubsystem *pSubsystem, ResMan::ISubresourceRole *pInRegisteredRole, const ResMan::SubresourceRolePublicParams *pInParams);
			void Log_SubresourceRoleContainer_Constructed_IfNecessary                (Impl_ResourceManagerSubsystem *pSubsystem);
			void Log_SubresourceRoleContainer_ReturningFromDestructor_IfNecessary    (Impl_ResourceManagerSubsystem *pSubsystem);

			void Log_SubsystemInitialized                                            (Impl_ResourceManagerSubsystem *pSubsystem);
			void Log_SubsystemRuntimeParamsUpdated                                   (Impl_ResourceManagerSubsystem *pSubsystem, const ResMan::ResourceManagerPublicRuntimeParams *pInParams);
			void Log_SubsystemStopped                                                (Impl_ResourceManagerSubsystem *pSubsystem);
			void Log_Wait_UntilAllCommandsProcessed                                  (Impl_ResourceManagerSubsystem *pSubsystem);
			void Log_Wait_UntilAllThreadedCommandsProcessed                          (Impl_ResourceManagerSubsystem *pSubsystem);
			void Log_Subsystem_ReturingFromDestructor                                (Impl_ResourceManagerSubsystem *pSubsystem);

			void Log_ResourceTypeRegistered                                          (Impl_ResourceManagerSubsystem *pSubsystem, ResMan::IResourceType *pInRegisteredResourceType, ResMan::IResourceTypeDescr *pInTypeDescr);
			void Log_ResourceTypeContainer_ReturningFromDestructor                   (Impl_ResourceManagerSubsystem *pSubsystem);
			void Log_ResourceTypeContainer_Constructed                               (Impl_ResourceManagerSubsystem *pSubsystem);

			void Log_SubresourceRoleRegistered                                       (Impl_ResourceManagerSubsystem *pSubsystem, ResMan::ISubresourceRole *pInRegisteredRole, const ResMan::SubresourceRolePublicParams *pInParams);
			void Log_SubresourceRoleContainer_Constructed                            (Impl_ResourceManagerSubsystem *pSubsystem);
			void Log_SubresourceRoleContainer_ReturningFromDestructor                (Impl_ResourceManagerSubsystem *pSubsystem);


			/****************************************************************************************************
			* Checking parameters
			****************************************************************************************************/
			void ThrowIfParamsInvalid                                                (const ResManQuick_SubsystemParams *pInSubsystemParams);
			// Throw if description of the resource type is invalid
			void ThrowIfParamsInvalid_ResourceType                                   (ResMan::IResourceTypeDescr *pInTypeDescr);
			void ThrowIfParamsIvalid_SubresourceRole                                 (const ResMan::SubresourceRolePublicParams& InParams);


			/****************************************************************************************************
			* Impl_Impl_SubresourceRoleContainer
			****************************************************************************************************/
			Impl_SubresourceRoleContainer::~Impl_SubresourceRoleContainer()
			{
				Log_SubresourceRoleContainer_ReturningFromDestructor_IfNecessary(GetOwnerSubsystem());
			}
			Impl_SubresourceRoleContainer::Impl_SubresourceRoleContainer(Impl_ResourceManagerSubsystem *pInOwnerSubsystem) :
				_pOwnerSubsystem(pInOwnerSubsystem)
			{
				assert(pInOwnerSubsystem != nullptr);
				Log_SubresourceRoleContainer_Constructed_IfNecessary(GetOwnerSubsystem());
			}

			ResMan::ISubresourceRole *Impl_SubresourceRoleContainer::RegisterSubresourceRole(const ResMan::SubresourceRolePublicParams& InParams)
			{
				assert(_subresourceRoles.size() < ResMan::MAX_SUBRESOURCE_ROLES);
				assert(&InParams != nullptr);

				ThrowIfParamsIvalid_SubresourceRole(InParams);

				int new_index = _subresourceRoles.size();
				auto pNewRole = new SubresourceRole(ResMan::SubresourceRoleIndex(new_index), InParams);
				auto it_new_role = _subresourceRoles.emplace(_subresourceRoles.end(), pNewRole);

				Log_SubresourceRoleRegistered_IfNecessary(GetOwnerSubsystem(), it_new_role->get(), &InParams);

				return it_new_role->get();
			}
			/****************************************************************************************************
			* Impl_ResourceManagerSubsystem
			****************************************************************************************************/
			Impl_ResourceManagerSubsystem::~Impl_ResourceManagerSubsystem()
			{
				Log_Subsystem_ReturingFromDestructor(this);
			}

			Impl_ResourceManagerSubsystem::Impl_ResourceManagerSubsystem(const ResManQuick_SubsystemParams& InParams, ResMan::IResourceManagerSubsystem *pInSubsystem) :
				_params(InParams),
				_loader(this),
				_resourceTypes(this),
				_subresourceRoles(this),
				_pOwnerSubsystem(pInSubsystem),
				_mainThreadLogger(CastTyped(pInSubsystem)->GetParams(), CastTyped(pInSubsystem)->GetParams()->GetLogSettings_MainThread())
				
			{
			}
			void Impl_ResourceManagerSubsystem::Tick(double InDeltaSeconds)
			{
				_simulationTime += InDeltaSeconds;
				// Code is to be here
				_tickIndex++;
			}

			void Impl_ResourceManagerSubsystem::Initialize()
			{
				_runtimeParams = _params.GetInitialRuntimeParams();

				ThrowIfParamsInvalid(&_params);

				_pManager.reset(new ResourceManager(&_manager));
				_manager.Initialize(this, _pManager.get());
				_loader.Initialize(_params);
				Log_SubsystemInitialized_IfNecessary(this);
			}

			void Impl_ResourceManagerSubsystem::UpdateRuntimeParams(const ResMan::ResourceManagerPublicRuntimeParams& InParams)
			{
				_runtimeParams = InParams;
				_loader.UpdateRuntimeParams(InParams);
				Log_SubsystemRuntimeParamsUpdated_IfNecessary(this, &InParams);
			}

			void Impl_ResourceManagerSubsystem::Stop()
			{
				Log_SubsystemStopped_IfNecessary(this);
			}

			ResMan::IResourceType *Impl_ResourceManagerSubsystem::RegisterResourceType(ResMan::IResourceTypeDescr *pInTypeDescr)
			{
				auto pResultResourceType = _resourceTypes.RegisterResourceType(pInTypeDescr);
				return pResultResourceType;
			}

			ResMan::ISubresourceRole *Impl_ResourceManagerSubsystem::RegisterRole(const ResMan::SubresourceRolePublicParams& InSubresourceRoleParams)
			{
				auto pRegisteredSubresourceRole =_subresourceRoles.RegisterSubresourceRole(InSubresourceRoleParams);
				return pRegisteredSubresourceRole;
			}

			void Impl_ResourceManagerSubsystem::PrecacheGroupResource(Impl::Impl_ManagedGroup *pInGroup, ResMan::LoadGroupCommandID InGroupCommandID, Impl::Impl_ManagedResource *pInResource, ResMan::Urgency InUrgency, bool bIsManual)
			{
				if (_runtimeParams.IsFreezed())
				{
					return;
				}

				const bool bLoadOnlyDependencies = false;
				auto params = ComputeLoaderCommandParams_ForResource_OfGroup
				(
					bIsManual,
					pInGroup,
					InGroupCommandID,
					pInResource,
					InUrgency,
					bLoadOnlyDependencies
				);
				if (false == params.is_initialized())
				{
					return;
				}
				_loader.EnqueueLoadCommand(params.get());
			}
			void Impl_ResourceManagerSubsystem::PrecacheGroupResourceDependencies(Impl::Impl_ManagedGroup *pInGroup, ResMan::LoadGroupCommandID InGroupCommandID, Impl::Impl_ManagedResource *pInResource, ResMan::Urgency InUrgency, bool bIsManual)
			{
				if (_runtimeParams.IsFreezed())
				{
					return;
				}

				const bool bLoadOnlyDependencies = true;
				auto params = ComputeLoaderCommandParams_ForResource_OfGroup
				(
					bIsManual,
					pInGroup,
					InGroupCommandID,
					pInResource,
					InUrgency,
					bLoadOnlyDependencies
				);
				if (false == params.is_initialized())
				{
					return;
				}
				_loader.EnqueueLoadCommand(params.get());
			}
			void Impl_ResourceManagerSubsystem::PrecacheGroupResourceSynchro(Impl::Impl_ManagedGroup *pInGroup, ResMan::LoadGroupCommandID InGroupCommandID, Impl::Impl_ManagedResource *pInResource, bool bIsManual)
			{
				if (_runtimeParams.IsFreezed())
				{
					return;
				}

				const bool bLoadOnlyDependencies = false;
				const ResMan::Urgency urgency = ResMan::Urgency_Normal; // Urgency is irrelevant for the synchro command
				auto params = ComputeLoaderCommandParams_ForResource_OfGroup
				(
					bIsManual,
					pInGroup,
					InGroupCommandID,
					pInResource,
					urgency,
					bLoadOnlyDependencies
				);
				if (false == params.is_initialized())
				{
					return;
				}
				_loader.LoadSynchro(params.get());
			}

			void Impl_ResourceManagerSubsystem::PrecacheResource(Impl::Impl_ManagedResource *pInResource, const ResMan::LoadCommand& InCommand, bool bIsManual)
			{	
				if (_runtimeParams.IsFreezed())
				{
					return;
				}

				const bool bLoadOnlyDependencies = false;
				if (InCommand.args.subresources.IsEmpty())
				{
					return;
				}
				Loader::LoadCommandParams params = ComputeLoaderCommandParams_ForResource(bIsManual, pInResource, InCommand.args, bLoadOnlyDependencies);
				_loader.EnqueueLoadCommand(params);
			}

			void Impl_ResourceManagerSubsystem::PrecacheResourceSynchro(Impl::Impl_ManagedResource *pInResource, const ResMan::LoadCommand& InCommand, bool bIsManual)
			{
				if (_runtimeParams.IsFreezed())
				{
					return;
				}

				const bool bLoadOnlyDependencies = false;
				if (InCommand.args.subresources.IsEmpty())
				{
					return;
				}
				Loader::LoadCommandParams params = ComputeLoaderCommandParams_ForResource(bIsManual, pInResource, InCommand.args, bLoadOnlyDependencies);
				_loader.LoadSynchro(params);
			}

			void Impl_ResourceManagerSubsystem::PrecacheResourceDependencyLists(Impl::Impl_ManagedResource *pInResource, const ResMan::PrecacheDependenciesCommand& InCommand, bool bIsManual)
			{
				if (_runtimeParams.IsFreezed())
				{
					return;
				}

				const bool bLoadOnlyDependencies = true;
				if (InCommand.args.subresources.IsEmpty())
				{
					return;
				}
				Loader::LoadCommandParams params = ComputeLoaderCommandParams_ForResource(bIsManual, pInResource, InCommand.args, bLoadOnlyDependencies);
				_loader.EnqueueLoadCommand(params);
			}

			void Impl_ResourceManagerSubsystem::PrecacheResourceDependencyListsSynchro(Impl::Impl_ManagedResource *pInResource, const ResMan::PrecacheDependenciesCommand& InCommand, bool bIsManual)
			{
				if (_runtimeParams.IsFreezed())
				{
					return;
				}

				const bool bLoadOnlyDependencies = true;
				if (InCommand.args.subresources.IsEmpty())
				{
					return;
				}
				Loader::LoadCommandParams params = ComputeLoaderCommandParams_ForResource(bIsManual, pInResource, InCommand.args, bLoadOnlyDependencies);
				_loader.LoadSynchro(params);
			}

			void Impl_ResourceManagerSubsystem::Wait_UntilAllCommandsProcessed()
			{
				_loader.Wait_UntilAllCommandsProcessed();
				Log_Wait_UntilAllCommandsProcessed_IfNecessary(this);
			}
			void Impl_ResourceManagerSubsystem::Wait_UntilAllResourceCommandsProcessed(ResMan::IManagedResource *pInResource, bool bInRecurisve)
			{
				_loader.Wait_UntilAllResourceCommandsProcessed(pInResource, bInRecurisve);
				Util::TODO("Logging");
			}
			void Impl_ResourceManagerSubsystem::Wait_UntilAllGroupCommandsProcessed(ResMan::IManagedGroup *pInGroup, bool bInRecurisve)
			{
				_loader.Wait_UntilAllGroupCommandsProcessed(pInGroup, bInRecurisve);
				Util::TODO("Logging");
			}

			void Impl_ResourceManagerSubsystem::Wait_UntilAllThreadedCommandsProcessed()
			{
				_loader.Wait_UntilAllThreadedCommandsProcessed();
				Log_Wait_UntilAllThreadedCommandsProcessed_IfNecessary(this);
			}
			void Impl_ResourceManagerSubsystem::Wait_UntilAllThreadedCommandsForResourceProcessed(ResMan::IManagedResource *pInResource, bool bInRecursive)
			{
				_loader.Wait_UntilAllThreadedCommandsForResourceProcessed(pInResource, bInRecursive);
				Util::TODO("Logging");
			}

			/*************************************************************************************************************
			* Impl_ResourceTypeContainer implementation
			*************************************************************************************************************/
			Impl_ResourceTypeContainer::Impl_ResourceTypeContainer(Impl_ResourceManagerSubsystem *pInOwnerSubsystem) :
				_pOwnerSubsystem(pInOwnerSubsystem)
			{
				assert(pInOwnerSubsystem != nullptr);
				Log_ResourceTypeContainer_Constructed_IfNecessary(GetOwnerSubsystem());
			}

			Impl_ResourceTypeContainer::~Impl_ResourceTypeContainer()
			{
				Log_ResourceTypeContainer_ReturningFromDestructor_IfNecessary(GetOwnerSubsystem());
			}

			ResMan::IResourceType *Impl_ResourceTypeContainer::RegisterResourceType(ResMan::IResourceTypeDescr *pInTypeDescr)
			{
				ThrowIfParamsInvalid_ResourceType(pInTypeDescr);

				// COMMENT: The subresource roles of the given descr are to be registered in the ResourceManagerSubsystem class.
				auto nextIndex = _resourceTypes.size();
				auto pResourceType = new ResourceType(pInTypeDescr, pInTypeDescr->GetParams(), nextIndex);
				_resourceTypes.emplace_back(pResourceType);

				Log_ResourceTypeRegistered_IfNecessary(GetOwnerSubsystem(), pResourceType, pInTypeDescr);

				return pResourceType;
			}

			/*************************************************************************************************************
			* utils implementation
			*************************************************************************************************************/
			Loader::LoadCommandParams GetLoaderCommandParams
			(
				bool                                        bIsManual,
				Impl::Impl_ManagedGroup                    *pInNullOrGroup,
				Impl::Impl_ManagedResource                 *pInResource,
				const ResMan::ResourceCommandArgs          &InCommand,
				size_t                                      InActualPriority,
				bool                                        bInLoadOnlyDependencies,
				bool                                        bInPerformRecursive
			)
			{
				Loader::LoadCommandParams params;
				params.request.SetIsManual(bIsManual);

				params.bPerformRecursive = bInPerformRecursive;
				params.priority = InActualPriority;
				params.request.SetRolesToLoad(InCommand.subresources);
				params.request.SetUrgency(InCommand.urgency);
				params.request.SetResource(pInResource);
				params.request.SetEnableState_LoadOnlyDependenciyLists(bInLoadOnlyDependencies);
				params.request.SetGroup(pInNullOrGroup);
				return params;
			}

			// Returns correct loader command params for resource that do belong to group.
			boost::optional<Loader::LoadCommandParams> ComputeLoaderCommandParams_ForResource_OfGroup
			(
				bool                                       bIsManual,
				Impl::Impl_ManagedGroup                   *pInGroup,
				ResMan::LoadGroupCommandID                 InGroupLoadCommandID,
				Impl::Impl_ManagedResource                *pInResource,
				ResMan::Urgency                            InUrgency,
				bool                                       bInLoadOnlyDependencies
			)
			{
				const bool bPerformRecursive = true; // TO DO: must be getter from the group params
				size_t groupPriority = 0; // TO DO: must be getted from the group priority

				auto subresourceRolesToLoad                                 = pInResource->GetCombinableParams().GetSubresourceRolesToLoad();
				auto subresourceRolesToKeepDependencyListsLoaded            = pInResource->GetCombinableParams().GetSubresourceRolesToKeepDependencies();

				auto rolesToLoad = bInLoadOnlyDependencies ? subresourceRolesToKeepDependencyListsLoaded : subresourceRolesToLoad;

				if (rolesToLoad.IsEmpty())
				{
					return boost::optional<Loader::LoadCommandParams>();
				}

				Loader::LoadCommandParams params;

				params.request.BindGroupCommand(InGroupLoadCommandID, pInGroup);
				params.request.SetIsManual(bIsManual);

				params.bPerformRecursive = bPerformRecursive;
				params.priority = groupPriority;
				params.request.SetRolesToLoad(rolesToLoad);
				params.request.SetUrgency(InUrgency);
				params.request.SetResource(pInResource);
				params.request.SetEnableState_LoadOnlyDependenciyLists(bInLoadOnlyDependencies);
				return params;
			}

			// Returns correct loader command params for resource that does not belong to group.
			// Resource are to be calculated base on settings of the subsystem and parameters of the resource.
			Loader::LoadCommandParams ComputeLoaderCommandParams_ForResource
			(
				bool                                        bIsManual,
				Impl::Impl_ManagedResource                 *pInResource,
				const ResMan::ResourceCommandArgs          &InCommand,
				bool bInLoadOnlyDependencies
			)
			{
				Impl::Impl_ManagedGroup * const pGroup = nullptr;
				const bool bPerformRecursive = true;
				const size_t actualPriority = Loader::ComputeActualCommandPriority(InCommand.urgency, pInResource->GetCombinableParams().GetBaseLoadPriority());
				auto params = GetLoaderCommandParams(bIsManual, pGroup, pInResource, InCommand, actualPriority, bInLoadOnlyDependencies, bPerformRecursive);
				return params;
			}

			/************************************************************************************************************
			* Checking params implementation
			************************************************************************************************************/
			void ThrowIfParamsInvalid(const ResManQuick_SubsystemParams *pInSubsystemParams)
			{
				// subsystem is tested only in single-threaded mode yet
				assert(pInSubsystemParams->IsSingleThreaded());
			}

			void ThrowIfParamsInvalid_ResourceType(ResMan::IResourceTypeDescr *pInTypeDescr)
			{
				assert(pInTypeDescr != nullptr);

				// Check that the given default URL exists
				//pInTypeDescr->GetParams().GetDefaultURL();

				// Check that the the subdir URL exists
			}

			void ThrowIfParamsIvalid_SubresourceRole(const ResMan::SubresourceRolePublicParams& InParams)
			{
				// not yet impl
			}

			/************************************************************************************************************
			* Logging implementation
			************************************************************************************************************/
			void Log_SubsystemInitialized_IfNecessary(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_SubsystemInitialized(pSubsystem);
				}
			}
			void Log_SubsystemRuntimeParamsUpdated_IfNecessary(Impl_ResourceManagerSubsystem *pSubsystem, const ResMan::ResourceManagerPublicRuntimeParams *pInParams)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_SubsystemRuntimeParamsUpdated(pSubsystem, pInParams);
				}
			}
			void Log_SubsystemStopped_IfNecessary(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_SubsystemStopped(pSubsystem);
				}
			}
			void Log_Wait_UntilAllCommandsProcessed_IfNecessary(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_Wait_UntilAllCommandsProcessed(pSubsystem);
				}
			}
			void Log_Wait_UntilAllThreadedCommandsProcessed_IfNecessary(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_Wait_UntilAllThreadedCommandsProcessed(pSubsystem);
				}
			}
			void Log_Subsystem_ReturingFromDestructor_IfNecessary(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_Subsystem_ReturingFromDestructor(pSubsystem);
				}
			}

			void Log_ResourceTypeRegistered_IfNecessary(Impl_ResourceManagerSubsystem *pSubsystem, ResMan::IResourceType *pInRegisteredResourceType, ResMan::IResourceTypeDescr *pInTypeDescr)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_ResourceTypeRegistered(pSubsystem, pInRegisteredResourceType, pInTypeDescr);
				}
			}
			void Log_ResourceTypeContainer_ReturningFromDestructor_IfNecessary(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_ResourceTypeContainer_ReturningFromDestructor(pSubsystem);
				}
			}
			void Log_ResourceTypeContainer_Constructed_IfNecessary(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_ResourceTypeContainer_Constructed(pSubsystem);
				}
			}
			void Log_SubresourceRoleRegistered_IfNecessary(Impl_ResourceManagerSubsystem *pSubsystem, ResMan::ISubresourceRole *pInRegisteredRole, const ResMan::SubresourceRolePublicParams *pInParams)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_SubresourceRoleRegistered(pSubsystem, pInRegisteredRole, pInParams);
				}
			}
			void Log_SubresourceRoleContainer_Constructed_IfNecessary(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_SubresourceRoleContainer_Constructed(pSubsystem);
				}
			}
			void Log_SubresourceRoleContainer_ReturningFromDestructor_IfNecessary(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_SubresourceRoleContainer_ReturningFromDestructor(pSubsystem);
				}
			}

			void Log_SubsystemInitialized(Impl_ResourceManagerSubsystem *pSubsystem)
			{			
				auto pLog = pSubsystem->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "Subsystem initialized", Util::EMessageType::Status);
				Log_SubsystemParams(pLog, &(pSubsystem->GetParams()));
			}
			void Log_SubsystemRuntimeParamsUpdated(Impl_ResourceManagerSubsystem *pSubsystem, const ResMan::ResourceManagerPublicRuntimeParams *pInParams)
			{
				auto pLog = pSubsystem->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "Subsystem runtime params updated: ", Util::EMessageType::Status);
				Log_SubsystemRuntimeParams(pLog, pInParams);
			}
			void Log_SubsystemStopped(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				auto pLog = pSubsystem->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "Subsystem stopped", Util::EMessageType::Status);
			}
			void Log_Wait_UntilAllCommandsProcessed(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				auto pLog = pSubsystem->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "Wait_UntilAllCommandsProcessed completed", Util::EMessageType::Status);
			}
			void Log_Wait_UntilAllThreadedCommandsProcessed(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				auto pLog = pSubsystem->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "Log_Wait_UntilAllThreadedCommandsProcessed completed", Util::EMessageType::Status);
			}
			void Log_Subsystem_ReturingFromDestructor(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				auto pLog = pSubsystem->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "Subsystem is returning from destructor", Util::EMessageType::Status);
			}
			void Log_ResourceTypeRegistered(Impl_ResourceManagerSubsystem *pSubsystem, ResMan::IResourceType *pInRegisteredResourceType, ResMan::IResourceTypeDescr *pInTypeDescr)
			{
				auto pLog = pSubsystem->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "Resource type registered", Util::EMessageType::Status);
				*pLog << "Resource type:" << Util::endl;
				ResMan::Log_RegisteredResourceType(pLog, pInRegisteredResourceType, pSubsystem->GetManager()->GetInterface());
			}

			void Log_ResourceTypeContainer_ReturningFromDestructor(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				auto pLog = pSubsystem->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "Resource type container: returing from destructor", Util::EMessageType::Status);
			}
			void Log_ResourceTypeContainer_Constructed(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				auto pLog = pSubsystem->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "Resource type: constructed", Util::EMessageType::Status);
			}
			void Log_SubresourceRoleRegistered(Impl_ResourceManagerSubsystem *pSubsystem, ResMan::ISubresourceRole *pInRegisteredRole, const ResMan::SubresourceRolePublicParams *pInParams)
			{
				auto pLog = pSubsystem->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "subresource role: registered", Util::EMessageType::Status);
				Log_SubresourceRole(pLog, pInRegisteredRole);
			}
			void Log_SubresourceRoleContainer_Constructed(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				auto pLog = pSubsystem->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "subresource role container: constructed", Util::EMessageType::Status);
			}
			void Log_SubresourceRoleContainer_ReturningFromDestructor(Impl_ResourceManagerSubsystem *pSubsystem)
			{
				auto pLog = pSubsystem->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "subresource role container: returning from destructor", Util::EMessageType::Status);
			}
		}
	}
}