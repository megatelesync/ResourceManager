#pragma once

/*****************************************************************************************************
* This file is to be included everywhere in the quick resource manager implementation
*****************************************************************************************************/

#include "Interface/MASTER_ResourceManager_Interface.h"
#include "CoreUtil/CoreUtil_Assert.h"
#include "Log/Util_ILoggingDevice.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace ResMan = Dv::ResourceManager::Ver_0_0_0_0;

		/**************************************************************************************************
		* ResourceType enumeration
		**************************************************************************************************/
		template<class ContType>
		class ContResourceTypeEnumerator : public Util::UniquePtrContEnumerator<ContType, ResMan::IResourceType>
		{
		public:
			explicit ContResourceTypeEnumerator(ContType *pInCont);
		};		

		template<class EnumeratorType>
		std::unique_ptr<Util::IEnumerator<ResMan::IResourceType>> MakeEnumeratorResult_ResourceType(EnumeratorType *pEnumerator);

		/**************************************************************************************************
		* SubresourceRole enumeration
		**************************************************************************************************/
		template<class ContType>
		class ContSubresourceRoleEnumerator : public Util::UniquePtrContEnumerator<ContType, ResMan::ISubresourceRole>
		{
		public:
			explicit ContSubresourceRoleEnumerator(ContType *pInCont);
		};

		// Make group enumerator result for the given enumerator
		template<class EnumeratorType>
		std::unique_ptr<Util::IEnumerator<ResMan::ISubresourceRole>> MakeEnumeratorResult_SubresourceRole(EnumeratorType *pEnumerator);

		/***********************************************************************************************************
		* Resource enumeration
		************************************************************************************************************/
		// Enumerating resources from the given unique_ptr container
		template<class ContType>
		class ContResourceEnumerator : public Util::UniquePtrContEnumerator<ContType, ResMan::IManagedResource>
		{
		public:
			explicit ContResourceEnumerator(ContType *pInCont);
		};

		// Containers of C++ ptrs to groups
		template<class ContType>
		class PtrContResourceEnumerator : public Util::PtrContEnumerator<ContType, ResMan::IManagedResource>
		{
		public:
			explicit PtrContResourceEnumerator(ContType *pInCont);
		};

		template<class ContType>
		class MapValueResourceEnumerator : public Util::MapContValueEnumerator<ContType, ResMan::IManagedResource>
		{
		public:
			explicit MapValueResourceEnumerator(ContType *pInCont);
		};

		template<class EnumeratorType>
		std::unique_ptr<Util::IEnumerator<ResMan::IManagedResource>> MakeResourceEnumeratorResult(EnumeratorType *pEnumerator);


		// Make group enumerator result for the given enumerator
		template<class EnumeratorType>
		std::unique_ptr<Util::IEnumerator<ResMan::IManagedGroup>> MakeGroupEnumeratorResult(EnumeratorType *pEnumerator);

		/**************************************************************************************************
		* Group enumeration
		**************************************************************************************************/
		// Enumerating groups from the given unique_ptr container
		template<class ContType>
		class ContGroupEnumerator : public Util::UniquePtrContEnumerator<ContType, ResMan::IManagedGroup>
		{
		public:
			explicit ContGroupEnumerator(ContType *pInCont);
		};

		// Containers of C++ ptrs to groups
		template<class ContType>
		class PtrContGroupEnumerator : public Util::PtrContEnumerator<ContType, ResMan::IManagedGroup>
		{
		public:
			explicit PtrContGroupEnumerator(ContType *pInCont);
		};

		/************************************************************************************************************
		* ResourceType enumerator implementation
		************************************************************************************************************/
		template<class ContType>
		ContResourceTypeEnumerator<ContType>::ContResourceTypeEnumerator(ContType *pInCont) :
			Util::UniquePtrContEnumerator<ContType, ResMan::IResourceType>(pInCont)
		{
		}

		template<class EnumeratorType>
		std::unique_ptr<Util::IEnumerator<ResMan::IResourceType>> MakeEnumeratorResult_ResourceType(EnumeratorType *pEnumerator)
		{
			return std::unique_ptr<Util::IEnumerator<ResMan::IResourceType>>(pEnumerator);
		}

		/**************************************************************************************************
		* SubresourceRole enumeration implementation
		**************************************************************************************************/
		template<class ContType>
		ContSubresourceRoleEnumerator<ContType>::ContSubresourceRoleEnumerator(ContType *pInCont) :
			Util::UniquePtrContEnumerator<ContType, ResMan::ISubresourceRole>(pInCont)
		{
		}

		template<class EnumeratorType>
		std::unique_ptr<Util::IEnumerator<ResMan::ISubresourceRole>> MakeEnumeratorResult_SubresourceRole(EnumeratorType *pEnumerator)
		{
			return std::unique_ptr<Util::IEnumerator<ResMan::ISubresourceRole>>(pEnumerator);
		}


		/************************************************************************************************************
		* group enumerator implementation
		************************************************************************************************************/
		template<class EnumeratorType>
		std::unique_ptr<Util::IEnumerator<ResMan::IManagedGroup>> MakeGroupEnumeratorResult(EnumeratorType *pEnumerator)
		{
			return std::unique_ptr<Util::IEnumerator<ResMan::IManagedGroup>>(pEnumerator);
		}

		/************************************************************************************************************
		* PtrContGroupEnumerator implementation
		************************************************************************************************************/
		template<class ContType>
		PtrContGroupEnumerator<ContType>::PtrContGroupEnumerator(ContType *pInCont) :
			Util::PtrContEnumerator<ContType, ResMan::IManagedGroup>(pInCont)
		{
		}

		/************************************************************************************************************
		* ContGroupEnumerator implementation
		************************************************************************************************************/
		template<class ContType>
		ContGroupEnumerator<ContType>::ContGroupEnumerator(ContType *pInCont) :
			Util::UniquePtrContEnumerator<ContType, ResMan::IManagedGroup>(pInCont)
		{
		}


		/************************************************************************************************************
		* ContResourceEnumerator implementation
		************************************************************************************************************/
		template<class ContType>
		ContResourceEnumerator<ContType>::ContResourceEnumerator(ContType *pInCont) : 
			Util::UniquePtrContEnumerator<ContType, ResMan::IManagedResource>(pInCont)
		{
		}

		template<class ContType>
		PtrContResourceEnumerator<ContType>::PtrContResourceEnumerator(ContType *pInCont) :
			Util::PtrContEnumerator<ContType, ResMan::IManagedResource>(pInCont)
		{
		}

		template<class ContType>
		MapValueResourceEnumerator<ContType>::MapValueResourceEnumerator(ContType *pInCont) :
			Util::MapContValueEnumerator<ContType, ResMan::IManagedResource>(pInCont)
		{
		}

		template<class EnumeratorType>
		std::unique_ptr<Util::IEnumerator<ResMan::IManagedResource>> MakeResourceEnumeratorResult(EnumeratorType *pEnumerator)
		{
			return std::unique_ptr<Util::IEnumerator<ResMan::IManagedResource>>(pEnumerator);
		}
	}
}