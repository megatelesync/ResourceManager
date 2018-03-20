#pragma once

#include "../Core/ResourceManagerException.h"
#include "../Interface/IResourceType.h"
#include "../Override/IResourceTypeDescr.h"
#include "../Utils/ISubresourceRole.h"
#include "ResourceManagerSubsystem_Params.h"

namespace Dv
{	
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			class IResourceManager;

			/******************************************************************************
			* Current TODO:
			* (WARNING: Old TODO is on the bottom of this file)
			******************************************************************************/
			class IResourceManagerSubsystem
			{
			public:
				/*************************************************************************************************
				* TYPEDEFS
				*************************************************************************************************/
				using TickIndex = uint32_t;

				/*************************************************************************************************
				* CONSTRUCTION
				*************************************************************************************************/
				IResourceManagerSubsystem                                          () = default;

				/*************************************************************************************************
				* SPECIAL FUNCTIONS
				*************************************************************************************************/
				virtual                           ~IResourceManagerSubsystem       ()                                   = default;
				 
				IResourceManagerSubsystem                                          (const IResourceManagerSubsystem&)   = delete;
				IResourceManagerSubsystem&        operator=                        (const IResourceManagerSubsystem&)   = delete;

				/*************************************************************************************************
				* GETTING PARAMETERS
				*************************************************************************************************/
				virtual const ResourceManagerSubsystemParams   *GetCommonParams    () const                             =0;

				/*************************************************************************************************
				* INITIALIZATION
				*************************************************************************************************/
				// Initializes and starts the subsystem.
				// After this function is called, the subsystem is ready to be used.
				// (Creates and starts all necessary threads)
				virtual void                      InitializeAndStart               () throw(ResourceManagerException)    =0;
				virtual void                      Stop                             ()                                    =0;

				/***************************************************************************************************
				* Getting the main interface object
				***************************************************************************************************/
				virtual IResourceManager          *GetManager                      ()                                                                   =0;

				/***************************************************************************************************
				* Ticking
				***************************************************************************************************/
				// Tick - delta seconds is the number of simulation (NOT realtime) seconds since the last frame
				virtual void                       Tick                             (double InDeltaSeconds)                       =0;
				virtual TickIndex                  GetTickIndex                     () const                                      =0;
				virtual double                     GetSimulationTime                () const                                      =0;

				/***************************************************************************************************
				* Controlling execution
				***************************************************************************************************/
				virtual const ResourceManagerPublicRuntimeParams&        GetRuntimeParams        () const                                                =0;
				// Update the runtime params with the new value
				virtual void                                             UpdateRuntimeParams     (const ResourceManagerPublicRuntimeParams& InParams)    =0;

				/********************************************************************************************
				* All resource types to be registered before calling the initialization function.
				* The registered resource type DESCR will NOT be deleted at the time of the subsystem destruction;
				* The returned IResourceType instance will be owned by the resource manager subsystem.
				*********************************************************************************************/
				virtual IResourceType             *RegisterResourceType            (IResourceTypeDescr *pInType)                  =0;

				/****************************************************************************************
				* Working with roles
				****************************************************************************************/
				virtual ISubresourceRole          *RegisterRole                    (const SubresourceRolePublicParams& InParams) = 0;
			};

			/************************************************************************************
			* Global functions to work with the resource manager subsystem interface
			************************************************************************************/
			// Register the given typed resource type using the given subsystem
			template<class ResourceTypeDescrArg>
			void                     RegisterType                                  (IResourceManagerSubsystem *pInSubsystem, TypedResourceType<ResourceTypeDescrArg> *pInTypedResourceType);

