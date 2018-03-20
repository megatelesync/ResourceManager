#pragma once

#include "IResourceType.h"
#include "IManagedResource.h"
#include "IManagedGroup.h"

#include "Enumerator/Util_IEnumerator.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			/***************************************************************************************
			* Main interface of the manager
			***************************************************************************************/
			class IResourceManager
			{
			public:
				/**********************************************************************************
				* TYPEDEFS
				**********************************************************************************/
				using ResourceEnumerator      = std::unique_ptr<Util::IEnumerator<IManagedResource>>;
				using ResourceTypeEnumerator  = std::unique_ptr<Util::IEnumerator<IResourceType>>;
				using GroupEnumerator         = std::unique_ptr<Util::IEnumerator<IManagedGroup>>;
				using RoleEnumerator          = std::unique_ptr<Util::IEnumerator<ISubresourceRole>>;

				/**********************************************************************************
				* CONSTRUCTION
				**********************************************************************************/
				IResourceManager                                                   () = default;

				/**********************************************************************************
				* SPECIAL FUNCTIONS
				**********************************************************************************/
				virtual                              ~IResourceManager             ()                                             = default;

				IResourceManager                                                   (const IResourceManager&)                      = delete;
				IResourceManager&                    operator=                     (const IResourceManager&)                      = delete;

				/**********************************************************************************
				* GETTING SUBSYSTEM
				**********************************************************************************/
				virtual IResourceManagerSubsystem            *GetSubsystem         () const = 0;

				/**********************************************************************************
				* Working with resource types
				**********************************************************************************/
				virtual ResourceTypeEnumerator               GetResourceTypes       ()                                             =0;

				/**********************************************************************************
				* Enumerating all resources
				*
				* There's method to enumerate resources by the given subresource role in the role interface.
				**********************************************************************************/
				virtual ResourceEnumerator                   GetResourceEnumerator  ()                                             =0;
				virtual ResourceEnumerator                   GetResourceWithURL     ()                                             =0;

				/**********************************************************************************
				* Find resource by URL
				**********************************************************************************/
				// Find registered resource with the given URL (returns nullptr when not found)
				virtual IManagedResource                    *FindResourceByURL     (const std::string& InURL)                      =0;

				/**********************************************************************************
				* Registering resources
				**********************************************************************************/
				virtual IManagedResource            *RegisterResource              (const PublicResourceParams& InParams, std::shared_ptr<IResourceLoader> pInLoader)               =0;
				virtual void                         UnregisterResource            (IManagedResource *pInResource)                =0;

				/**********************************************************************************
				* Registering groups
				**********************************************************************************/
				virtual IManagedGroup               *RegisterGroup                 (const GroupParams& InParams)            =0;
				virtual void                         UnregisterGroup               (IManagedGroup *pInGroup)                      =0;

				/**********************************************************************************
				* Registering groups
				**********************************************************************************/
				virtual GroupEnumerator             GetGroups                     ()                                              =0;

				/**********************************************************************************
				* Registering groups
				**********************************************************************************/
				virtual IManagedResource            *PopFailedResource           ()                                               =0;
				virtual IManagedGroup               *PopFailedGroup              ()                                               =0;

				/**********************************************************************************
				* Working with subresource roles
				**********************************************************************************/
				virtual RoleEnumerator               GetSubresourceRoles         ()                                                =0;
				virtual ISubresourceRole            *GetSubresourceRoleByIndex   (SubresourceRoleIndex InIndex)                    =0;

				/**********************************************************************************
				* Waiting for events
				**********************************************************************************/
				// Wait until all commands running on all loading loops processed.
				virtual void                        Wait_UntilAllCommandsProcessed() =0;
				// Wait until all commands for the given resource are processed;
				// If bInRecurisve is true, then child commands are also waited for to complete.
				virtual void                        Wait_UntilAllResourceCommandsProcessed(IManagedResource *pInResource, bool bInRecurisve) =0;
				// Wait until all commands for the given group are processed;
				// If bInRecurisve is true, then child commands are also waited for to complete.
				virtual void                        Wait_UntilAllGroupCommandsProcessed(IManagedGroup *pInGroup, bool bInRecurisve) =0;

				// Wait until all commands that run NOT on the main thread have been processed.
				// WARNING!!! The non-threaded child commands will not be waited for to complete.
				virtual void                        Wait_UntilAllThreadedCommandsProcessed() =0;
				// Wait until all commands connected with the given resource that run NOT on the main thread have been processed.
				// WARNING!!! The non-threaded child commands will not be waited for to complete.
				virtual void                        Wait_UntilAllThreadedCommandsForResourceProcessed(IManagedResource *pInResource, bool bInRecursive) =0;
			};

			// Return first existing full filename for the given filename;
			// If the given filename is full path, it's used as-is;
			// if the given filename is not full path, then try to search for the first existing file in data directories
			// (base data directories, and directories specific to the given resoruce type).
			//
			// if the file does not exist in any of the directories to search, default URL is returned (without testing that it exists).
			// Returns unitialized boost::optional<> only if the default URL is not specified for the given resource.
			boost::optional<std::string>            GetExistingFullFilename                                   (IResourceManagerSubsystem *pInSubsystem, const std::string& InFilename, IResourceType *pInResourceType);

			/************************************************************************************
			* Make subresource role set that contains all registered subresource roles
			************************************************************************************/
			SubresourceRoleSet                      MakeSubresourceRoleSet_AllRoles                           (IResourceManager *pInManager);

			/***********************************************************************************
			* Converting to string
			************************************************************************************/
			std::string                            ToString_SubresourceRoleSet                                (SubresourceRoleSet InSet, IResourceManager *pInManager);
		}
	}
} 
