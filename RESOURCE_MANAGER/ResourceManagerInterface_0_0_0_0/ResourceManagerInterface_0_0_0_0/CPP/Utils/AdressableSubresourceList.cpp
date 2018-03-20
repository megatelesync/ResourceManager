#include "../../H/Utils/AddressableSubresourceList.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			size_t AddressableSubresourceList::GetNumAddressableSubresources(SubresourceRoleIndex InRoleIndex) const
			{
				auto pAdressableSubresourceList = _GetSubresourceRecordList_ForRole(InRoleIndex);
				if (nullptr == pAdressableSubresourceList)
				{
					return 0;
				}
				else
				{
					return pAdressableSubresourceList->size();
				}
			}
			const AddressableSubresource *AddressableSubresourceList::GetSubresource(SubresourceRoleIndex InSubresourceRoleIndex, AddressableSubresourceIndex InIndex) const
			{
				auto pSubresource = _GetAddressableSubresourceRecord(InSubresourceRoleIndex, InIndex);
				return pSubresource;
			}
			void AddressableSubresourceList::LinkAddressableSubresourceTo(SubresourceRoleIndex InSubresourceRoleIndex, AddressableSubresourceIndex InIndex, IManagedResource *pInResource)
			{
				auto pSubresource = _GetAddressableSubresourceRecord(InSubresourceRoleIndex, InIndex);
				pSubresource->pResource = pInResource;
			}
			SubresourceRoleSet AddressableSubresourceList::GetAddressableSubresourceRoles() const
			{
				return _addressableSubresourceRoles;
			}
			void AddressableSubresourceList::Register(SubresourceRoleIndex InRoleIndex, const LoadableAddressableSubresourceParams *pInParams)
			{
				AssertAddressableSubresourceParamsCorrect(pInParams);
				auto pAddressableSubresourceList_ForRole = _GetSubresourceRecordList_ForRole_ExistingOrNew(InRoleIndex);
				auto pExistingRecord_WithTheSameURL = _FindSubresourceByURL(pAddressableSubresourceList_ForRole, pInParams->GetURL());
				// Record with the same URL may not exist
				assert(nullptr == pExistingRecord_WithTheSameURL);
				pAddressableSubresourceList_ForRole->push_back(AddressableSubresource(pInParams));
			}

			void AddressableSubresourceList::BeginRegistration(SubresourceRoleIndex InRoleIndex)
			{
				auto pList = _GetSubresourceRecordList_ForRole_ExistingOrNew(InRoleIndex);
				pList->clear();
			}
			void AddressableSubresourceList::EndRegistration(SubresourceRoleIndex InRoleIndex)
			{
			}

			const AddressableSubresource  *AddressableSubresourceList::_FindSubresourceByURL(const AddressableSubresourceList::SubresourceRecordList *pInSubresourceList, const std::string& InURL) const
			{
				for (int subresourceIndex = 0; subresourceIndex < pInSubresourceList->size(); subresourceIndex++)
				{
					auto pSubresourceRecord = &(pInSubresourceList->at(subresourceIndex));
					const std::string currentSubresourceURL = pSubresourceRecord->params.GetURL();
					if (currentSubresourceURL == InURL)
					{
						return pSubresourceRecord;
					}
				}
				return nullptr;
			}

			const AddressableSubresource *AddressableSubresourceList::_GetAddressableSubresourceRecord(SubresourceRoleIndex InRoleIndex, AddressableSubresourceIndex InIndex) const
			{
				auto pAddressableSubresourceList = _GetSubresourceRecordList_ForRole(InRoleIndex);
				auto pAddressableSubresource = &(pAddressableSubresourceList->at(InIndex));
				return pAddressableSubresource;
			}
			AddressableSubresource *AddressableSubresourceList::_GetAddressableSubresourceRecord(SubresourceRoleIndex InRoleIndex, AddressableSubresourceIndex InIndex)
			{
				auto pAddressableSubresourceList = _GetSubresourceRecordList_ForRole(InRoleIndex);
				auto pAddressableSubresource = &(pAddressableSubresourceList->at(InIndex));
				return pAddressableSubresource;
			}

			AddressableSubresourceList::SubresourceRecordList *AddressableSubresourceList::_GetSubresourceRecordList_ForRole_ExistingOrNew(SubresourceRoleIndex InRoleIndex)
			{
				auto it_new_list = _subresources.find(InRoleIndex);
				if (_subresources.end() == it_new_list)
				{
					_addressableSubresourceRoles.Include(InRoleIndex);
					auto it_inserted_list = _subresources.insert(_subresources.end(), std::make_pair(InRoleIndex, SubresourceRecordList()));
					return &(it_inserted_list->second);
				}
				else
				{
					return &(it_new_list->second);
				}
			}

			const AddressableSubresourceList::SubresourceRecordList *AddressableSubresourceList::_GetSubresourceRecordList_ForRole(SubresourceRoleIndex InRoleIndex) const
			{
				if (InRoleIndex < _subresources.size())
				{
					auto pAdressableSubresourceList = &(_subresources.at(InRoleIndex));
					return pAdressableSubresourceList;
				}
				else
				{
					return nullptr;
				}
			}
			AddressableSubresourceList::SubresourceRecordList *AddressableSubresourceList::_GetSubresourceRecordList_ForRole(SubresourceRoleIndex InRoleIndex)
			{
				if (InRoleIndex < _subresources.size())
				{
					auto pAdressableSubresourceList = &(_subresources.at(InRoleIndex));
					return pAdressableSubresourceList;
				}
				else
				{
					return nullptr;
				}
			}

			AddressableSubresource::AddressableSubresource(const LoadableAddressableSubresourceParams *pInParams) :
				params(*pInParams)
			{
			}

			void CopyNotLoadedAddressableSubresources(AddressableSubresourceList *pDestList, AddressableSubresourceList *pSrc, SubresourceRoleSet InSubresourceRoleSet)
			{
				auto roleEnumerator = InSubresourceRoleSet.GetEnumerator();
				while (true)
				{
					if (roleEnumerator.IsEnd())
					{
						break;
					}
					auto roleIndex = roleEnumerator.GetRoleIndex();
					if (false == pDestList->GetAddressableSubresourceRoles().IsSet(roleIndex))
					{
						CopyAddressableRoleSubresources(pDestList, pSrc, roleIndex);
						roleEnumerator.MoveNext();
					}
					roleEnumerator.MoveNext();
				}
			}

			void CopyAddressableRoleSubresources(AddressableSubresourceList *pDestList, AddressableSubresourceList *pSrc, SubresourceRoleIndex InRoleIndex, bool bInCallBeginEnd)
			{
				assert(pDestList);
				assert(pSrc);
				if(bInCallBeginEnd)
				{
					pDestList->BeginRegistration(InRoleIndex);
				}
				auto numSubresourcesOfRole = pSrc->GetNumAddressableSubresources(InRoleIndex);
				for (auto subresourceIndex = 0; subresourceIndex < numSubresourcesOfRole; subresourceIndex++)
				{					
					auto pSrcSubresource = pSrc->GetSubresource(InRoleIndex, subresourceIndex);
					pDestList->Register(InRoleIndex, &(pSrcSubresource->params));
				}
				if(bInCallBeginEnd)
				{
					pDestList->EndRegistration(InRoleIndex);
				}
			}
		}
	}
}