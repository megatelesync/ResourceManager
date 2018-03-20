#pragma once

#include "../Override/IResourceLoader.h"

#include <list>


namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			struct AddressableSubresource
			{
				LoadableAddressableSubresourceParams                          params;
				IManagedResource                                             *pResource = nullptr;

				AddressableSubresource(const LoadableAddressableSubresourceParams *pInParams);
			};

			class AddressableSubresourceList
			{
			public:
				/*****************************************************************************************************************************
				* Typedefs
				****************************************************************************************************************************/
				using SubresourceRecordList = std::vector<AddressableSubresource>;
				using SubresourceCont = std::map<SubresourceRoleIndex,SubresourceRecordList>;

				/*****************************************************************************************************************************
				* Interface for getting subresources
				****************************************************************************************************************************/
				size_t                                             GetNumAddressableSubresources                     (SubresourceRoleIndex InRoleIndex) const;
				const AddressableSubresource                      *GetSubresource                                    (SubresourceRoleIndex InSubresourceRoleIndex, AddressableSubresourceIndex InIndex) const;

				/*****************************************************************************************************************************
				* Interface for loading and getting what loaded
				*****************************************************************************************************************************/
				void                                               LinkAddressableSubresourceTo                      (SubresourceRoleIndex InSubresourceRoleIndex, AddressableSubresourceIndex InIndex, IManagedResource *pInResource);
				SubresourceRoleSet                                 GetAddressableSubresourceRoles                     () const;


				/*****************************************************************************************************************************
				* Interface for registering list elements
				*****************************************************************************************************************************/
				// Beginning to registering roles (clears list of subresources for corresponding role)
				void                                               BeginRegistration                                 (SubresourceRoleIndex InRoleIndex);
				void                                               EndRegistration                                   (SubresourceRoleIndex InRoleIndex);
				void                                               Register                                          (SubresourceRoleIndex InRoleIndex, const LoadableAddressableSubresourceParams *pInParams);

			private:
				// Find record of the addressable subresource and assert that addressable subresource with corresponding URL does NOT exist
				const AddressableSubresource                      *_FindSubresourceByURL (const SubresourceRecordList *pInSubresourceList, const std::string& InURL) const;

				const AddressableSubresource                      *_GetAddressableSubresourceRecord(SubresourceRoleIndex InRoleIndex, AddressableSubresourceIndex InIndex) const;
				AddressableSubresource                            *_GetAddressableSubresourceRecord(SubresourceRoleIndex InRoleIndex, AddressableSubresourceIndex InIndex);

				const SubresourceRecordList                       *_GetSubresourceRecordList_ForRole(SubresourceRoleIndex InRoleIndex) const;
				SubresourceRecordList                             *_GetSubresourceRecordList_ForRole(SubresourceRoleIndex InRoleIndex);

				SubresourceRecordList                             *_GetSubresourceRecordList_ForRole_ExistingOrNew(SubresourceRoleIndex InRoleIndex);

				SubresourceCont                                    _subresources;
				SubresourceRoleSet                                 _addressableSubresourceRoles;
			};
			// Copy addressable subreosurces of the given role from one list to another
			void CopyAddressableRoleSubresources(AddressableSubresourceList *pDestList, AddressableSubresourceList *pSrc, SubresourceRoleIndex InRoleIndex, bool bInCallBeginEnd = true);
			// Copy all addressable subresources for that roles for which no addressable subresources are loaded yet.
			void CopyNotLoadedAddressableSubresources(AddressableSubresourceList *pDestList, AddressableSubresourceList *pSrc, SubresourceRoleSet InSubresourceRoleSet);
		}
	}
}