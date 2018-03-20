#pragma once

#include "ResManD3D11\ResManD3D11.h"

/***********************************************************************************
* DONE:
* 1. D3D11InputLayoutResource                  (+CODED: 26.12.2016/19:25);
* 2. D3D11BufferResource                       (+CODED: 26.12.2016/19:25);
* 3. D3D11BlendStateResource                   (+CODED: 26.12.2016/19:25);
* 4. D3D11DepthStencilStateResource            (+CODED: 26.12.2016/19:25);
* 5. D3D11SamplerStateResource                 (+CODED: 26.12.2016/19:39);
*
* TO BE DONE:
* 1. D3D11RasterizerStateResource
*
* TODO:
* 1. D3D11DepthStencilViewResource
* 2. D3D11ShaderPairResource
* 2.1. Create D3D11ShaderResource
* 2.1.1. Create TextResource
* 2.1.1.1. When opening file fails, the error must be returned
* 2.1.1.2. When reading current line failed, the error must be returned
* 2.2. D3D11ShaderLoaderBase
* 2.3. D3D11ShaderLoaderFromMemory
* 2.3.1. Class itself (+CODED 27.12.2016/13:40)
* 2.3.2. D3D11VertexShaderLoaderFromMemory (+CODED 27.12.2016/13:40)
* 2.3.3. D3D11PixelShaderLoaderFromMemory
* 2.4. D3D11ShaderLoaderFromFile
* 2.4.1. Class itself (+CODED: 27.12.2016/14:50)
* 2.4.1.1. Create the load strings from stream utility (+CODED: 27.12.2016/14:50)
* 2.4.1.2. D3D11VertexShaderLoaderFromFile (+CODED: 27.12.2016/15:02)
* 2.4.1.3. D3D11PixelShaderLoaderFromFile
* 2.5. D3D11Texture2DResource:
* 2.5.1. Mip-map autogeneration functionality
* 2.5.2. Structure default ctors
*
* TODO misc:
* 1. static_assert is_base_of
* 1.1. TypedD3D11ResTypeDescrBase               (+CODED: 26.12.2016/19:48)
* 1.2. TypedD3D11ResourceLoaderBase
* 1.2.1. Unable to specify restriction: is_base_of insists on specifying the template argument
* 2. Make functions for getting whether the given role should be loaded or not
* (_ShouldLoadTempContent(pRole), _ShouldTempContentBeUnloaded(pRole)) (+CODED: 26.12.2016/19:08)
*
* TODO ResMan interface:
* 1. Make the functions to return/output result code:
* 1.1. Loading resources                       (+CODED: 27.12.2016/18:32)
* 2. it makes sense to allow to create resources with assigning initial content,
* and not assigning loaders. Such resources should be unloaded unless the load command is stated explicitly.
* (resource example: text)
***********************************************************************************/

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{

		}
	}
}