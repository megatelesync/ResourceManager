#include "../../H/Utils/ISubresourceRole.h"

#include <cassert>

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{	
			/********************************************************************************************************************
			* ISubresourceRole utils
			********************************************************************************************************************/
			std::string ToString_SubresourceRole(ISubresourceRole *pInRole)
			{
				assert(pInRole != nullptr);
				std::string result;
				result += "{";
				result += std::to_string(static_cast<int>(pInRole->GetRoleIndex()));
				result += ";";
				result += pInRole->GetParams().GetName();
				result += "}";
				return result;
			}

			void Log_SubresourceRolePublicParams(Util::ILoggingDevice *pInLogger, const SubresourceRolePublicParams *pInParams)
			{
				assert(pInLogger != nullptr);
				*pInLogger << "Name: " << pInParams->GetName() << Util::endl;
				*pInLogger << "BasePriority: " << pInParams->GetBasePriority() << Util::endl;
			}


			void Log_SubresourceRole(Util::ILoggingDevice *pInLogger, const ISubresourceRole *pInRole)
			{
				assert(pInLogger != nullptr);
				*pInLogger << "RoleIndex: " << pInRole->GetRoleIndex() << Util::endl;
				Log_SubresourceRolePublicParams(pInLogger, &(pInRole->GetParams()));
			}

			/********************************************************************************************************************
			* SubresourceRoleSet
			********************************************************************************************************************/
			SubresourceRoleSet::SubresourceRoleSet() : _roles(0)
			{
			}

			SubresourceRoleSet::SubresourceRoleSet(std::initializer_list<SubresourceRoleIndex> InInitializerList) : SubresourceRoleSet()
			{
				for (const auto& roleIndex : InInitializerList)
				{
					Include(roleIndex);
				}
			}

			bool SubresourceRoleSet::IsSet(const SubresourceRoleIndex InIndex) const
			{
				return _roles[InIndex];
			}
			bool SubresourceRoleSet::IsSet(const ISubresourceRole *pInRole) const
			{
				assert(pInRole != nullptr);
				return _roles[pInRole->GetRoleIndex()];
			}
			void SubresourceRoleSet::SetEnableState(const SubresourceRoleIndex InIndex, bool bInValue)
			{
				_roles.set(InIndex, bInValue);
			}
			void SubresourceRoleSet::SetEnableState(const ISubresourceRole *pInRole, bool bInValue)
			{
				assert(pInRole != nullptr);
				_roles.set(pInRole->GetRoleIndex(), bInValue);
			}
			void SubresourceRoleSet::Include(const ISubresourceRole *pInRole)
			{
				SetEnableState(pInRole, true);
			}
			void SubresourceRoleSet::Include(const SubresourceRoleIndex InIndex)
			{
				SetEnableState(InIndex, true);
			}
			void SubresourceRoleSet::Exclude(const SubresourceRoleIndex InIndex)
			{
				SetEnableState(InIndex, false);
			}
			void SubresourceRoleSet::Exclude(const ISubresourceRole *pInRole)
			{
				SetEnableState(pInRole, false);
			}
			void SubresourceRoleSet::Clear()
			{
				_roles.reset();
			}
			void SubresourceRoleSet::MergeWith(SubresourceRoleSet InSubresources)
			{
				_roles |= InSubresources._roles;
			}
			void SubresourceRoleSet::IntersectWith(SubresourceRoleSet InSubresources)
			{
				_roles &= InSubresources._roles;
			}
			void SubresourceRoleSet::Subtract(SubresourceRoleSet InSubresources)
			{
				_roles &= ~InSubresources._roles;
			}

			SubresourceRoleEnumerator SubresourceRoleSet::GetEnumerator() const 
			{ 
				return SubresourceRoleEnumerator(this);
			}

			bool operator==(SubresourceRoleSet lhs, SubresourceRoleSet rhs)
			{
				return  lhs._roles == rhs._roles;
			}
			bool operator!=(SubresourceRoleSet lhs, SubresourceRoleSet rhs)
			{
				return false == (operator==(lhs, rhs));
			}

			/*******************************************************************************************************
			* SubresourceRoleSetEnumerator implementation
			********************************************************************************************************/
			SubresourceRoleEnumerator::SubresourceRoleEnumerator(const SubresourceRoleSet *pInRoles) :
				_roles(*pInRoles),
				_index(0)
			{
				auto bSet = _roles.IsSet(SubresourceRoleIndex(_index));
				if (!bSet)
				{
					MoveNext();
				}
			}

			boost::optional<SubresourceRoleIndex> SubresourceRoleEnumerator::MoveNext()
			{
				SubresourceRoleIndex typedIndex(_index);
				while(true)
				{
					++_index;
					if (IsEnd())
					{
						break;
					}
					typedIndex = SubresourceRoleIndex(_index);
					auto bSet = _roles.IsSet(typedIndex);
					if (bSet)
					{
						return SubresourceRoleIndex(typedIndex);
					}
				}
			}
		}
	}
}