			ISubresourceRole        *RegisterRealResourceSubresourceRole           (IResourceManagerSubsystem *pInSubsys, ResourceTypeSubresourceRoleParams *pOutSubresourceRoles, const std::string& InName);
			// Register subresource role that may be not loaded
			ISubresourceRole        *RegisterSubresourceRole                       (IResourceManagerSubsystem *pInSubsys, ResourceTypeSubresourceRoleParams *pOutSubresourceRoles, const std::string& InName);
			// Register subresource role that must be loaded to create the resource
			ISubresourceRole        *RegisterSubresourceRole_ThatMustBeLoaded      (IResourceManagerSubsystem *pInSubsys, ResourceTypeSubresourceRoleParams *pOutSubresourceRoles, const std::string& InName, bool bMustBeLoadedWhileResourceIsLoaded = true);

			/************************************************************************************
			* ResourceManagerPublicRuntimeParams implementation
			************************************************************************************/
			inline bool ResourceManagerPublicRuntimeParams::IsFreezed() const
			{
				return _bFreezed;
			}
			
			inline bool ResourceManagerPublicRuntimeParams::IsDecisionMakingEnabled() const
			{
				return _bDecisionMakingEnabled;
			}
			
			inline void ResourceManagerPublicRuntimeParams::SetFreezed(bool bInValue)
			{
				_bFreezed = bInValue;
			}
			inline void ResourceManagerPublicRuntimeParams::SetDecisionMakingEnabled(bool bInEnabled)
			{
				_bDecisionMakingEnabled = bInEnabled;
			}

			/************************************************************************************
			* Global functions imlementation
			************************************************************************************/
			template<class ResourceTypeDescrArg>
			void RegisterType(IResourceManagerSubsystem *pInSubsystem, TypedResourceType<ResourceTypeDescrArg> *pInTypedResourceType)
			{
				assert(pInSubsystem != nullptr);
				auto pRegisteredType = pInSubsystem->RegisterResourceType(pInTypedResourceType->GetDescr());
				pInTypedResourceType->Initialize(pRegisteredType);
			}

			/*****************************************************************************************************
			* OLD TODO 1
			* 1. IResourceManagerSubsystem (+ CODED: 23.12.2016/1:41)
			* 2. IResourceManager (+ CODED: 23.12.2016/1:41)
			* 3. IResourceType (+ CODED: 23.12.2016/1:41)
			* 4. ISubresourceRole (+ CODED: 23.12.2016/1:41)
			* 5. IManagedResource
			* 5.1. Provide access to the resource loader (+ DONE: 22.12.2016/2:32)
			* 5.2. Resource can belong to many groups at the same time (+ DONE: 22.12.2016/2:32)
			* 6. IManagedGroup
			*
			* 7. IResourceLoader
			* 7.1. Create multiple resource per loader support (+ CODED: 23.12.2016/1:41)
			* 7.2. How to dynamically load the adressable subresource? (+ ANSWERED: 23.12.2016/1:41)
			* 8. ResourceDependencyList
			* 8.1. Separate real used loader (in the case when no given resource exists) and the desired loader
			* (+ CODED: 23.12.2016/1:41)
			* 8.2. Implement the class
			* 9. SubresourceRoleSet (+ CODED: 23.12.2016/1:41)
			* 9.1. Replace all "ResourceRoleSet" with "SubresourceRoleSet" (+ CODED: 23.12.2016/1:41)
			* 9.2. Do we really need the enumerator of set elements? (+ ANSWERED: 23.12.2016/1:41)
			* 10. Create class that represents set of roles for many resources (+ CODED: 23.12.2016/1:41)
			* 10.1. Make resource role shared between resource types: (+ CODED: 23.12.2016/1:41)
			* 10.1.1. Registration (+ CODED: 23.12.2016/1:41)
			* 10.1.2. Link replacement (+ CODED: 23.12.2016/1:41)
			* 10.1.3. Remove from ResourceType (+ CODED: 23.12.2016/1:41)
			* 10.1.4. ISubresourceRole::
			* 10.1.4.1. GetResources() (+ CODED: 23.12.2016/1:41)
			* 10.1.4.2. GetResourceByURL() (+ CODED: 23.12.2016/1:41)
			* 10.2. SubresourceRoleSet (+ CODED: 23.12.2016/1:41)
			******************************************************************************************************/
		} // Ver_0_0_0_0
	} // ResourceManager
} // Dv