#pragma once

/**************************************************************************************************
* Interface of the resource manager for the client of the resource manager.
* 
* Automatically includes:
* - resource manager "overridable" interface;
* - resource manager "Utils" interface;
**************************************************************************************************/

#include "../Utils//MASTER_ResourceManager_Utils.h"
#include "../Override/MASTER_ResourceManager_Override.h"

#include "LoadCommand.h"
#include "CommandState.h"
#include "IResourceType.h"
#include "IManagedGroup.h"
#include "IManagedResource.h"
#include "IResourceManager.h"