#pragma once

#include "../H//D3D11ShaderResource.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/********************************************************************************************************************
			* implementation utils
			********************************************************************************************************************/
			d3d_utils::HResultValue<ID3D11VertexShader *>        CreateShaderObject_Vertex       (ID3D11Device *pInDev, ID3D10Blob *pInBytecodeBlob, ID3D11ClassLinkage *pInClassLinkage);
			d3d_utils::HResultValue<ID3D11PixelShader *>         CreateShaderObject_Pixel        (ID3D11Device *pInDev, ID3D10Blob *pInBytecodeBlob, ID3D11ClassLinkage *pInClassLinkage);
			d3d_utils::HResultValue<ID3D11ComputeShader *>       CreateShaderObject_Compute      (ID3D11Device *pInDev, ID3D10Blob *pInBytecodeBlob, ID3D11ClassLinkage *pInClassLinkage);
			d3d_utils::HResultValue<ID3D11GeometryShader *>      CreateShaderObject_Geometry     (ID3D11Device *pInDev, ID3D10Blob *pInBytecodeBlob, ID3D11ClassLinkage *pInClassLinkage);
			d3d_utils::HResultValue<ID3D11HullShader *>          CreateShaderObject_Hull         (ID3D11Device *pInDev, ID3D10Blob *pInBytecodeBlob, ID3D11ClassLinkage *pInClassLinkage);
			d3d_utils::HResultValue<ID3D11DomainShader *>        CreateShaderObject_Domain       (ID3D11Device *pInDev, ID3D10Blob *pInBytecodeBlob, ID3D11ClassLinkage *pInClassLinkage);

			/********************************************************************************************************************
			* D3D11VertexShaderLoaderFromFile implementation
			********************************************************************************************************************/
			D3D11VertexShaderLoaderFromFile::D3D11VertexShaderLoaderFromFile
			(
				const std::string                      &InFilename,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) : D3D11ShaderLoaderFromFile(InFilename, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{

			}

			d3d_utils::HResultValue<ID3D11VertexShader *> D3D11VertexShaderLoaderFromFile::_CreateShaderObject
			(
				ID3D10Blob                                      *pInBytecodeBlob,				
				ID3D11ClassLinkage                              *pInClassLinkage
			)
			{
				return CreateShaderObject_Vertex(_GetDev(), pInBytecodeBlob, pInClassLinkage);
			}

			/********************************************************************************************************************
			* D3D11PixelShaderLoaderFromFile implementation
			********************************************************************************************************************/
			D3D11PixelShaderLoaderFromFile::D3D11PixelShaderLoaderFromFile
			(
				const std::string                      &InFilename,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) : D3D11ShaderLoaderFromFile(InFilename, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{

			}

			d3d_utils::HResultValue<ID3D11PixelShader *> D3D11PixelShaderLoaderFromFile::_CreateShaderObject
			(
				ID3D10Blob                                      *pInBytecodeBlob,
				ID3D11ClassLinkage                              *pInClassLinkage
			)
			{
				return CreateShaderObject_Pixel(_GetDev(), pInBytecodeBlob, pInClassLinkage);
			}

			/********************************************************************************************************************
			* D3D11ComputeShaderLoaderFromFile implementation
			********************************************************************************************************************/
			D3D11ComputeShaderLoaderFromFile::D3D11ComputeShaderLoaderFromFile
			(
				const std::string                      &InFilename,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) : D3D11ShaderLoaderFromFile(InFilename, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{

			}

			d3d_utils::HResultValue<ID3D11ComputeShader *> D3D11ComputeShaderLoaderFromFile::_CreateShaderObject
			(
				ID3D10Blob                                      *pInBytecodeBlob,
				ID3D11ClassLinkage                              *pInClassLinkage
			)
			{
				return CreateShaderObject_Compute(_GetDev(), pInBytecodeBlob, pInClassLinkage);
			}

			/********************************************************************************************************************
			* D3D11GeometryShaderLoaderFromFile implementation
			********************************************************************************************************************/
			D3D11GeometryShaderLoaderFromFile::D3D11GeometryShaderLoaderFromFile
			(
				const std::string                      &InFilename,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) : D3D11ShaderLoaderFromFile(InFilename, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{

			}

			d3d_utils::HResultValue<ID3D11GeometryShader *> D3D11GeometryShaderLoaderFromFile::_CreateShaderObject
			(
				ID3D10Blob                                      *pInBytecodeBlob,
				ID3D11ClassLinkage                              *pInClassLinkage
			)
			{
				return CreateShaderObject_Geometry(_GetDev(), pInBytecodeBlob, pInClassLinkage);
			}

			/********************************************************************************************************************
			* D3D11HullShaderLoaderFromFile implementation
			********************************************************************************************************************/
			D3D11HullShaderLoaderFromFile::D3D11HullShaderLoaderFromFile
			(
				const std::string                      &InFilename,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) : D3D11ShaderLoaderFromFile(InFilename, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{

			}

			d3d_utils::HResultValue<ID3D11HullShader *> D3D11HullShaderLoaderFromFile::_CreateShaderObject
			(
				ID3D10Blob                                      *pInBytecodeBlob,
				ID3D11ClassLinkage                              *pInClassLinkage
			)
			{
				return CreateShaderObject_Hull(_GetDev(), pInBytecodeBlob, pInClassLinkage);
			}

			/********************************************************************************************************************
			* D3D11DomainShaderLoaderFromFile implementation
			********************************************************************************************************************/
			D3D11DomainShaderLoaderFromFile::D3D11DomainShaderLoaderFromFile
			(
				const std::string                      &InFilename,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) : D3D11ShaderLoaderFromFile(InFilename, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{

			}

			d3d_utils::HResultValue<ID3D11DomainShader *> D3D11DomainShaderLoaderFromFile::_CreateShaderObject
			(
				ID3D10Blob                                      *pInBytecodeBlob,
				ID3D11ClassLinkage                              *pInClassLinkage
			)
			{
				return CreateShaderObject_Domain(_GetDev(), pInBytecodeBlob, pInClassLinkage);
			}

			/********************************************************************************************************************
			* D3D11VertexShaderLoaderFromMemory implementation
			********************************************************************************************************************/
			D3D11VertexShaderLoaderFromMemory::D3D11VertexShaderLoaderFromMemory
			(
				const std::vector<std::string>         *pInSourceCode,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) :
				D3D11ShaderLoaderFromMemory(pInSourceCode, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{
			}

			d3d_utils::HResultValue<ID3D11VertexShader *> D3D11VertexShaderLoaderFromMemory::_CreateShaderObject
			(
				ID3D10Blob                                      *pInBytecodeBlob,
				ID3D11ClassLinkage                              *pInClassLinkage
			)
			{
				return CreateShaderObject_Vertex(_GetDev(), pInBytecodeBlob, pInClassLinkage);
			}

			/********************************************************************************************************************
			* D3D11PixelShaderLoaderFromMemory implementation
			********************************************************************************************************************/
			D3D11PixelShaderLoaderFromMemory::D3D11PixelShaderLoaderFromMemory
			(
				const std::vector<std::string>         *pInSourceCode,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) :
				D3D11ShaderLoaderFromMemory(pInSourceCode, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{
			}

			d3d_utils::HResultValue<ID3D11PixelShader *> D3D11PixelShaderLoaderFromMemory::_CreateShaderObject
			(
				ID3D10Blob                                      *pInBytecodeBlob,
				ID3D11ClassLinkage                              *pInClassLinkage
			)
			{
				return CreateShaderObject_Pixel(_GetDev(), pInBytecodeBlob, pInClassLinkage);
			}

			/********************************************************************************************************************
			* D3D11ComputeShaderLoaderFromMemory implementation
			********************************************************************************************************************/
			D3D11ComputeShaderLoaderFromMemory::D3D11ComputeShaderLoaderFromMemory
			(
				const std::vector<std::string>         *pInSourceCode,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) :
				D3D11ShaderLoaderFromMemory(pInSourceCode, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{
			}

			d3d_utils::HResultValue<ID3D11ComputeShader *> D3D11ComputeShaderLoaderFromMemory::_CreateShaderObject
			(
				ID3D10Blob                                      *pInBytecodeBlob,
				ID3D11ClassLinkage                              *pInClassLinkage
			)
			{
				return CreateShaderObject_Compute(_GetDev(), pInBytecodeBlob, pInClassLinkage);
			}

			/********************************************************************************************************************
			* D3D11GeometryShaderLoaderFromMemory implementation
			********************************************************************************************************************/
			D3D11GeometryShaderLoaderFromMemory::D3D11GeometryShaderLoaderFromMemory
			(
				const std::vector<std::string>         *pInSourceCode,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) :
				D3D11ShaderLoaderFromMemory(pInSourceCode, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{
			}

			d3d_utils::HResultValue<ID3D11GeometryShader *> D3D11GeometryShaderLoaderFromMemory::_CreateShaderObject
			(
				ID3D10Blob                                      *pInBytecodeBlob,
				ID3D11ClassLinkage                              *pInClassLinkage
			)
			{
				return CreateShaderObject_Geometry(_GetDev(), pInBytecodeBlob, pInClassLinkage);
			}

			/********************************************************************************************************************
			* D3D11HullShaderLoaderFromMemory implementation
			********************************************************************************************************************/
			D3D11HullShaderLoaderFromMemory::D3D11HullShaderLoaderFromMemory
			(
				const std::vector<std::string>         *pInSourceCode,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) :
				D3D11ShaderLoaderFromMemory(pInSourceCode, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{
			}

			d3d_utils::HResultValue<ID3D11HullShader *> D3D11HullShaderLoaderFromMemory::_CreateShaderObject
			(
				ID3D10Blob                                      *pInBytecodeBlob,
				ID3D11ClassLinkage                              *pInClassLinkage
			)
			{
				return CreateShaderObject_Hull(_GetDev(), pInBytecodeBlob, pInClassLinkage);
			}

			/********************************************************************************************************************
			* D3D11DomainShaderLoaderFromMemory implementation
			********************************************************************************************************************/
			D3D11DomainShaderLoaderFromMemory::D3D11DomainShaderLoaderFromMemory
			(
				const std::vector<std::string>         *pInSourceCode,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) :
				D3D11ShaderLoaderFromMemory(pInSourceCode, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{
			}

			d3d_utils::HResultValue<ID3D11DomainShader *> D3D11DomainShaderLoaderFromMemory::_CreateShaderObject
			(
				ID3D10Blob                                      *pInBytecodeBlob,
				ID3D11ClassLinkage                              *pInClassLinkage
			)
			{
				return CreateShaderObject_Domain(_GetDev(), pInBytecodeBlob, pInClassLinkage);
			}

			/********************************************************************************************************************
			* implementation utils
			********************************************************************************************************************/
			d3d_utils::HResultValue<ID3D11VertexShader *> CreateShaderObject_Vertex(ID3D11Device *pInDev, ID3D10Blob *pInBytecodeBlob, ID3D11ClassLinkage *pInClassLinkage)
			{				
				ID3D11VertexShader *pResShader = nullptr;
				HRESULT hr = pInDev->CreateVertexShader(pInBytecodeBlob->GetBufferPointer(), pInBytecodeBlob->GetBufferSize(), pInClassLinkage, &pResShader);
				return d3d_utils::HResultValue<ID3D11VertexShader *>(hr, pResShader);
			}

			d3d_utils::HResultValue<ID3D11PixelShader *>  CreateShaderObject_Pixel(ID3D11Device *pInDev, ID3D10Blob *pInBytecodeBlob, ID3D11ClassLinkage *pInClassLinkage)
			{
				ID3D11PixelShader *pResShader = nullptr;
				HRESULT hr = pInDev->CreatePixelShader(pInBytecodeBlob->GetBufferPointer(), pInBytecodeBlob->GetBufferSize(), pInClassLinkage, &pResShader);
				return d3d_utils::HResultValue<ID3D11PixelShader *>(hr, pResShader);
			}

			d3d_utils::HResultValue<ID3D11ComputeShader *>  CreateShaderObject_Compute(ID3D11Device *pInDev, ID3D10Blob *pInBytecodeBlob, ID3D11ClassLinkage *pInClassLinkage)
			{
				ID3D11ComputeShader *pResShader = nullptr;
				HRESULT hr = pInDev->CreateComputeShader(pInBytecodeBlob->GetBufferPointer(), pInBytecodeBlob->GetBufferSize(), pInClassLinkage, &pResShader);
				return d3d_utils::HResultValue<ID3D11ComputeShader *>(hr, pResShader);
			}

			d3d_utils::HResultValue<ID3D11GeometryShader *>  CreateShaderObject_Geometry(ID3D11Device *pInDev, ID3D10Blob *pInBytecodeBlob, ID3D11ClassLinkage *pInClassLinkage)
			{
				ID3D11GeometryShader *pResShader = nullptr;
				HRESULT hr = pInDev->CreateGeometryShader(pInBytecodeBlob->GetBufferPointer(), pInBytecodeBlob->GetBufferSize(), pInClassLinkage, &pResShader);
				return d3d_utils::HResultValue<ID3D11GeometryShader *>(hr, pResShader);
			}

			d3d_utils::HResultValue<ID3D11HullShader *>  CreateShaderObject_Hull(ID3D11Device *pInDev, ID3D10Blob *pInBytecodeBlob, ID3D11ClassLinkage *pInClassLinkage)
			{
				ID3D11HullShader *pResShader = nullptr;
				HRESULT hr = pInDev->CreateHullShader(pInBytecodeBlob->GetBufferPointer(), pInBytecodeBlob->GetBufferSize(), pInClassLinkage, &pResShader);
				return d3d_utils::HResultValue<ID3D11HullShader *>(hr, pResShader);
			}

			d3d_utils::HResultValue<ID3D11DomainShader *>  CreateShaderObject_Domain(ID3D11Device *pInDev, ID3D10Blob *pInBytecodeBlob, ID3D11ClassLinkage *pInClassLinkage)
			{
				ID3D11DomainShader *pResShader = nullptr;
				HRESULT hr = pInDev->CreateDomainShader(pInBytecodeBlob->GetBufferPointer(), pInBytecodeBlob->GetBufferSize(), pInClassLinkage, &pResShader);
				return d3d_utils::HResultValue<ID3D11DomainShader *>(hr, pResShader);
			}
		}
	}
}