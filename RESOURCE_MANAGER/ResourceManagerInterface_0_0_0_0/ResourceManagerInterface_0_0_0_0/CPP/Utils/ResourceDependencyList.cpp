#include "../../H/Utils/ResourceDependencyList.h"
#include "../../H/Interface/IManagedResource.h"
#include "../../H/Override/IResourceLoader.h"
#include "../../H/Interface/IResourceType.h"
#include "../../H/Interface/IResourceManager.h"

#include <algorithm>

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			/**********************************************************************************************************
			* ResourceDependencyList implementation
			**********************************************************************************************************/
			ResourceDependencyList::~ResourceDependencyList()
			{
			}

			// Number of dependencies of the subresources with the given type
			size_t ResourceDependencyList::GetNumSubresourceDependencies(ISubresourceRole *pInRole) const
			{
				return GetNumSubresourceDependencies(pInRole->GetRoleIndex());
			}
			size_t ResourceDependencyList::GetNumSubresourceDependencies(SubresourceRoleIndex InRoleIndex) const
			{
				auto it_registered_dependency = _FindRegisteredDependencies(_dependencyLists, InRoleIndex);
				if (it_registered_dependency != _dependencyLists.cend())
				{
					auto result = it_registered_dependency->dependencies.size();
					return result;
				}
				else
				{
					return 0;
				}
			}
			size_t ResourceDependencyList::GetNumLoadingSubresourceDependencies(SubresourceRoleIndex InRoleIndex) const
			{
				auto it_registered_dependency = _FindRegisteredDependencies(_loadingDependencyLists, InRoleIndex);
				if (it_registered_dependency != _loadingDependencyLists.cend())
				{
					auto result = it_registered_dependency->dependencies.size();
					return result;
				}
				else
				{
					return 0;
				}
			}
			// Get dependency info for the dependency with the given subresource role and index
			const Dependency& ResourceDependencyList::GetDependency(SubresourceRoleIndex InRoleIndex, DependencyIndex InDependencyIndex) const
			{			
				assert(InDependencyIndex >= 0);
				const auto& dependency_list = _FindRegisteredDependencies(_dependencyLists, InRoleIndex);
				assert(InDependencyIndex < dependency_list->dependencies.size());
				const auto& result = dependency_list->dependencies[InDependencyIndex];
				return result;
			}
			const Dependency *ResourceDependencyList::GetLoadingDependency(SubresourceRoleIndex InRoleIndex, DependencyIndex InDependencyIndex) const
			{
				assert(InDependencyIndex >= 0);
				const auto& dependency_list = _FindRegisteredDependencies(_loadingDependencyLists, InRoleIndex);
				assert(InDependencyIndex < dependency_list->dependencies.size());
				const auto pResult = &(dependency_list->dependencies[InDependencyIndex]);
				return pResult;
			}
			const Dependency& ResourceDependencyList::GetDependency(ISubresourceRole *pInRole, DependencyIndex InDependencyIndex) const
			{
				assert(pInRole != nullptr);
				return GetDependency(pInRole->GetRoleIndex(), InDependencyIndex);
			}

			void ResourceDependencyList::LinkDependencyToManagedResource(ISubresourceRole *pInRole, DependencyIndex InDependencyIndex, IManagedResource *pInResource)
			{
				assert(pInRole != nullptr);
				return LinkDependencyToManagedResource(pInRole->GetRoleIndex(), InDependencyIndex, pInResource);
			}

			void ResourceDependencyList::LinkDependencyToManagedResource(SubresourceRoleIndex InRoleIndex, DependencyIndex InDependencyIndex, IManagedResource *pInResource)
			{				
				assert(InDependencyIndex >= 0);
				assert(_loading_roles.IsSet(InRoleIndex));
				Dependency *pDependency = _FindDependency(InRoleIndex, InDependencyIndex);
				assert(pDependency != nullptr);
				pDependency->LinkToManagedResource(pInResource);
			}

			void ResourceDependencyList::ClearSubresourceDependencies(SubresourceRoleSet InRoleSet)
			{
				assert(&InRoleSet != nullptr);
				for (auto& curr_role_dependencies : _dependencyLists)
				{
					if (InRoleSet.IsSet(curr_role_dependencies.roleIndex))
					{
						curr_role_dependencies.dependencies.clear();
						_loaded_dependency_roles.Exclude(curr_role_dependencies.roleIndex);
					}
				}
			}

			const Dependency *ResourceDependencyList::RegisterDependency(SubresourceRoleIndex InForSubresourceRole, IManagedResource *pInResource)
			{
				auto pDependency = _RegisterDependency(InForSubresourceRole, pInResource->GetLoader());
				pDependency->LinkToManagedResource(pInResource);
				return pDependency;
			}


			const Dependency *ResourceDependencyList::RegisterDependency(SubresourceRoleIndex InForSubresourceRole, std::shared_ptr<IResourceLoader> pInLoader)
			{
				return _RegisterDependency(InForSubresourceRole, pInLoader);
			}

			Dependency *ResourceDependencyList::_RegisterDependency(SubresourceRoleIndex InForSubresourceRoleIndex, std::shared_ptr<IResourceLoader> pInLoader)
			{
				// IMPLEMENTATION NOTES:
				//		This function must operate only on the container of loading dependencies (_loadingDependencyLists)
				// and NEVER touches the main dependencies container.
				//		Moreover, the function may NOT touch the loaded dependency roles set.

				assert(pInLoader != nullptr);
				auto pSubresourceRole = pInLoader->GetMainResourceParams().GetRole();

				auto it_role_dependencies = _FindRegisteredDependencies(_loadingDependencyLists, InForSubresourceRoleIndex);
				if (it_role_dependencies == _loadingDependencyLists.cend())
				{
					_loadingDependencyLists.push_back(RoleDependencies(InForSubresourceRoleIndex, Dependency(pInLoader)));
					auto &dependencies = _loadingDependencyLists.back().dependencies;
					// WARNING: We can NOT touch the loaded dependency roles set (see the function's IMPL details comment),
					// so we will update it within the NotifyLoadEnd()
					auto it_new_dependency = dependencies.begin();
					return &(*it_new_dependency);
				}
				else
				{
					auto &dependencies = it_role_dependencies->dependencies;
					dependencies.emplace_back(pInLoader);
					auto it_new_dependency = std::prev(dependencies.end());
					return &(*it_new_dependency);
				}
			}

			void ResourceDependencyList::NotifyLoadBegin(SubresourceRoleSet InRoles)
			{
				assert(&InRoles != nullptr);
				assert(!_loading_roles.Contains(InRoles));
				_loading_roles.MergeWith(InRoles);
			}

			void ResourceDependencyList::NotifyLoadEnd(SubresourceRoleSet InRoles)
			{
				assert(_loading_roles.Contains(InRoles));

				_loading_roles.Subtract(InRoles);
				// Here we merge the just-loaded dependency lists for roles those were in the loading state
				// with the main dependency list
				for (auto it_src = _loadingDependencyLists.begin(); it_src != _loadingDependencyLists.end(); ++it_src)
				{
					auto it_dest = _FindRegisteredDependencies(_dependencyLists, it_src->roleIndex);
					if (it_dest != _dependencyLists.end())
					{
						// delete the old list if it existed
						_dependencyLists.erase(it_dest);
					}					
				}
				_dependencyLists.splice(_dependencyLists.end(), _loadingDependencyLists);

				// Now the just-loaded dependency lists container (_loadingDependencyLists) must be empty
				assert(_loadingDependencyLists.empty());

				// We loaded the the dependencies for the given set of roles, 
				// so include the loaded roles in the loaded role set
				_loaded_dependency_roles.MergeWith(InRoles);

			}

			Dependency *ResourceDependencyList::_FindDependency(SubresourceRoleIndex InRoleIndex, DependencyIndex InDependencyIndex)
			{
				assert(InDependencyIndex >= 0);
				auto it_dependency_list = _FindRegisteredDependencies(_dependencyLists, InRoleIndex);
				if (_dependencyLists.cend() == it_dependency_list)
				{
					return nullptr;
				}
				if (it_dependency_list->dependencies.size() <= InDependencyIndex)
				{
					return nullptr;
				}
				auto pDependency = &(it_dependency_list->dependencies[InDependencyIndex]);
				return pDependency;
			}

			// _FindDependencies:
			// This function is provided to avoid code duplication for const and non-const containers
			template<class ContainerType> auto FindDependencies(ContainerType& InDependencyCont, SubresourceRoleIndex InRoleIndex) -> decltype(InDependencyCont.end())
			{
				assert(&InDependencyCont != nullptr);				
				for (auto it = InDependencyCont.begin(); it != InDependencyCont.end(); ++it)
				{
					if (it->roleIndex == InRoleIndex)
					{
						return it;
					}
				}
				return InDependencyCont.end();
			}

			ResourceDependencyList::DependencyLists::iterator ResourceDependencyList::_FindRegisteredDependencies(DependencyLists& InDependencyLists, SubresourceRoleIndex InRoleIndex)
			{
				return FindDependencies(InDependencyLists, InRoleIndex);
			}

			ResourceDependencyList::DependencyLists::const_iterator ResourceDependencyList::_FindRegisteredDependencies(const DependencyLists& InDependencyLists, SubresourceRoleIndex InRoleIndex) const
			{
				return FindDependencies(InDependencyLists, InRoleIndex);
			}

			/**********************************************************************************************************
			* RoleDependencies implementation
			**********************************************************************************************************/
			ResourceDependencyList::RoleDependencies::RoleDependencies(SubresourceRoleIndex InRoleIndex, Dependency&& InFirstDependency) :
				roleIndex(InRoleIndex)
			{
				dependencies.emplace_back(std::move(InFirstDependency));
			}

			/**********************************************************************************************************
			* Dependency implementation
			**********************************************************************************************************/
			Dependency::Dependency() :
				_pManagedResource(nullptr),
				_pDesiredLoader(nullptr)
			{
			}

			Dependency::Dependency(std::shared_ptr<IResourceLoader> pInLoader) :
				_pManagedResource(nullptr),
				_pDesiredLoader(pInLoader)
			{
			}



			std::shared_ptr<IResourceLoader> Dependency::GetDesiredLoader() const
			{
				return _pDesiredLoader;
			}

			void Dependency::LinkToManagedResource(IManagedResource *pInResource)
			{
				assert(pInResource != nullptr);
				_pManagedResource = pInResource;
				// Never nullify!!! See comments in the private variables section of the Dependency class
				assert(_pDesiredLoader.get() != nullptr);
			}

			void Dependency::SetDesiredLoader(std::shared_ptr<IResourceLoader> pInDesiredLoader)
			{
				// we cannot update the desired loader if it's already set
				// or the dependency already linked
				assert(pInDesiredLoader.get() == nullptr);
				assert(_pManagedResource == nullptr);
				_pDesiredLoader = pInDesiredLoader;
			}

			/****************************************************************************************************************************
			* DependencyEnumerator implementation
			****************************************************************************************************************************/
			bool DependencyEnumerator::_IsDependencyIndex_OutOfRange() const
			{
				auto bResult = dependencyIndex >= pDependencyList->GetNumSubresourceDependencies(GetSubresourceRoleIndex());
				return bResult;
			}

			DependencyEnumerator::DependencyEnumerator(const ResourceDependencyList *pInDependencyList, SubresourceRoleEnumerator InCurrentProcessingRole) :
				pDependencyList(pInDependencyList),
				enumerator_currentProcessingRole(InCurrentProcessingRole),
				dependencyIndex(0),
				_bMoved(false)
			{
				_Initialize_CurrentSubresourceRole();
			}

			void DependencyEnumerator::_Initialize_CurrentSubresourceRole()
			{
				if (!IsEnd() && _IsDependencyIndex_OutOfRange())
				{
					MoveNextSubresourceRole_WithDependencies();
				}
			}

			// Getting dependency: will return nullptr, if dependency is out of range
			const Dependency *DependencyEnumerator::GetDependency() const
			{
				if (IsEnd())
				{
					return nullptr;
				}
				auto& dependency = pDependencyList->GetDependency(enumerator_currentProcessingRole.GetRoleIndex(), dependencyIndex);
				return &dependency;
			}

			bool DependencyEnumerator::MoveNextSubresourceRole()
			{
				_bMoved = true;
				auto nextRole = enumerator_currentProcessingRole.MoveNext();
				if (nextRole.is_initialized())
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			bool DependencyEnumerator::MoveNextSubresourceRole_WithDependencies()
			{
				while (true)
				{
					MoveNextSubresourceRole();
					if (IsEnd())
					{
						return false;
					}
					if (!_IsDependencyIndex_OutOfRange())
					{
						return true;
					}
				}
			}

			bool DependencyEnumerator::MoveNext()
			{
				++dependencyIndex;
				if (_IsDependencyIndex_OutOfRange())
				{
					auto bIsEnd = MoveNextSubresourceRole_WithDependencies();
					return bIsEnd;
				}
				else
				{
					return true;
				}
			}

			/****************************************************************************************************************************
			* Loading dependency enumerator implementation
			****************************************************************************************************************************/
			bool LoadingDependencyEnumerator::_IsDependencyIndex_OutOfRange() const
			{
				auto bResult = dependencyIndex >= pDependencyList->GetNumLoadingSubresourceDependencies(GetSubresourceRoleIndex());
				return bResult;
			}

			LoadingDependencyEnumerator::LoadingDependencyEnumerator(const ResourceDependencyList *pInDependencyList, SubresourceRoleEnumerator InCurrentProcessingRole) :
				pDependencyList(pInDependencyList),
				enumerator_currentProcessingRole(InCurrentProcessingRole),
				dependencyIndex(0),
				_bMoved(false)
			{
				_Initialize_CurrentSubresourceRole();
			}

			void LoadingDependencyEnumerator::_Initialize_CurrentSubresourceRole()
			{
				if (!IsEnd() && _IsDependencyIndex_OutOfRange())
				{
					MoveNextSubresourceRole_WithDependencies();
				}
			}

			// Getting dependency: will return nullptr, if dependency is out of range
			const Dependency *LoadingDependencyEnumerator::GetDependency() const
			{
				if (IsEnd())
				{
					return nullptr;
				}
				auto pDependency = pDependencyList->GetLoadingDependency(enumerator_currentProcessingRole.GetRoleIndex(), dependencyIndex);
				return pDependency;
			}

			bool  LoadingDependencyEnumerator::MoveNextSubresourceRole()
			{
				_bMoved = true;
				auto nextRole = enumerator_currentProcessingRole.MoveNext();
				if (nextRole.is_initialized())
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			bool  LoadingDependencyEnumerator::MoveNextSubresourceRole_WithDependencies()
			{
				while (true)
				{
					MoveNextSubresourceRole();
					if (IsEnd())
					{
						return false;
					}
					if (!_IsDependencyIndex_OutOfRange())
					{
						return true;
					}
				}
			}

			bool  LoadingDependencyEnumerator::MoveNext()
			{
				++dependencyIndex;
				if (_IsDependencyIndex_OutOfRange())
				{
					auto bIsEnd = MoveNextSubresourceRole_WithDependencies();
					return bIsEnd;
				}
				else
				{
					return true;
				}
			}

			/***************************************************************************************************************************************
			* ResourceDependencyList utils
			****************************************************************************************************************************************/
			void Log_DependencyList(Util::ILoggingDevice *pInLog, const ResourceDependencyList *pInDependencyList, IResourceManager *pInResourceManager)
			{
				auto loadingSubresourceRoles = pInDependencyList->GetLoadingSubresourceRoles();
				*pInLog << "loadingSubresourceRoles: " << ToString_SubresourceRoleSet(loadingSubresourceRoles, pInResourceManager) << Util::endl;
				auto loadedSubresourceDependencies = pInDependencyList->GetLoadedSubresourcesDependencies();
				*pInLog << "loadedSubresourceDependencies: " << ToString_SubresourceRoleSet(loadedSubresourceDependencies, pInResourceManager) << Util::endl;

				auto pSubresourceRoleEnumerator = std::move(pInResourceManager->GetSubresourceRoles());
				while (true)
				{
					auto pSubresourceRole = pSubresourceRoleEnumerator->GetCurrent();
					if (nullptr == pSubresourceRole)
					{
						break;
					}

					// Enumerate all dependency resources of the given role
					auto numDependencies = pInDependencyList->GetNumSubresourceDependencies(pSubresourceRole);
					if (numDependencies > 0)
					{
						std::string perRoleScoper_title; 

						perRoleScoper_title.append("Dependencies of role "+ToString_SubresourceRole(pSubresourceRole));

						Util::LogRecordScoper perRoleScoper(pInLog, perRoleScoper_title, Util::EMessageType::Info);
						*pInLog << "numDependencies: " << numDependencies << Util::endl;

						for (auto dependencyIndex = 0; dependencyIndex < numDependencies; dependencyIndex++)
						{
							auto pDependency = &(pInDependencyList->GetDependency(pSubresourceRole->GetRoleIndex(), dependencyIndex));
							Log_Dependency(pInLog, dependencyIndex, pDependency);
						}
					}

					pSubresourceRoleEnumerator->MoveNext();
				}
			}
			void Log_Dependency(Util::ILoggingDevice *pInLog, ResourceDependencyList::DependencyIndex InIndex, const Dependency *pInDependency)
			{
				auto pDesiredLoader = pInDependency->GetDesiredLoader();
				auto pManagedResource = pInDependency->GetManagedResource();

				*pInLog << "Index=" << InIndex;
				Util::NotYetImplVoid_Silent("Log loader lean and mean");
				Util::NotYetImplVoid_Silent("Log ManagedResource lean and mean");
			}

			SubresourceRoleSet BeginLoadExtraDependencyLists(ResourceDependencyList *pOutDependencyList, SubresourceRoleSet InSubresourceRoles)
			{
				auto extraDependencyLists = InSubresourceRoles;
				extraDependencyLists.Subtract(pOutDependencyList->GetLoadingSubresourceRoles());
				if(false == extraDependencyLists.IsEmpty())
				{
					pOutDependencyList->NotifyLoadBegin(extraDependencyLists);
				}
				return extraDependencyLists;
			}
			void EndLoadingDependencyLists(ResourceDependencyList *pOutDependencyList)
			{
				auto subresourceRolesToEndLoading = pOutDependencyList->GetLoadingSubresourceRoles();
				pOutDependencyList->NotifyLoadEnd(subresourceRolesToEndLoading);
			}

			void CopySubresourceDependencies(SubresourceRoleSet InSubresourceRoleSet, const ResourceDependencyList *pInSourceDependencyList, ResourceDependencyList *pInDestDependencyList, bool bCallBeginEnd)
			{
				auto subresourceRoleEnumerator = InSubresourceRoleSet.GetEnumerator();
				while (!subresourceRoleEnumerator.IsEnd())
				{
					CopySubresourceRoleDependencies(subresourceRoleEnumerator.GetRoleIndex(), pInSourceDependencyList, pInDestDependencyList, bCallBeginEnd);
					subresourceRoleEnumerator.MoveNext();
				}
			}
			void CopySubresourceRoleDependencies(SubresourceRoleIndex InRoleIndex, const ResourceDependencyList *pInSourceDependencyList, ResourceDependencyList *pInDestDependencyList, bool bCallBeginEnd)
			{
				assert(pInSourceDependencyList != nullptr);
				assert(pInDestDependencyList != nullptr);
				if (bCallBeginEnd)
				{
					pInDestDependencyList->NotifyLoadBegin(SubresourceRoleSet({ InRoleIndex }));
				}
				size_t numDependenciesToCopy = pInSourceDependencyList->GetNumSubresourceDependencies(InRoleIndex);
				for (ResourceDependencyList::DependencyIndex index = 0; index < numDependenciesToCopy; index++)
				{
					const auto& sourceDependency = pInSourceDependencyList->GetDependency(InRoleIndex, index);
					if(sourceDependency.GetManagedResource() == nullptr)
					{
						pInDestDependencyList->RegisterDependency(InRoleIndex, sourceDependency.GetDesiredLoader());
					}
					else
					{
						pInDestDependencyList->RegisterDependency(InRoleIndex, sourceDependency.GetManagedResource());
					}
				}
				if (bCallBeginEnd)
				{
					pInDestDependencyList->NotifyLoadEnd(SubresourceRoleSet({ InRoleIndex }));
				}
			}
		}
	}
}