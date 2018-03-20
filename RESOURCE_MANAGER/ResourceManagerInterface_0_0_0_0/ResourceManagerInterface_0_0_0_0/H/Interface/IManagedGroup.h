#pragma once

#include "CombinableParams.h"

#include "IManagedResource.h"

#include "Enumerator/Util_IEnumerator.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			class ISubresourceRole;

			/**************************************************************************************************
			* Parameters of the group to be exposed by the manager interface
			**************************************************************************************************/
			class GroupPublicParams
			{
			public:
				/*********************************************************************************************
				* CONSTRUCTION
				*********************************************************************************************/
				GroupPublicParams() :
					_name("UNNAMED_GROUP")
				{
				}

				/*********************************************************************************************
				* GETTERS
				*********************************************************************************************/
				const std::string&            GetName                                    ()        const;

				ResourcePriority              GetBaseLoadPriority                        ()        const;
				ResourcePriority              GetBaseUnloadPriority                      ()        const;

				const CombinableParams&      GetCombinable                              ()        const { return _combinableParams;  }

				/*********************************************************************************************
				* SETTERS (FOR INITIALIZATION)
				*********************************************************************************************/
				void                          SetName                                    (const std::string& InValue);
				void                          SetBaseLoadPriority                        (ResourcePriority InValue);
				void                          SetBaseUnloadPriority                      (ResourcePriority InValue);
				void                          SetCombinable                              (const CombinableParams& InValue) { _combinableParams = InValue; }
				CombinableParams             *GetCombinablePtr                           () { return &_combinableParams; }

			private:
				std::string                   _name;
				CombinableParams              _combinableParams;
			};

			class GroupParams
			{
			public:
				/*********************************************************************************************
				* GETTERS
				*********************************************************************************************/
				const GroupPublicParams      *GetPublic                                      ()       const                      { return &_pub; }

				const std::string&            GetName                                        ()        const;

				ResourcePriority              GetBaseLoadPriority                            ()        const;
				ResourcePriority              GetBaseUnloadPriority                          ()        const;

				/*********************************************************************************************
				* SETTERS (FOR INITIALIZATION)
				*********************************************************************************************/
				void                          SetName                                       (const std::string& InValue);
				void                          SetBaseLoadPriority                           (ResourcePriority InValue);
				void                          SetBaseUnloadPriority                         (ResourcePriority InValue);

				CombinableParams             *GetCombinablePtr                              ()                                    { return _pub.GetCombinablePtr(); }

			private:
				GroupPublicParams             _pub;
			};

			struct GroupDependencyState
			{
				GroupDependencyState() : bLoading(true) {}

				bool bLoading;
			};

			/**************************************************************************************************
			* Group of resources
			*
			* Groups resources with the same precache/unload rules.
			* Group examples:
			* 1. UI resources
			* 2. Visible resources
			* 3. Invisible precached resources
			**************************************************************************************************/
			class IManagedGroup
			{
			public:
				/*********************************************************************************************
				* TYPEDEFS
				*********************************************************************************************/
				using ResourceEnumerator         =           std::unique_ptr<Util::IEnumerator<IManagedResource>>;

				/*********************************************************************************************
				* CONSTRUCTION
				*********************************************************************************************/
				IManagedGroup                                                                          () = default;

				/*********************************************************************************************
				* SPECIAL FUNCTIONS
				*********************************************************************************************/
				virtual                                              ~IManagedGroup                    ()                                           = default;

				IManagedGroup                                                                          (const IManagedGroup&)                       = delete;
				IManagedGroup&                                        operator=                        (const IManagedGroup&)                       = delete;

				/*********************************************************************************************
				* GETTERS
				*********************************************************************************************/
				virtual IResourceManager                             *GetOwnerManager                  () const = 0;

				virtual const GroupPublicParams&                      GetParams                        () const                                     =0;

				/*********************************************************************************************
				* UPDATING PARAMS
				*********************************************************************************************/
				virtual const CombinableParams                       *GetCombinableParams              () const                                     =0;
				virtual void                                          UpdateCombinableParams           (const CombinableParams&)                    =0;
				
				/*********************************************************************************************
				* RESOURCE ENUMERATION
				*********************************************************************************************/
				// Get subresource roles for which resources are loaded
				virtual SubresourceRoleSet                            GetSubresourceRoles_PrecachedResources                () const =0;
				virtual SubresourceRoleSet                            GetSubresourceRoles_NotPrecachedResources             () const =0;
				virtual SubresourceRoleSet                            GetSUbresourceRoles_FailedPrecachedResources          () const =0;

				// enumerate only resources that are cached according to the group's precache settings
				// and only those of them that play the given role (the role is the role that is associated with the resource)
				virtual ResourceEnumerator                            GetPrecachedResources            (const ISubresourceRole *pInRole) const            =0;
				

				virtual ResourceEnumerator                            GetFailedPrecachedResources      (const ISubresourceRole *pInRole) const = 0;

				// enumerate only resources that are NOT precached appropriately
				// and only those of them that play the given role (the role is the role that is associated with the resource)
				virtual ResourceEnumerator                            GetNotPrecachedResources         (const ISubresourceRole *pInRole) const            =0;

				/*********************************************************************************************
				* RESOURCE REGISTRATION
				*********************************************************************************************/
				// Attaching resource: if the resource is already included in the group, will do nothing.
				virtual void                                           AttachResource                  (IManagedResource *pInResource)               =0;
				virtual void                                           DetachResourceFromGroup         (IManagedResource *pInResource)               =0;

				/********************************************************************************************
				* PRIORITY MANAGEMENT
				*********************************************************************************************/
				// Priority of the manager: 
				// group with the highest priority to be loaded first, unloaded last
				// group with the least priority to be unloaded first, loaded last
				virtual ResourcePriority                               GetPrecachePriority              () const                                     =0;
				virtual void                                           SetHighestPrecachePriority       ()                                           =0;

				/********************************************************************************************
				* PRECACHING
				********************************************************************************************/
				// Precache asynchronously according to the group settings (roles to precache etc).
				// Urgency is the parameter that controls how fast the precaching or unloading needs to be done				
				virtual void                                           Precache                         (Urgency InUrgency)                                     =0;
				// Precache the group synchronously (stop the main loop until the resource group is loaded).
				// Returns state of the resource group.
				virtual const GroupCommandState&                      PrecacheSynchro                  ()                                                      =0;

				// State of the last precaching/unloading command
				virtual const GroupCommandState&                       GetCommandState                  () const                                                =0;

				/********************************************************************************************
				* DEPENDENCY LIST
				********************************************************************************************/
				virtual void                                           PrecacheDependencyList           (Urgency InUrgency)                                     =0;
				virtual const GroupDependencyState&                    GetDependencyListState           () const                                                =0;
			};
			// Count of resources that are properly precached
			size_t GetCount_PrecachedResources                                                          (IManagedGroup *pInGroup, ISubresourceRole *pInRole);
			// Count of resources that are not precached (but not precached failed)
			size_t GetCount_NotPrecachedResources                                                       (IManagedGroup *pInGroup, ISubresourceRole *pInRole);
			size_t GetCount_PrecachedFailedReosurces                                                    (IManagedGroup *pInGroup, ISubresourceRole *pInRole);
			size_t GetCount_PrecachedOrFailedPrecached                                                  (IManagedGroup *pInGroup, ISubresourceRole *pInRole);			

			/**************************************************************************************************
			* GroupParams implementation
			**************************************************************************************************/
			inline const std::string& GroupParams::GetName() const
			{
				return _pub.GetName();
			}

			inline ResourcePriority GroupParams::GetBaseLoadPriority() const
			{
				return _pub.GetBaseLoadPriority();
			}

			inline ResourcePriority GroupParams::GetBaseUnloadPriority() const
			{
				return _pub.GetBaseUnloadPriority();
			}

			inline void GroupParams::SetName(const std::string& InValue)
			{
				_pub.SetName(InValue);
			}

			inline void GroupParams::SetBaseLoadPriority(ResourcePriority InValue)
			{
				_pub.SetBaseLoadPriority(InValue);
			}
			inline void GroupParams::SetBaseUnloadPriority(ResourcePriority InValue)
			{
				_pub.SetBaseUnloadPriority(InValue);
			}

			/**************************************************************************************************
			* GroupPublicParams implementation
			**************************************************************************************************/
			inline const std::string& GroupPublicParams::GetName() const
			{
				return _name;
			}		

			inline ResourcePriority GroupPublicParams::GetBaseLoadPriority() const
			{
				return _combinableParams.GetBaseLoadPriority();
			}

			inline ResourcePriority GroupPublicParams::GetBaseUnloadPriority() const
			{
				return _combinableParams.GetBaseUnloadPriority();
			}
			
			inline void GroupPublicParams::SetName(const std::string& InValue)
			{
				assert(&InValue != nullptr);
				_name = InValue;
			}

			inline void GroupPublicParams::SetBaseLoadPriority(ResourcePriority InValue)
			{
				_combinableParams.SetBaseLoadPriority(InValue);
			}
			inline void GroupPublicParams::SetBaseUnloadPriority(ResourcePriority InValue)
			{
				_combinableParams.SetBaseUnloadPriority(InValue);
			}
		}
	}
}
