#pragma once

#include "ResManQuick_Fwd.h"
#include "Subsystem/IResourceManagerSubsystem.h"
#include "../PrivateH/ResManQuick_impl_ResourceManagerSubsystem.h"
#include "ResManQuick_ResourceManager.h"
#include "../PrivateH/Log/ResManQuick_impl_Log.h"

namespace Dv
{
	namespace ResManQuick
	{
		/*************************************************************************************************************
		* TODO:
		* 1. Implement resource storage:
		* 1.1. Implement Impl_ManagedResource resource accounting (+DONE: 32.12.2016/19:01)
		* 1.2. Implement the ResourceContainer (+DONE: 32.12.2016/19:01)
		* 1.3. Implement the Managed resource
		*
		* 2. Implement group storage
		* 2.1. Implement Impl_ManagedGroup
		* 2.2. Implement RoledGroupResources
		* 2.3. Implement ResourceGroupCombiCont
		*
		* 3. Implement loading
		*
		* TODO questions:
		* 1. Where to store the resource type - ResourceManagerSubsystem or ResourceManager?
		* 2. Where to store the resource roles - ResourceManagerSubsystem or ResourceManager?
		*************************************************************************************************************/
		class ResourceManagerSubsystem : public ResMan::IResourceManagerSubsystem
		{
		public:
			~ResourceManagerSubsystem();

			/*********************************************************************************************************
			* CONSTRUCTION
			*********************************************************************************************************/
			ResourceManagerSubsystem                                                        (const ResManQuick_SubsystemParams& InParams);

			/*************************************************************************************************
			* GETTING PARAMETERS
			*************************************************************************************************/
			const ResManQuick_SubsystemParams                      *GetParams               () const { return &(GetImpl()->GetParams()); }
			virtual const ResMan::ResourceManagerSubsystemParams   *GetCommonParams         () const override { return &(GetParams()->GetCommonSubsystemParams()); }

			/*********************************************************************************************************
			* INITIALIZATION
			*********************************************************************************************************/
			virtual void                               InitializeAndStart                   () throw(ResMan::ResourceManagerException)     override;
			virtual void                               Stop                                 ()                                             override;

			/*********************************************************************************************************
			* Getting the main interface object
			*********************************************************************************************************/
			virtual ResMan::IResourceManager          *GetManager                           () override                      { return  _subsystem.GetManager()->GetInterface();  }

			/*********************************************************************************************************
			* Ticking
			*********************************************************************************************************/
			// Tick - delta seconds is the number of simulation (NOT realtime) seconds since the last frame
			virtual void                               Tick                                 (double InDeltaSeconds) override;
			virtual TickIndex                          GetTickIndex                         () const override;
			virtual double                             GetSimulationTime                    () const override;

			/*********************************************************************************************************
			* Controlling execution
			*********************************************************************************************************/
			virtual const ResMan::ResourceManagerPublicRuntimeParams&        GetRuntimeParams          () const override { return _subsystem.GetRuntimeParams(); }
			// Update the runtime params with the new value
			virtual void                                                     UpdateRuntimeParams       (const ResMan::ResourceManagerPublicRuntimeParams& InParams) override;

			/********************************************************************************************
			* All resource types to be registered before calling the initialization function.
			* The registered resource type DESCR will NOT be deleted at the time of the subsystem destruction.
			*********************************************************************************************/
			virtual ResMan::IResourceType                                   *RegisterResourceType       (ResMan::IResourceTypeDescr *pInType) override;

			/****************************************************************************************
			* Working with roles
			****************************************************************************************/
			virtual ResMan::ISubresourceRole                                *RegisterRole               (const ResMan::SubresourceRolePublicParams& InParams) override;

			/****************************************************************************************
			* Working with implementation
			****************************************************************************************/
			const Impl::Impl_ResourceManagerSubsystem                       *GetImpl                    () const { return &_subsystem; }
			Impl::Impl_ResourceManagerSubsystem                             *GetImpl                    () { return &_subsystem; }

			/****************************************************************************************
			* logging 
			****************************************************************************************/
			// Get logger that is to be worked with ONLY FROM THE MAIN THREAD
			Impl::Logger                                                    *GetLogger_MainThread       () { return GetImpl()->GetLogger_MainThread(); }

		private:			
			Impl::Impl_ResourceManagerSubsystem                              _subsystem;
			bool                                                             _bInitialized = false;
		};

		/*********************************************************************************************************************
		* ResourceManagerSubsystem IMPLEMENTATION
		*********************************************************************************************************************/
		inline ResourceManagerSubsystem::TickIndex ResourceManagerSubsystem::GetTickIndex() const
		{
			return _subsystem.GetTickIndex();
		}
		inline double ResourceManagerSubsystem::GetSimulationTime() const
		{
			return _subsystem.GetSimulationTime();
		}
	}
}