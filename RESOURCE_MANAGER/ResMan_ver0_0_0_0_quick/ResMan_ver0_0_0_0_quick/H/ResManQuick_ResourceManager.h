#pragma once

#include "ResManQuick_Fwd.h"
#include "ResManQuick_System.h"
#include "interface/IResourceManager.h"
#include "../PrivateH/ResManQuick_impl_ResourceManager.h"

#include <queue>

namespace Dv
{
	namespace ResManQuick
	{
		/***************************************************************************************
		* TODO:
		* 1. Enumerators:
		* 1.1. Implement resource type enumerator
		* 1.2. GroupEnumerator
		* 1.3. RoleEnumerator
		* 2. Interface
		* 2.0. Common:
		* 2.0.1. Replace all enumerator type definitions by removing pointer from in the T templ argument in the resource manager interface.
		* (+DONE: 291.12.2016/14:21)
		* 2.1. Resources:
		* 2.1.1. Register the resource
		* 2.1.2. Unregister the resource
		* 2.1.2.1. Q. How to get the resource iterator from the given resource?
		* 2.1.3. Pop failed resource:
		* 2.1.3.1. Q. If resource was remove?
		* 2.2. Groups:
		* 2.2.1. The same questions as for the resource
		***************************************************************************************/

		class ResourceManager : public ResMan::IResourceManager
		{
		public:
			~ResourceManager                                                                            ();

			/**********************************************************************************
			* Working with resource types
			**********************************************************************************/
			ResourceManager                                                                             (Impl::Impl_ResourceManager *pInImpl);

			/**********************************************************************************
			* GETTING SUBSYSTEM
			**********************************************************************************/
			virtual ResMan::IResourceManagerSubsystem   *GetSubsystem                                    () const override;

			/**********************************************************************************
			* Working with resource types
			**********************************************************************************/
			virtual ResourceTypeEnumerator               GetResourceTypes                                ()                                        override;

			/**********************************************************************************
			* Registering resources
			**********************************************************************************/
			virtual ResMan::IManagedResource            *RegisterResource                                (const ResMan::PublicResourceParams& InParams, std::shared_ptr<ResMan::IResourceLoader> pInLoader) override;
			virtual void                                 UnregisterResource                              (ResMan::IManagedResource *pInResource) override;

			/**********************************************************************************
			* Enumerating resources
			**********************************************************************************/
			virtual ResourceEnumerator                   GetResourceEnumerator                           () override;
			virtual ResourceEnumerator                   GetResourceWithURL                              () override;

			/**********************************************************************************
			* Find resource by URL
			**********************************************************************************/
			virtual ResMan::IManagedResource            *FindResourceByURL                               (const std::string& InURL) override;

			/**********************************************************************************
			* Registering groups
			**********************************************************************************/
			virtual ResMan::IManagedGroup               *RegisterGroup                                   (const ResMan::GroupParams& InParams) override;
			virtual void                                 UnregisterGroup                                 (ResMan::IManagedGroup *pInGroup) override;

			/**********************************************************************************
			* Registering groups
			**********************************************************************************/
			virtual GroupEnumerator                      GetGroups                                       ()                                       override             { return GetImpl()->GetEnumerator_Groups(); };

			/**********************************************************************************
			* Registering groups
			**********************************************************************************/
			virtual ResMan::IManagedResource            *PopFailedResource                               () override;
			virtual ResMan::IManagedGroup               *PopFailedGroup                                  () override;

			/**********************************************************************************
			* Working with subresource roles
			**********************************************************************************/
			virtual RoleEnumerator                       GetSubresourceRoles                             () override;
			virtual ResMan::ISubresourceRole            *GetSubresourceRoleByIndex                       (ResMan::SubresourceRoleIndex InIndex) override;

			/**********************************************************************************
			* Working with implementation
			**********************************************************************************/
			Impl::Impl_ResourceManager                  *GetImpl                                         () const { return _pImpl; }

			/**********************************************************************************
			* Waiting for events
			**********************************************************************************/
			// Wait until all commands running on all loading loops processed.
			virtual void                                Wait_UntilAllCommandsProcessed                                   () override;
			// Wait until all commands for the given resource are processed;
			// If bInRecurisve is true, then child commands are also waited for to complete.
			virtual void                                Wait_UntilAllResourceCommandsProcessed                           (ResMan::IManagedResource *pInResource, bool bInRecurisve) override;
			// Wait until all commands for the given group are processed;
			// If bInRecurisve is true, then child commands are also waited for to complete.
			virtual void                                Wait_UntilAllGroupCommandsProcessed                              (ResMan::IManagedGroup *pInGroup, bool bInRecurisve) override;

			// Wait until all commands that run NOT on the main thread have been processed.
			// WARNING!!! The non-threaded child commands will not be waited for to complete.
			virtual void                                Wait_UntilAllThreadedCommandsProcessed                           () override;
			// Wait until all commands connected with the given resource that run NOT on the main thread have been processed.
			// WARNING!!! The non-threaded child commands will not be waited for to complete.
			virtual void                                Wait_UntilAllThreadedCommandsForResourceProcessed                (ResMan::IManagedResource *pInResource, bool bInRecursive) override;

		private:
			Impl::Impl_ResourceManager                  *_pImpl;
		};
	}
}