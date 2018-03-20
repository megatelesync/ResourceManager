#pragma once

#include "ISubresourceRole.h"

#include "Enumerator\Util_IEnumerator.h"
#include <cassert>
#include <list>
#include <vector>
#include "Log/Util_ILoggingDevice.h"
#include <set>

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			class IManagedResource;
			class IResourceLoader;
			class IResourceManager;

			/***********************************************************************************
			* Typedefs
			***********************************************************************************/
			using DependencyListLoaderCont = std::set<IResourceLoader *>;

			/***********************************************************************************
			* Dependency of a resource
			*
			* This type is move-only.
			***********************************************************************************/
			class Dependency
			{
			public:
				/**********************************************************************************
				* CONSTRUCTION
				**********************************************************************************/
				// default ctor: initialize with nullptr desired loader
				Dependency                                   ();
				// initialize with the given desired loader
				Dependency                                   (std::shared_ptr<IResourceLoader> pInDesiredLoader);

				/**********************************************************************************
				* GETTERS
				**********************************************************************************/
				// Pointer to the corresponding managed resource
				// (nullptr if the managed resource is not created yet)
				IManagedResource                                  *GetManagedResource     () const                 { return _pManagedResource; }

				/**********************************************************************************
				* Loaders
				*
				* The desired loader for the resource cannot be changed after it's assigned.
				* The loader is owned by the dependency until it's linked to the managed resource
				* (since that moment, the loader will be managed by the managed resource).
				* No matter what object owns the loader, the loader getter function
				* returns the correct desired loader pointer.
				**********************************************************************************/
				std::shared_ptr<IResourceLoader>                  GetDesiredLoader       () const;

				/**********************************************************************************
				* SETTERS
				**********************************************************************************/
				// Set pointer to the managed resource that this dependency points to
				void                                               LinkToManagedResource   (IManagedResource *pInResource);
				// Setting desired loader: use when cannot initialize with ctor
				void                                               SetDesiredLoader        (std::shared_ptr<IResourceLoader> pInDesiredLoader);

			private:
				IManagedResource                                  *_pManagedResource;

				// The desired loader: never nulled in initilized state
				// (ever if attached to a managed resource)
				std::shared_ptr<IResourceLoader>                   _pDesiredLoader;
			};


			/***********************************************************************************************************
			* List of resource dependencies
			*
			* THREADING:
			* The class is itself is NOT thread-safe;
			*
			* The following functions are typically to be called on the main thread:
			*     - LinkDependencyToManagedResource()
			*     - NotifyLoadBegin()/NotifyLoadEnd()
			*     - ClearSubresourceDependencies()
			* The following functions are typically to be called on the loader thread:
			*     - RegisterDependency()
			* WARNING!!! MAYBE: Hide GetLoadedSubresourceDependencies!!! For thread-safety it must be inaccessible
			* from the managed resource interface!!!
			***********************************************************************************************************/
			class ResourceDependencyList
			{
			public:
				/*****************************************************************************************************
				* TYPEDEFS
				*****************************************************************************************************/
				using DependencyIndex = uint32_t;

				/*****************************************************************************************************
				* SPECIAL FUNCTIONS
				*****************************************************************************************************/
				~ResourceDependencyList();

				/*****************************************************************************************************
				* States of subresource dependency lists
				*****************************************************************************************************/
				// For the given subresource type the dependencies are treated
				// as loaded if the dependency load operation was performed for the corresponding subresource type
				// (ever if the load result contains zero dependencies)
				SubresourceRoleSet            GetLoadedSubresourcesDependencies             () const;
				// Get set of subresource roles which dependencies are loading at the moment
				SubresourceRoleSet            GetLoadingSubresourceRoles                    () const;

				/*****************************************************************************************************
				* Getting dependencies
				*
				* Dependencies are to be getted by index due to the following reasons:
				* 1. We want to encapsulate the Dependency object from the direct non-const access
				* so we cannot use the non-const pointer, but we need to modify through the dependency list interface
				* 2. We want to be able to update a given dependency
				* through the dependency list interface, but we cannot use the enumerator
				* because it's not designed for the purpose of being used as the pointer
				* (its only goal is to enumerate).
				* 3. Dependencies are not deleted randomly from a list of dependencies, so we can use the dynamic array.
				********************************************************************************************************/
				// Number of dependencies of the subresources with the given type
				size_t                               GetNumSubresourceDependencies                 (ISubresourceRole *pInRole) const;
				size_t                               GetNumSubresourceDependencies                 (SubresourceRoleIndex InRoleIndex) const;
				size_t                               GetNumLoadingSubresourceDependencies          (SubresourceRoleIndex InRoleIndex) const;
				// Get dependency info for the dependency with the given subresource role and index
				const Dependency&                    GetDependency                                 (ISubresourceRole *pInRole, DependencyIndex InDependencyIndex) const;
				const Dependency&                    GetDependency                                 (SubresourceRoleIndex InRoleIndex, DependencyIndex InDependencyIndex) const;
				// Get dependency in the list of loading dependencies
				const Dependency                    *GetLoadingDependency                          (SubresourceRoleIndex InRoleIndex, DependencyIndex InDependencyIndex) const;
				

				/*****************************************************************************************************
				* Attaching managed resources
				*****************************************************************************************************/
				// Link the given managed resource to the given dependency:
				// - the provided resource CAN be nullptr;
				// - dependency for the given role and dependency index must be loaded;
				// - Linking to managed resource can NOT be done if dependency list for the corresponding role is loading.
				void                                 LinkDependencyToManagedResource               (ISubresourceRole *pInRole, DependencyIndex InDependencyIndex, IManagedResource *pInResource);
				void                                 LinkDependencyToManagedResource               (SubresourceRoleIndex InRoleIndex, DependencyIndex InDependencyIndex, IManagedResource *pInResource);

				/*****************************************************************************************************
				* Loading and registering dependencies
				* (this interface part is provided for the resource manager implementation, 
				* not for the resource manager user)
				*
				* THREAD-SAFETY
				* While loading dependency lists of the given set of roles on one thread,
				* these dependency lists may be getted from another thread
				* (the old lists are to be returned).
				*
				* Calling the loaded dependency registration function (RegisterDependency())
				* at the moment of calling NotifyLoadBegin()/NotifyLoadEnd()
				* is not thread-safe.
				*****************************************************************************************************/
				// Notify start of dependency loading for the given set of subresource roles
				// (WARNING! none of the given roles' dependencies may be loading at the moment!)
				void                                 NotifyLoadBegin                               (SubresourceRoleSet InRoles);
				// Notify end of the dependency loading
				// (WARNING! all of the given roles' dependencies must be loading at the moment!)
				void                                 NotifyLoadEnd                                 (SubresourceRoleSet InRoles);

				// Loaded dependency registration function.
				// Register resource dependency of the subresource type that is loading at this moment
				// (the function must be called between the NotifyLoadBegin()/NotiftLoadEnd() pair),
				// Returns const pointer to the registered dependency.
				// Pointer (vs. reference) is used because it's intended to be saved.
				const Dependency                    *RegisterDependency                            (SubresourceRoleIndex InForSubresourceRole, std::shared_ptr<IResourceLoader> pInLoader);
				const Dependency                    *RegisterDependency                            (SubresourceRoleIndex InForSubresourceRole, IManagedResource *pInResource);

				/*****************************************************************************************************
				* Clearing dependencies
				* (this interface part is provided for the loader)
				*****************************************************************************************************/
				// Clear dependency lists for the given subresources
				void                                 ClearSubresourceDependencies                  (SubresourceRoleSet InRoleSet);

			private:
				// Dependencies for the given role
				struct RoleDependencies
				{					
					SubresourceRoleIndex                    roleIndex;
					std::vector<Dependency>                 dependencies;

					// Register the dependency list for the given role and the first dependency
					// (that is to be moved into the container)
					RoleDependencies(SubresourceRoleIndex InRoleIndex, Dependency&& InFirstDependency);
				};

				// Set of dependency lists of all roles
				// (list is faster than the map for short sequences and allows splicing)
				using DependencyLists                     = std::list<RoleDependencies>;

				// Find iterator to the dependency list of the given role in the given list
				// (or cend() if not found)
				static DependencyLists::iterator                 _FindRegisteredDependencies(DependencyLists& InDependencyLists, SubresourceRoleIndex InRoleIndex);
				DependencyLists::const_iterator                  _FindRegisteredDependencies(const DependencyLists& InDependencyLists, SubresourceRoleIndex InRoleIndex) const;
				// Find dependency withing list of loading dependencies
				Dependency                                      *_FindDependency(SubresourceRoleIndex InRoleIndex, DependencyIndex InDependencyIndex);

				Dependency                                      *_RegisterDependency(SubresourceRoleIndex InForSubresourceRole, std::shared_ptr<IResourceLoader> pInLoader);

				// The main dependency list
				DependencyLists                           _dependencyLists;
				SubresourceRoleSet                        _loaded_dependency_roles;

				// Dependencies for loading element
				DependencyLists                           _loadingDependencyLists;
				// Dependency roles that are loading at the moment
				SubresourceRoleSet                        _loading_roles;
			};
			// Copy dependencies for the given role(s) without clearing the destination dependency list before the copying.
			// Managed resource pointer is also copied if binded for a dependency.
			// IF bCallBeginEnd == true, Notify_LoadBegin()/Notify_LoadEnd are called automatically.
			void                                          CopySubresourceDependencies(SubresourceRoleSet InSubresourceRoleSet, const ResourceDependencyList *pInSourceDependencyList, ResourceDependencyList *pInDestDependencyList, bool bCallBeginEnd);
			void                                          CopySubresourceRoleDependencies(SubresourceRoleIndex InRoleIndex, const ResourceDependencyList *pInSourceDependencyList, ResourceDependencyList *pInDestDependencyList, bool bCallBeginEnd);

			// Begin load only dependency lists for subresource roles that are not loading.
			// (this function is necessary because ResourceDependencyList::NotifyLoadBegin()
			// cannot be called for resources that are loading at the moment)
			// Returns: set of extra dependency lists to be loaded.
			SubresourceRoleSet                            BeginLoadExtraDependencyLists(ResourceDependencyList *pOutDependencyList, SubresourceRoleSet InSubresourceRoles);
			// Call NotifyLoadEnd() for dependency lists that are loading at the moment.
			void                                          EndLoadingDependencyLists(ResourceDependencyList *pOutDependencyList);

			// Logging
			void                                          Log_DependencyList(Util::ILoggingDevice *pInLog, const ResourceDependencyList *pInDependencyList, IResourceManager *pInResourceManager);
			void                                          Log_Dependency(Util::ILoggingDevice *pInLog, ResourceDependencyList::DependencyIndex, const Dependency *pInDependency);


			/*******************************************************************************************************
			* Dependency enumerator
			*
			* This enumerator enumerates all loaded dependencies within the given dependency list
			* that belong to the given set of subresource roles
			*
			* Right after construction the enumerator state points to first dependency
			* (if possible, otherwise IsEnd() returns true).
			*******************************************************************************************************/
			class DependencyEnumerator
			{
			public:
				/******************************************************************************************************
				* Construction
				******************************************************************************************************/
				DependencyEnumerator                                                                  (const ResourceDependencyList *pInDependencyList, SubresourceRoleEnumerator InCurrentProcessingRole);

				/******************************************************************************************************
				* Special operations
				******************************************************************************************************/
				DependencyEnumerator                                                                 (const DependencyEnumerator&) = default;
				DependencyEnumerator&                 operator=                                      (const DependencyEnumerator&) = default;

				/******************************************************************************************************
				* Getters
				******************************************************************************************************/
				// Get number of dependencies of the given role
				size_t                                GetNumDependencies_OfSubresourceRole           () const { return pDependencyList->GetNumSubresourceDependencies(GetSubresourceRoleIndex()); }
				SubresourceRoleIndex                  GetSubresourceRoleIndex                        () const { return enumerator_currentProcessingRole.GetRoleIndex(); }
				int                                   GetDependencyIndex                             () const { return dependencyIndex; }
				// Getting dependency: will return nullptr, if dependency is out of range
				const Dependency                     *GetDependency                                  () const;

				/******************************************************************************************************
				* Is no more dependencies
				******************************************************************************************************/
				bool                                  IsEnd                                          () const
				{ 
					return enumerator_currentProcessingRole.IsEnd(); 
				}

				/******************************************************************************************************
				* Moving
				******************************************************************************************************/
				// true if some move operaion was called
				bool                                  HasBeenMoved                                  () const { return _bMoved; }
				// Move to next dependency (always moves to correct dependency, if possible)
				bool                                  MoveNext                                      ();

				// Move to next subresource role (whic may contain depedencies, or may not)
				bool                                  MoveNextSubresourceRole                       ();
				// Move to next subresource role for which (!) at least one dependency is registered
				bool                                  MoveNextSubresourceRole_WithDependencies      ();

			private:
				void                                               _Initialize_CurrentSubresourceRole();
				bool                                               _IsDependencyIndex_OutOfRange() const;

				const ResourceDependencyList                       *pDependencyList;
				int                                                 dependencyIndex;
				SubresourceRoleEnumerator                           enumerator_currentProcessingRole;
				bool                                                _bMoved;
			};

			class LoadingDependencyEnumerator
			{
			public:
				/******************************************************************************************************
				* Construction
				******************************************************************************************************/
				LoadingDependencyEnumerator(const ResourceDependencyList *pInDependencyList, SubresourceRoleEnumerator InCurrentProcessingRole);

				/******************************************************************************************************
				* Special operations
				******************************************************************************************************/
				LoadingDependencyEnumerator(const LoadingDependencyEnumerator&) = default;
				LoadingDependencyEnumerator&                 operator=                                      (const LoadingDependencyEnumerator&) = default;

				/******************************************************************************************************
				* Getters
				******************************************************************************************************/
				// Get number of dependencies of the given role
				size_t                                GetNumDependencies_OfSubresourceRole() const { return pDependencyList->GetNumSubresourceDependencies(GetSubresourceRoleIndex()); }
				SubresourceRoleIndex                  GetSubresourceRoleIndex() const { return enumerator_currentProcessingRole.GetRoleIndex(); }
				int                                   GetDependencyIndex() const { return dependencyIndex; }
				// Getting dependency: will return nullptr, if dependency is out of range
				const Dependency                     *GetDependency() const;

				/******************************************************************************************************
				* Is no more dependencies
				******************************************************************************************************/
				bool                                  IsEnd() const
				{
					return enumerator_currentProcessingRole.IsEnd();
				}

				/******************************************************************************************************
				* Moving
				******************************************************************************************************/
				// true if some move operaion was called
				bool                                  HasBeenMoved() const { return _bMoved; }
				// Move to next dependency (always moves to correct dependency, if possible)
				bool                                  MoveNext();

				// Move to next subresource role (whic may contain depedencies, or may not)
				bool                                  MoveNextSubresourceRole();
				// Move to next subresource role for which (!) at least one dependency is registered
				bool                                  MoveNextSubresourceRole_WithDependencies();

			private:
				void                                               _Initialize_CurrentSubresourceRole();
				bool                                               _IsDependencyIndex_OutOfRange() const;

				const ResourceDependencyList                       *pDependencyList;
				int                                                 dependencyIndex;
				SubresourceRoleEnumerator                           enumerator_currentProcessingRole;
				bool                                                _bMoved;
			};

			/*********************************************************************************************************
			* IMPLEMENTATION
			*********************************************************************************************************/
			inline SubresourceRoleSet ResourceDependencyList::GetLoadedSubresourcesDependencies() const
			{
				return _loaded_dependency_roles;
			}
			inline SubresourceRoleSet ResourceDependencyList::GetLoadingSubresourceRoles() const
			{
				return _loading_roles;
			}
		}
	}
}
