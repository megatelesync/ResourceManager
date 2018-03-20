#pragma once

#include "../Core/ResourceManagerTypes.h"
#include "Log/Util_ILoggingDevice.h"

#include "Enumerator/Util_IEnumerator.h"

#include <boost/optional.hpp>
#include <string>
#include <bitset>

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			class IResourceType;
			class IManagedResource;

			// Maximal number of subresource roles allowed to be registered
			const size_t MAX_SUBRESOURCE_ROLES = 32;

			/********************************************************************************
			* Public parameters of a subresource role
			********************************************************************************/
			class SubresourceRolePublicParams
			{
			public:
				SubresourceRolePublicParams                             ();
				SubresourceRolePublicParams                             (const std::string& InName);

				const std::string&                  GetName             ()                                     const;
				void                                SetName             (const std::string &InNewName);

				ResourcePriority                    GetBasePriority     ()                                     const;
				void                                SetBasePriority     (ResourcePriority InBasePriority);

			private:
				std::string                         _name;
				ResourcePriority                    _basePriority;
			};
			void Log_SubresourceRolePublicParams(Util::ILoggingDevice *pInLogger, const SubresourceRolePublicParams *pInParams);

			/****************************************************************************************
			* Type-safe subresource role index class
			*
			* This class is necessary to be able to work with subresource roles
			* without knowning concrete subresource role classes
			****************************************************************************************/
			class SubresourceRoleIndex
			{
			public:
				/***********************************************************************************
				* TYPEDEFS
				************************************************************************************/
				using RoleIndex = uint8_t;

				// explicit is intended
				explicit SubresourceRoleIndex             (RoleIndex InSubresourceRoleIndex)
					: _subresourceRoleIndex               (InSubresourceRoleIndex)                               {}

				operator RoleIndex                        () const                                               { return _subresourceRoleIndex;  }

			private:
				RoleIndex                                 _subresourceRoleIndex;
			};

			/****************************************************************************************
			* Role of the subresource
			*
			* This class is always to be passed by reference
			****************************************************************************************/
			class ISubresourceRole
			{
			public:
				/***********************************************************************************
				* TYPEDEFS
				************************************************************************************/
				using RoleIndex                =         SubresourceRoleIndex;

				using ResourceEnumerator       =         std::unique_ptr<Util::IEnumerator<IManagedResource>>;
				using ResourceURL              =         std::string;

				/***********************************************************************************
				* CONSTRUCTION
				************************************************************************************/
				ISubresourceRole                                                                         () = default;

				/***********************************************************************************
				* SPECIAL OPERATIONS
				************************************************************************************/
				virtual ~ISubresourceRole                                                                ()                               = default;

				// Forbid copying and as it's reference type
				ISubresourceRole                                                                         (const ISubresourceRole&)        = delete;
				ISubresourceRole&                                     operator=                          (const ISubresourceRole&)        = delete;

				/***********************************************************************************
				* GETTERS
				************************************************************************************/				
				virtual	const SubresourceRolePublicParams&            GetParams                          () const                         =0;
				virtual	RoleIndex                                     GetRoleIndex                       () const                         =0;

				/***********************************************************************************
				* RESOURCE ENUMERATION
				************************************************************************************/
				// Enumerate each resource which main subsresource is of this role
				virtual ResourceEnumerator                            GetResources                       ()                               =0;
				// Search resource with the given URL within resources which main subsresource is of this role
				// (returns nullptr if resource with the given URL is not found)
				virtual IManagedResource                             *GetResourceByURL                   (const ResourceURL& InURL)       =0;
			};
			
			std::string                                               ToString_SubresourceRole           (ISubresourceRole *pInRole);

			void                                                      Log_SubresourceRole                (Util::ILoggingDevice *pInLogger, const ISubresourceRole *pInRole);

			/*************************************************************************************************
			* Set of subresource roles.
			*
			* Enumeration:
			*    Enumerator is provided for enumerating all included roles. 
			*    Maybe it's ineffecient for maximal roles count more than 32.
			*
			* Warning!!! This class must be passed by value.
			*************************************************************************************************/
			class SubresourceRoleEnumerator; //Fwd declaration
			class SubresourceRoleSet 
			{
			public:
				friend bool                                    operator==                     (SubresourceRoleSet lhs, SubresourceRoleSet rhs);
				friend bool                                    operator!=                     (SubresourceRoleSet lhs, SubresourceRoleSet rhs);

				/*********************************************************************************************
				* Types
				**********************************************************************************************/
				using BitSet         = std::bitset<MAX_SUBRESOURCE_ROLES>;

				/*********************************************************************************************
				* SPECIAL FUNCTIONS
				**********************************************************************************************/
				// Copying is allowed
				SubresourceRoleSet&                 operator=                       (const SubresourceRoleSet& InRoleSet) = default;
				SubresourceRoleSet                                                  (const SubresourceRoleSet& InRoleSet) = default;
				// Moving is allowed
				SubresourceRoleSet&                 operator=                       (SubresourceRoleSet&& InRoleSet) = default;
				SubresourceRoleSet                                                  (SubresourceRoleSet&& InRoleSet) = default;

				/*********************************************************************************************
				* CONSTRUCTION AND INITIALIZATION
				**********************************************************************************************/
				// default ctor: initializes role set where all roles are resetted to false
				SubresourceRoleSet();
				SubresourceRoleSet(std::initializer_list<SubresourceRoleIndex> InInitializerList);

				/*********************************************************************************************
				* Enumerating all included roles
				**********************************************************************************************/
				SubresourceRoleEnumerator        GetEnumerator                     () const;

				/*********************************************************************************************
				* Getting role state
				**********************************************************************************************/
				bool                                IsSet                           (const SubresourceRoleIndex InIndex) const;
				bool                                IsSet                           (const ISubresourceRole *pInRole) const;

				/*********************************************************************************************
				* Set roles
				**********************************************************************************************/
				void                                SetEnableState                  (const SubresourceRoleIndex InIndex, bool bInValue);
				void                                SetEnableState                  (const ISubresourceRole *pInRole, bool bInValue);
				void                                Include                         (const SubresourceRoleIndex InIndex);
				void                                Include                         (const ISubresourceRole *pInRole);
				void                                Exclude                         (const SubresourceRoleIndex InIndex);
				void                                Exclude                         (const ISubresourceRole *pInRole);

				/*********************************************************************************************
				* Operations
				**********************************************************************************************/
				void                                Clear                           ();

				// Merge with the given subresource set
				void                                MergeWith                       (SubresourceRoleSet InSubresources);
				void                                IntersectWith                   (SubresourceRoleSet InSubresources);
				// subtract the given set of roles from this set
				void                                Subtract                        (SubresourceRoleSet InSubresources);


				/*********************************************************************************************
				* Counting
				**********************************************************************************************/
				// Count of included roles
				size_t                              CountIncluded                   () const;
				bool                                IsEmpty                         () const;

				/*********************************************************************************************
				* Inclusion tests
				**********************************************************************************************/
				bool                                Contains                       (SubresourceRoleSet InOther) const;

			private:
				BitSet                              _roles;
			};
			bool                                    operator==                     (SubresourceRoleSet lhs, SubresourceRoleSet rhs);
			bool                                    operator!=                     (SubresourceRoleSet lhs, SubresourceRoleSet rhs);

			/******************************************************************************************************
			* This enumerator enumerates all included roles
			*******************************************************************************************************/
			class SubresourceRoleEnumerator
			{
			public:
				SubresourceRoleEnumerator                                                            (const SubresourceRoleSet *pInRoles);
				boost::optional<SubresourceRoleIndex>                 MoveNext                       ();

				bool                                                  IsEnd                          () const { return _index >= MAX_SUBRESOURCE_ROLES; }
				SubresourceRoleIndex                                  GetRoleIndex                   () const { return SubresourceRoleIndex(_index);  }

			private:
				SubresourceRoleSet                                    _roles;
				size_t                                                _index;
			};

			/*******************************************************************************************************
			* SubresourceRoleSet implementation
			********************************************************************************************************/
			inline size_t SubresourceRoleSet::CountIncluded() const
			{
				return _roles.count();
			}
			inline bool SubresourceRoleSet::IsEmpty() const
			{
				return _roles.none();
			}
			inline bool SubresourceRoleSet::Contains(SubresourceRoleSet InOther) const
			{
			  auto result = ~_roles & InOther._roles;
			  return result.none();
			}

			/****************************************************************************************
			* Subresource role public params implementation
			****************************************************************************************/
			inline SubresourceRolePublicParams::SubresourceRolePublicParams() :
				_name("{UNNAMED}"),
				_basePriority(0)
			{
			}

			inline SubresourceRolePublicParams::SubresourceRolePublicParams(const std::string& InName) :
				_name(InName),
				_basePriority(0)
			{
			}

			inline const std::string& SubresourceRolePublicParams::GetName() const
			{
				return _name;
			}

			inline void SubresourceRolePublicParams::SetName(const std::string &InNewName)
			{
				_name = InNewName;
			}

			inline ResourcePriority SubresourceRolePublicParams::GetBasePriority() const
			{
				return _basePriority;
			}
			inline void SubresourceRolePublicParams::SetBasePriority(ResourcePriority InBasePriority)
			{
				_basePriority = InBasePriority;
			}
		} // Ver_0_0_0_0
	} // ResourceManager
} // Dv