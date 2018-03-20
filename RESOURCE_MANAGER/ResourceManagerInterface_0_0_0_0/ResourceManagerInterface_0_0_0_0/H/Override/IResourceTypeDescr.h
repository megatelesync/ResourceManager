#pragma once

#include "../Utils/ISubresourceRole.h"

#include "Log/Util_ILoggingDevice.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			class IResourceTypeDescr;
			class IResourceManagerSubsystem;
			class IResourceManager;

			/***********************************************************************************************
			* Subresource role sets associated with the given resource type
			***********************************************************************************************/
			class ResourceTypeSubresourceRoleParams
			{
			public:
				ResourceTypeSubresourceRoleParams() = default;

				// Subresourec roles that can be used with the given resource
				SubresourceRoleSet                                  GetApplicableSubresourceRoles                       () const              { return _applicableSubresourceRoles; }

				// Returns subresources of which roles must be loaded to be able 
				// to create this resource from scratch (i.e. without having an already-created resource)
				// (for example, to create d3d11 buffer resource, the RAM data must be loaded;
				// however, if the D3D11 buffer object is already created, and we can get it's content,
				// no RAM data is necessary)
				SubresourceRoleSet                                  GetRolesThatMustBeLoadedToCreateResource            () const              { return _mustBeLoadedToCreateResource; }

				// Subresource roles that must be loaded while real resource is loaded.
				// When one of this subresource is unloaded, the real object gets destroyed.
				SubresourceRoleSet                                  GetRolesThatMustBeLoadedWhileRealResourceIsLoaded   () const              { return _mustBeLoadedWhileRealResourceIsLoaded; }
				// Subresource role that impersonates the real resource itself
				ISubresourceRole                                   *GetRealResourceSubresourceRole                      () const              { return _pRealResourceSubresourceRole; }

				void                                                SetRealResourceSubresourceRole                      (ISubresourceRole *pInRole) { _pRealResourceSubresourceRole = pInRole; }
				void                                                IncludeApplicable                                   (ISubresourceRole *pInRole);
				void                                                IncludeIn_MustBeLoadedToCreateResource              (ISubresourceRole *pInRole);
				void                                                IncludeIn_MustBeLoadedWhileRealResourceIsLoaded     (ISubresourceRole *pInRole);

			private:
				SubresourceRoleSet                                 _applicableSubresourceRoles;
				SubresourceRoleSet                                 _mustBeLoadedToCreateResource;
				SubresourceRoleSet                                 _mustBeLoadedWhileRealResourceIsLoaded;
				ISubresourceRole                                  *_pRealResourceSubresourceRole = nullptr;
			};

			void Log_ResourceTypeSubresourceRoleParams            (Util::ILoggingDevice *pInLogger, const ResourceTypeSubresourceRoleParams *pInParams, IResourceManager *pInResourceManager);

			/***********************************************************************************************
			* Parameters to be exposed by the resource manager interface
			*
			* WARNING!!! Must be exposed for const access only! Otherwise subresource role params may be damanged.
			***********************************************************************************************/
			class ResourceTypePublicParams
			{
			public:
				/******************************************************************************************
				* GETTERS
				******************************************************************************************/
				// User-friendly name of the resource
				const std::string&                                 GetName                          () const;

				// URL of the resource to be used by default for the current resource type
				// (to be set to empty value for resource type that does not have default resource)
				const std::string&                                 GetDefaultURL                    () const;

				// Name of the subdirectory for files of resources of the given file;
				// (if empty, no subdirectory is provided for this resource)
				const std::string&                                 GetSubdirectoryName              () const { return _subdirectoryName; }

				const ResourceTypeSubresourceRoleParams&           GetSubresourceRoles              () const { return _subresourceRoles; }

				/******************************************************************************************
				* SETTERS (FOR INITIALIZATION)
				******************************************************************************************/				
				void                                               SetName                          (const std::string& InValue);
				void                                               SetSubdirectoryName              (const std::string& InValue);
				void                                               SetDefaultURL                    (const std::string& InValue);

				// Provide access to subresource roles parameters for initialization 
				// (provided for IResourceTypeDescr::RegisterApplicableRoles(...))
				ResourceTypeSubresourceRoleParams                  *GetSubresourceRolesPtr          () { return &_subresourceRoles; }

			private:
				std::string                                        _subdirectoryName = "";
				std::string                                        _name;
				std::string                                        _defaultURL;
				ResourceTypeSubresourceRoleParams                  _subresourceRoles;
			};

			void Log_RegisteredResourceType_PublicParams(Util::ILoggingDevice *pInLogger, const ResourceTypePublicParams *pInParams, IResourceManager *pInResourceManager);


			/***********************************************************************************************
			* Description of the resource type.
			*
			* Subclass must be defined for each of the resource types to be managed.
			*
			* Subclasses of this class must be default-constructble (required for the typed resource type).
			***********************************************************************************************/
			class IResourceTypeDescr
			{
			public:				
				/********************************************************************************************
				* SPECIAL FUNCTIONS
				********************************************************************************************/
				virtual                                        ~IResourceTypeDescr       ()                                            = default;

				IResourceTypeDescr                                                       (const IResourceTypeDescr&)                   = delete;
				IResourceTypeDescr&                            operator=                 (const IResourceTypeDescr&)                   = delete;

				/********************************************************************************************
				* DEFAULT CTOR
				********************************************************************************************/
				IResourceTypeDescr                                                       () = default;

				/********************************************************************************************
				* Register applicable roles (using the given manager subsystem interface)
				* Typically the registered roles to be stored in an object
				* that is accesible by the getter from the concrete IResourceTypeDescr subclass.
				********************************************************************************************/
				virtual void                                   RegisterApplicableRoles   (IResourceManagerSubsystem *pInSubsys)        =0;

				/********************************************************************************************
				* GETTING PARAMETERS
				********************************************************************************************/
				virtual const ResourceTypePublicParams&        GetParams                 () const                                      =0;
				
				/********************************************************************************************
				* Delete by the given raw ptr
				*********************************************************************************************/
				virtual void                                   Delete                    (void *pInResource)                           =0;				
			};
			void Log_ResourceTypeDescr(Util::ILoggingDevice *pInLogger, const IResourceTypeDescr *pInDescr, IResourceManager *pInResourceManager);

			/************************************************************************************************
			* ResourceTypePublicParams IMPLEMENTATION
			************************************************************************************************/
			inline const std::string& ResourceTypePublicParams::GetName() const
			{
				return _name;
			}
			inline const std::string& ResourceTypePublicParams::GetDefaultURL() const
			{
				return _defaultURL;
			}
			inline void ResourceTypePublicParams::SetName(const std::string& InValue)
			{
				assert(&InValue != nullptr);
				_name = InValue;
			}
			inline void ResourceTypePublicParams::SetSubdirectoryName(const std::string& InValue)
			{
				assert(&InValue != nullptr);
				_subdirectoryName = InValue;
			}
			inline void ResourceTypePublicParams::SetDefaultURL(const std::string& InValue)
			{
				assert(&InValue != nullptr);
				_defaultURL = InValue;
			}

			/***********************************************************************************************
			* ResourceTypeSubresourceRoleParams implementation
			***********************************************************************************************/
			inline void ResourceTypeSubresourceRoleParams::IncludeApplicable(ISubresourceRole *pInRole)
			{
				_applicableSubresourceRoles.Include(pInRole);
			}
			inline void ResourceTypeSubresourceRoleParams::IncludeIn_MustBeLoadedToCreateResource(ISubresourceRole *pInRole)
			{
				_mustBeLoadedToCreateResource.Include(pInRole);
			}
			inline void ResourceTypeSubresourceRoleParams::IncludeIn_MustBeLoadedWhileRealResourceIsLoaded(ISubresourceRole *pInRole)
			{
				_mustBeLoadedWhileRealResourceIsLoaded.Include(pInRole);
			}
		} // Ver_0_0_0_0
	} // ResourceManager
} // Dv
