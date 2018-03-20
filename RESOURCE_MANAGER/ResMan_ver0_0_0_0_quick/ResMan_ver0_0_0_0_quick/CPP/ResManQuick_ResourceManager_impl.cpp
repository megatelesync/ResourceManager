#pragma once

#include "../PrivateH/ResManQuick_impl_ResourceManager.h"
#include "../PrivateH/ResManQuick_impl_ResourceManagerSubsystem.h"
#include "../PrivateH/ResManQuick_impl_access_SubresourceRole.h"
#include "../PrivateH/ResManQuick_impl_access_ManagedResource.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			/********************************************************************************************************************
			* Typedefs implementation
			********************************************************************************************************************/
			using AllGroupEnumerator                         = ContGroupEnumerator<const GroupCont_SequentialCont>;
			
			using AllResourceEnumerator                      = ContResourceEnumerator<const ResourceContainer_Cont>;
			using AllResourceWithURLEnumerator               = MapValueResourceEnumerator<const ResourceContainer::UrlToResourceMap>;

			/********************************************************************************************************************
			* Logging utils
			********************************************************************************************************************/
			void Log_ResourceManager_ReturingFromDestructor_IfNecesssary(Impl_ResourceManager *pManager);
			void Log_ResourceManager_Initialized_IfNecessary(Impl_ResourceManager *pManager);

			void Log_ResourceManager_FailedResourcePushed_IfNecessary(Impl_ResourceManager *pManager, ManagedResource *pPushedResource);
			void Log_ResourceManager_FaledResourcePopped_IfNecessary(Impl_ResourceManager *pManager, ManagedResource *pPoppedResource);

			void Log_ResourceManager_GroupRegistered_IfNecessary(Impl_ResourceManager *pManager, ManagedGroup *pRegisteredGroup, const ResMan::GroupParams *pInGroupParams);
			void Log_ResourceManager_BeforeUnregisterGroup_IfNecessary(Impl_ResourceManager *pManager, ManagedGroup *pGroupToUnregister);

			void Log_ResourceManager_ReturingFromDestructor(Impl_ResourceManager *pManager);
			void Log_ResourceManager_Initialized(Impl_ResourceManager *pManager);
			void Log_ResourceManager_FailedResourcePushed(Impl_ResourceManager *pManager, ManagedResource *pPushedResource);
			void Log_ResourceManager_FaledResourcePopped(Impl_ResourceManager *pManager, ManagedResource *pPoppedResource);
			void Log_ResourceManager_GroupRegistered(Impl_ResourceManager *pManager, ManagedGroup *pRegisteredGroup, const ResMan::GroupParams *pInGroupParams);
			void Log_ResourceManager_BeforeUnregisterGroup(Impl_ResourceManager *pManager, ManagedGroup *pGroupToUnregister);


			/********************************************************************************************************************
			* Impl_ResourceManager implementation
			********************************************************************************************************************/
			Impl_ResourceManager::Impl_ResourceManager()
			{
			}

			Impl_ResourceManager::~Impl_ResourceManager()
			{
				Log_ResourceManager_ReturingFromDestructor_IfNecesssary(this);
			}

			void Impl_ResourceManager::Initialize(Impl_ResourceManagerSubsystem *pInOwnerSubsystem, ResMan::IResourceManager *pInOwnerManager)
			{
				_pSubsystem = pInOwnerSubsystem;
				_pOwnerManager = pInOwnerManager;
				_resourceRepo.Initialize(this);
				Log_ResourceManager_Initialized_IfNecessary(this);
			}
			void Impl_ResourceManager::Tick()
			{
			}

			ResMan::IResourceManager::ResourceEnumerator Impl_ResourceManager::GetResourceEnumerator()
			{				
				return MakeResourceEnumeratorResult(new AllResourceEnumerator(_resourceRepo.GetResources()));
			}
			ResMan::IResourceManager::ResourceEnumerator Impl_ResourceManager::GetResourceWithURL()
			{
				auto pUrlToResourceMap = _resourceRepo.GetUrlToResourceMap();
				return MakeResourceEnumeratorResult(new AllResourceWithURLEnumerator(pUrlToResourceMap));
			}

			void AppendResources_FromEnumerator(std::list<Impl_ManagedResource *> *pOutResourcesToPrecache, Util::IEnumerator<ResMan::IManagedResource> *pEnumerator)
			{
				while (true)
				{
					if (pEnumerator->GetCurrent() == nullptr)
					{
						break;
					}
					auto pResource = CastTyped(pEnumerator->GetCurrent())->GetImpl();
					pOutResourcesToPrecache->push_back(pResource);
					pEnumerator->MoveNext();
				}
			}
			void AppendNotPrecachedResources(Impl_ManagedGroup *pInGroup, std::list<Impl_ManagedResource *> *pOutResourcesToPrecache, bool bIsManual)
			{
				assert(pInGroup->IsOperable());

				auto pManager = pInGroup->GetOwnerManager();
				auto enumerator = pInGroup->GetSubresourceRoles_NotPrecachedResources().GetEnumerator();
				while (true)
				{
					if (true == enumerator.IsEnd())
					{
						break;
					}
					auto pRole = pManager->GetSubresourceRoleByIndex(enumerator.GetRoleIndex());
					auto pEnumerator = std::move(pInGroup->GetNotPrecachedResources(pRole));
					AppendResources_FromEnumerator(pOutResourcesToPrecache, pEnumerator.get());
					enumerator.MoveNext();
				}
			}
			void AppendPrecachedResources(Impl_ManagedGroup *pInGroup, std::list<Impl_ManagedResource *> *pOutResourcesToPrecache, bool bIsManual)
			{
				assert(pInGroup->IsOperable());

				auto pManager = pInGroup->GetOwnerManager();
				auto enumerator = pInGroup->GetSubresourceRoles_NotPrecachedResources().GetEnumerator();
				while (true)
				{
					if (true == enumerator.IsEnd())
					{
						break;
					}
					auto pRole = pManager->GetSubresourceRoleByIndex(enumerator.GetRoleIndex());
					auto pEnumerator = std::move(pInGroup->GetPrecachedResources(pRole));
					AppendResources_FromEnumerator(pOutResourcesToPrecache, pEnumerator.get());
					enumerator.MoveNext();
				}
			}
			void AppendPrecachedFailedResources(Impl_ManagedGroup *pInGroup, std::list<Impl_ManagedResource *> *pOutResourcesToPrecache, bool bIsManual)
			{
				assert(pInGroup->IsOperable());

				auto pManager = pInGroup->GetOwnerManager();
				auto enumerator = pInGroup->GetSubresourceRoles_NotPrecachedResources().GetEnumerator();
				while (true)
				{
					if (true == enumerator.IsEnd())
					{
						break;
					}
					auto pRole = pManager->GetSubresourceRoleByIndex(enumerator.GetRoleIndex());
					auto pEnumerator = std::move(pInGroup->GetFailedPrecachedResources(pRole));
					AppendResources_FromEnumerator(pOutResourcesToPrecache, pEnumerator.get());
					enumerator.MoveNext();
				}
			}

			void ListResourcesToPrecache(Impl_ManagedGroup *pInGroup, std::list<Impl_ManagedResource *> *pOutResourcesToPrecache, bool bIsManual)
			{
				assert(pInGroup->IsOperable());

				const bool bForceReload = false;

				pOutResourcesToPrecache->clear();
				AppendNotPrecachedResources(pInGroup, pOutResourcesToPrecache, bIsManual);

				if(bForceReload)
				{
					AppendPrecachedFailedResources(pInGroup, pOutResourcesToPrecache, bIsManual);
					AppendPrecachedResources(pInGroup, pOutResourcesToPrecache, bIsManual);
				}
			}

			void Impl_ResourceManager::PrecacheGroup(Impl_ManagedGroup *pInGroup, bool bIsManual, ResMan::Urgency InUrgency)
			{
				assert(pInGroup->IsOperable());

				const bool bOnlyDependencyLists = false;
				const bool bSynchro = false;

				_PrecacheGroupResources(pInGroup, bIsManual, InUrgency, bOnlyDependencyLists, bSynchro);
			}

			void Impl_ResourceManager::PrecacheGroupDependencyLists(Impl_ManagedGroup *pInGroup, bool bIsManual, ResMan::Urgency InUrgency)
			{
				assert(pInGroup->IsOperable());

				const bool bOnlyDependencyLists = true;
				const bool bSynchro = false;

				_PrecacheGroupResources(pInGroup, bIsManual, InUrgency, bOnlyDependencyLists, bSynchro);
			}
			void Impl_ResourceManager::PrecacheGroupSynchro(Impl_ManagedGroup *pInGroup, bool bIsManual)
			{
				assert(pInGroup->IsOperable());

				const bool bOnlyDependencyLists = true;
				const bool bSynchro = true;

				_PrecacheGroupResources(pInGroup, bIsManual, ResMan::Urgency_Normal, bOnlyDependencyLists, bSynchro);
			}

			void Impl_ResourceManager::_PrecacheGroupResources(Impl_ManagedGroup *pInGroup, bool bIsManual, ResMan::Urgency InUrgency, bool bOnlyDependencyLists, bool bSynchro)
			{
				assert(pInGroup->IsOperable());

				std::list<Impl_ManagedResource *>                             resourcesToPrecache;
				ListResourcesToPrecache(pInGroup, &resourcesToPrecache, bIsManual);

				// We only precache if the group is not loading at the moment
				if (false == pInGroup->GetCommandState().bDoingGroupCommand)
				{
					auto commandId = _PushGroupLoadRequest(pInGroup->GetManagedGroup());
					if(false == resourcesToPrecache.empty())
					{
						pInGroup->Notify_LoadBegun(&resourcesToPrecache);
						for (auto pResource : resourcesToPrecache)
						{
							if (bOnlyDependencyLists)
							{
								GetSubsystem()->PrecacheGroupResourceDependencies(pInGroup, commandId, pResource, InUrgency, bIsManual);
							}
							else
							{
								if (bSynchro)
								{
									GetSubsystem()->PrecacheGroupResourceSynchro(pInGroup, commandId, pResource, bIsManual);
								}
								else
								{
									GetSubsystem()->PrecacheGroupResource(pInGroup, commandId, pResource, InUrgency, bIsManual);
								}
							}
						}
					}
				}
			}

			ResMan::LoadGroupCommandID Impl_ResourceManager::_PushGroupLoadRequest(ManagedGroup *pInGroup)
			{
				assert(pInGroup != nullptr);
				auto commandId = ResMan::LoadGroupCommandID(_groupCommandIdGenerator.GenerateID());
				_loadingGroups.push(pInGroup);
				return commandId;
			}

			ManagedResource *Impl_ResourceManager::FindResourceByURL(const std::string& InURL)
			{
				return _resourceRepo.FindByURL(InURL);
			}

			ManagedResource *Impl_ResourceManager::RegisterResource(const ResMan::PublicResourceParams& InParams, std::shared_ptr<ResMan::IResourceLoader> pInLoader)
			{
				auto pResourceType = pInLoader->GetMainResourceParams().GetResourceType();
				auto returned = _resourceRepo.RegisterResource(InParams, pInLoader, pResourceType, ResMan::AddressableSubresourceIndex_GetNull());
				return returned;
			}

			void Impl_ResourceManager::UnregisterResource(ManagedResource *pInResource)
			{
				pInResource->GetImpl()->DetachResource_FromAllGroups();
				_resourceRepo.UnregisterResource(pInResource->GetImpl());
			}

			ManagedResource *Impl_ResourceManager::PopFailedResource()
			{
				if (_failedResources.empty())
				{
					return nullptr;
				}
				else
				{
					auto pPopped = _failedResources.front().get();
					pPopped->GetImpl()->Notify_FailHasBeenProcessed();
					_failedResources.pop();
					Log_ResourceManager_FaledResourcePopped_IfNecessary(this, pPopped);
					return pPopped;
				}
			}

			void Log_FatalError_Encountered(Util::ILoggingDevice *pInLog, ManagedResource *pInResource)
			{
				Util::LogRecordScoper scoper(pInLog, "Fatal error", Util::EMessageType::FatalError);

				*pInLog << "Resource:" << Util::endl;
				Log_Resource_LeanAndMean(pInLog, pInResource->GetImpl(), Util::EMessageType::FatalError);
			}

			bool Impl_ResourceManager::TryPushFailedResource(std::shared_ptr<ManagedResource> pInResource)
			{
				if (pInResource->GetImpl()->GetNumFailsToBePopped() == 0)
				{
					_failedResources.push(pInResource);
					Log_ResourceManager_FailedResourcePushed_IfNecessary(this, pInResource.get());

					auto pResourceCombinableParams = &(pInResource->GetCombinableParams());

					if (_ShouldProcessErrorAsFatal(pResourceCombinableParams))
					{
						Log_FatalError_Encountered(GetSubsystem()->GetLogger_MainThread(), pInResource.get());
						exit(-1);
					}
					return true;
				}
				else
				{
					return false;
				}

			}

			bool Impl_ResourceManager::_ShouldProcessErrorAsFatal(const ResMan::CombinableParams *pInCombinableParams) const
			{
				auto pErrorHandlingParams = GetSubsystem()->GetParams().GetCommonSubsystemParams().GetErrorHandling();
				auto bShouldAlwaysTreatErrorsAsFatal = pErrorHandlingParams->ShouldAlwaysTreatErrorsAsFatal();
				if (bShouldAlwaysTreatErrorsAsFatal)
				{
					return true;
				}
				bool bShouldNeverTreatErrorsAsFatal = ResMan::ShouldActuallyNeverTreatErrorsAsFatal(pErrorHandlingParams);
				if (bShouldNeverTreatErrorsAsFatal)
				{
					return false;
				}
				auto bIsFatal_CombinableParams = pInCombinableParams->GetErrorHandlingParams()->IsFatal();
				return bIsFatal_CombinableParams;
			}

			ManagedGroup *Impl_ResourceManager::RegisterGroup(const ResMan::GroupParams& InGroupParams)
			{
				GroupContElement *pGroupContElement = _groups.GetFreeGroupElement();
				auto pNewGroup = new ManagedGroup(this, &InGroupParams, pGroupContElement);
				_groups.RegisterGroup(pGroupContElement, pNewGroup);

				Log_ResourceManager_GroupRegistered_IfNecessary(this, pNewGroup, &InGroupParams);

				return pNewGroup;
			}

			void Impl_ResourceManager::UnregisterGroup(ManagedGroup *pInGroup)
			{
				assert(pInGroup->GetImpl()->IsOperable());
				Log_ResourceManager_BeforeUnregisterGroup_IfNecessary(this, pInGroup);
				pInGroup->GetImpl()->Notify_BeforeUnregisterGroup();
				_groups.DeleteGroup(pInGroup);
			}

			ManagedGroup *Impl_ResourceManager::PopFailedGroup()
			{
				if (_failedGroups.empty())
				{
					return nullptr;
				}
				else
				{
					auto pPopped = _failedGroups.front();
					_failedGroups.pop();
					return pPopped.get();
				}
			}

			void Impl_ResourceManager::Notify_GroupResource_TriedToLoad(Impl_ManagedGroup *pInGroup, Impl_ManagedResource *pInResource)
			{
				const auto& commandState                         = pInGroup->GetCommandState();
				auto countToBeTriedToLoad                        = commandState.counters.GetNumResourcesToBeTriedToLoad();
				if (0 == countToBeTriedToLoad)
				{
					pInGroup->Notify_LoadEnded();
					if (commandState.last_result.status_code != ResMan::ECommandStatusCode::OK)
					{
						
						auto pGroupPtr = pInGroup->GetManagedGroup()->GetImpl()->GetSlot()->GetGroupPtr();
						_failedGroups.push(pGroupPtr);
					}
				}
			}

			ResMan::IResourceManager::GroupEnumerator Impl_ResourceManager::GetEnumerator_Groups()
			{
				return MakeGroupEnumeratorResult(new AllGroupEnumerator(&(_groups.GetAllGroups())));
			}

			void Impl_ResourceManager::Wait_UntilAllCommandsProcessed()
			{
				GetSubsystem()->Wait_UntilAllCommandsProcessed();
			}

			void Impl_ResourceManager::Wait_UntilAllResourceCommandsProcessed(ResMan::IManagedResource *pInResource, bool bInRecurisve)
			{
				GetSubsystem()->Wait_UntilAllResourceCommandsProcessed(pInResource, bInRecurisve);
			}
			void Impl_ResourceManager::Wait_UntilAllGroupCommandsProcessed(ResMan::IManagedGroup *pInGroup, bool bInRecurisve)
			{
				GetSubsystem()->Wait_UntilAllGroupCommandsProcessed(pInGroup, bInRecurisve);
			}

			void Impl_ResourceManager::Wait_UntilAllThreadedCommandsProcessed()
			{
				GetSubsystem()->Wait_UntilAllThreadedCommandsProcessed();
			}

			void Impl_ResourceManager::Wait_UntilAllThreadedCommandsForResourceProcessed(ResMan::IManagedResource *pInResource, bool bInRecursive)
			{
				GetSubsystem()->Wait_UntilAllThreadedCommandsForResourceProcessed(pInResource, bInRecursive);
			}

			ResMan::ISubresourceRole *Impl_ResourceManager::GetSubresourceRoleByIndex(ResMan::SubresourceRoleIndex InIndex)
			{
				const auto& subresourceRoles = GetSubsystem()->GetSubresourceRoles();

				assert(InIndex >= 0);
				assert(InIndex < subresourceRoles.size());

				auto pResult = subresourceRoles[InIndex].get();
				return pResult;
			}

			/***************************************************************************************************************************
			* Logging utils implementation
			***************************************************************************************************************************/
			void Log_ResourceManager_ReturingFromDestructor_IfNecesssary(Impl_ResourceManager *pManager)
			{
				bool bShouldLog = true;
				if(bShouldLog)
				{
					Log_ResourceManager_ReturingFromDestructor(pManager);
				}
			}
			void Log_ResourceManager_Initialized_IfNecessary(Impl_ResourceManager *pManager)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_ResourceManager_Initialized(pManager);
				}
			}
			void Log_ResourceManager_FailedResourcePushed_IfNecessary(Impl_ResourceManager *pManager, ManagedResource *pPushedResource)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_ResourceManager_FailedResourcePushed(pManager, pPushedResource);
				}
			}
			void Log_ResourceManager_FaledResourcePopped_IfNecessary(Impl_ResourceManager *pManager, ManagedResource *pPoppedResource)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_ResourceManager_FaledResourcePopped(pManager, pPoppedResource);
				}
			}
			void Log_ResourceManager_GroupRegistered_IfNecessary(Impl_ResourceManager *pManager, ManagedGroup *pRegisteredGroup, const ResMan::GroupParams *pInGroupParams)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_ResourceManager_GroupRegistered(pManager, pRegisteredGroup, pInGroupParams);
				}
			}
			void Log_ResourceManager_BeforeUnregisterGroup_IfNecessary(Impl_ResourceManager *pManager, ManagedGroup *pGroupToUnregister)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_ResourceManager_BeforeUnregisterGroup(pManager, pGroupToUnregister);
				}
			}

			void Log_ResourceManager_ReturingFromDestructor(Impl_ResourceManager *pManager)
			{
				auto pLog = pManager->GetSubsystem()->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "ResourceManager: returning from destructor", Util::EMessageType::Status);
			}
			void Log_ResourceManager_Initialized(Impl_ResourceManager *pManager)
			{
				auto pLog = pManager->GetSubsystem()->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "ResourceManager: Initialized", Util::EMessageType::Status);
			}
			void Log_ResourceManager_FailedResourcePushed(Impl_ResourceManager *pManager, ManagedResource *pPushedResource)
			{
				auto pLog = pManager->GetSubsystem()->GetLogger_MainThread();

				Util::LogRecordScoper logRecordScoper(pLog, "ResourceManager: failed resource pushed", Util::EMessageType::Error);
				*pLog << "Resource:" << Util::endl;

				Log_Resource(pLog, pPushedResource->GetImpl(), Util::EMessageType::Error);
			}
			void Log_ResourceManager_FaledResourcePopped(Impl_ResourceManager *pManager, ManagedResource *pPoppedResource)
			{
				auto pLog = pManager->GetSubsystem()->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "ResourceManager: failed resource popped", Util::EMessageType::Status);
				*pLog << "Resource:" << Util::endl;
				Log_Resource(pLog, pPoppedResource->GetImpl(), Util::EMessageType::Status);
			}
			void Log_ResourceManager_GroupRegistered(Impl_ResourceManager *pManager, ManagedGroup *pRegisteredGroup, const ResMan::GroupParams *pInGroupParams)
			{
				auto pLog = pManager->GetSubsystem()->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "ResourceManager: group registered", Util::EMessageType::Status);
				*pLog << "Group:" << Util::endl;
				Log_Group(pLog, pRegisteredGroup->GetImpl(), Util::EMessageType::Status);
			}
			void Log_ResourceManager_BeforeUnregisterGroup(Impl_ResourceManager *pManager, ManagedGroup *pGroupToUnregister)
			{
				auto pLog = pManager->GetSubsystem()->GetLogger_MainThread();
				Util::LogRecordScoper logRecordScoper(pLog, "ResourceManager: group is to be unregistered", Util::EMessageType::Status);
				*pLog << "Group:" << Util::endl;
				Log_Group(pLog, pGroupToUnregister->GetImpl(), Util::EMessageType::Status);
			}
		} // Impl
	} // ResManQuick
} // Dv