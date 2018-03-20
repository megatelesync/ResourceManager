#pragma once

#include "../../H/Override/IResourceTypeDescr.h"
#include "../../H/Utils/ISubresourceRole.h"

#include "Enumerator/Util_IEnumerator.h"

#include "Log/Util_ILoggingDevice.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			class IResourceType;

			/********************************************************************************************
			* Typed resource type
			* 
			* Grants typed access to the manager resource type interface.
			* The resource type description is owned by the class.
			*
			* TEMPLATE ARGUMENTS:
			* 1. Resource descriptor type class (ResourceTypeDescrArg) 
			* - represents the concrete resource type descr subclass
			********************************************************************************************/
			template<class ResourceTypeDescrArg> class TypedResourceType
			{
			public:
				/****************************************************************************************
				* TYPEDEFS
				****************************************************************************************/
				using ResourceTypeDescr = ResourceTypeDescrArg;

				/****************************************************************************************
				* CONSTRUCTION
				****************************************************************************************/
				TypedResourceType();

				/****************************************************************************************
				* Registration
				****************************************************************************************/
				// Initialize with the given registered resource type
				void                                              Initialize(IResourceType *pInResourceType);

				/****************************************************************************************
				* GETTERS
				****************************************************************************************/
				// Get the registered resource
				const IResourceType                               *GetResourceType() const;
				IResourceType                                     *GetResourceType();
				// Getting concrete descriptor of the resource type
				// (for const access only, because we do NOT need to use non-const members)
				const ResourceTypeDescr                           *GetDescr() const;
				ResourceTypeDescr                                 *GetDescr();
				const typename ResourceTypeDescr::RoleSetType     &GetRoles() const                           { return GetDescr()->GetRoles(); }

				/****************************************************************************************
				* PARAMETER GETTERS
				****************************************************************************************/
				const ResourceTypePublicParams&                   GetParams                                   () const;
				SubresourceRoleSet                         GetApplicableSubresources                   () const;
				SubresourceRoleSet                         GetWhatMustBeLoaded_ToCreateResource        () const;

			private:
				IResourceType                                    *_pResourceType;
				std::unique_ptr<ResourceTypeDescr>                _pResourceTypeDescr;
			};

			/********************************************************************************************
			* Resource type that is already registered in the manager
			********************************************************************************************/
			class IResourceType
			{
			public:
				/****************************************************************************************
				* TYPEDEFS
				****************************************************************************************/
				using ResourceTypeIndex      = uint8_t;

				/****************************************************************************************
				* CONSTRUCTION
				****************************************************************************************/
				IResourceType                                                                       ()                                               = default;

				/****************************************************************************************
				* SPECIAL TYPES
				****************************************************************************************/
				virtual                                            ~IResourceType                   ()                                               = default;

				IResourceType                                                                       (const IResourceType&)                           = delete;
				IResourceType&                                      operator=                       (const IResourceType&)                           = delete;

				/****************************************************************************************
				* Parameters
				****************************************************************************************/
				virtual const ResourceTypePublicParams&             GetParams                        () const                                         =0;
				virtual IResourceTypeDescr                         *GetDescr                         () const                                         =0;
				 
				// Index of this resource types indices are unical and sequential withing the resource type
				virtual ResourceTypeIndex                           GetIndex                         () const                                         =0;
			};
			const ResourceTypeSubresourceRoleParams GetSubresourceRoleParams                   (IResourceType *pResourceType);
			SubresourceRoleSet GetApplicableSubresourceRoles                                   (IResourceType *pResourceType);
			SubresourceRoleSet GetSubresourceRoles_ThatMustBeLoadedToCreateResource            (IResourceType *pResourceType);

			void Log_RegisteredResourceType(Util::ILoggingDevice *pInLogger, const IResourceType *pInRegisteredResourceType, IResourceManager *pInResourceManager);

			/************************************************************************************************
			* IResourceType utils implementation
			************************************************************************************************/
			inline const ResourceTypeSubresourceRoleParams GetSubresourceRoleParams(IResourceType *pResourceType)
			{
				return pResourceType->GetParams().GetSubresourceRoles();
			}
			inline SubresourceRoleSet GetApplicableSubresourceRoles(IResourceType *pResourceType)
			{
				return GetSubresourceRoleParams(pResourceType).GetApplicableSubresourceRoles();
			}
			inline SubresourceRoleSet GetSubresourceRoles_ThatMustBeLoadedToCreateResource(IResourceType *pResourceType)
			{
				return GetSubresourceRoleParams(pResourceType).GetRolesThatMustBeLoadedToCreateResource();
			}

			/************************************************************************************************
			* TypedResourceType implementation
			************************************************************************************************/
			template<class ResourceTypeDescr>
			TypedResourceType<ResourceTypeDescr>::TypedResourceType() :
				_pResourceType(nullptr)
			{
				static_assert(std::is_default_constructible<ResourceTypeDescr>::value, "ResourceTypeDescr must be copy-constructible");
				static_assert(std::is_base_of<IResourceTypeDescr, ResourceTypeDescr>::value, "ResourceTypeDescr must be subclass of IResourceTypeDescr");

				_pResourceTypeDescr.reset(new ResourceTypeDescr());
			}

			template<class ResourceTypeDescr>
			void TypedResourceType<ResourceTypeDescr>::Initialize(IResourceType *pInResourceType)
			{
				assert(pInResourceType != nullptr);
				_pResourceType = pInResourceType;
			}

			template<class ResourceTypeDescrArg>
			const typename IResourceType *TypedResourceType<ResourceTypeDescrArg>::GetResourceType() const
			{
				return _pResourceType.get();
			}

			template<class ResourceTypeDescrArg>
			typename IResourceType *TypedResourceType<ResourceTypeDescrArg>::GetResourceType()
			{
				return _pResourceType;
			}

			template<class ResourceTypeDescrArg>
			const typename TypedResourceType<ResourceTypeDescrArg>::ResourceTypeDescr *TypedResourceType<ResourceTypeDescrArg>::GetDescr() const
			{
				return _pResourceTypeDescr.get();
			}

			template<class ResourceTypeDescrArg>
			typename TypedResourceType<ResourceTypeDescrArg>::ResourceTypeDescr *TypedResourceType<ResourceTypeDescrArg>::GetDescr()
			{
				return _pResourceTypeDescr.get();
			}

			template<class ResourceTypeDescrArg>
			const ResourceTypePublicParams& TypedResourceType<ResourceTypeDescrArg>::GetParams() const
			{
				return _pResourceType->GetParams();
			}

			template<class ResourceTypeDescrArg>
			SubresourceRoleSet TypedResourceType<ResourceTypeDescrArg>::GetApplicableSubresources() const
			{
				return _pResourceType->GetParams().GetSubresourceRoles().GetApplicableSubresourceRoles();
			}

			template<class ResourceTypeDescrArg>
			SubresourceRoleSet TypedResourceType<ResourceTypeDescrArg>::GetWhatMustBeLoaded_ToCreateResource() const
			{
				return _pResourceType->GetWhatMustBeLoaded_ToCreateResource();
			}
		} // Ver_0_0_0_0
	} // ResourceManager
} // Dv
