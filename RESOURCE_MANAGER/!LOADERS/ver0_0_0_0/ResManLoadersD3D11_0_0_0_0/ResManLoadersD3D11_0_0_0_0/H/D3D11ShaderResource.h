#pragma once

#include "ResManLoadersD3D11_System.h"

#include "StringUtil/Util_StringUtil_Master.h"
#include "!UTILS/H/utils_com.h"

#include "d3dcompiler.h"

/****************************************************************************************************
* TODO:
* 1. Embed shader reflection support (+DONE: 12.07.2017)
* 1.1. Add var to TypedD3D11Shader
* 1.1.1. Add var (+DONE: 12.07.2017)
* 1.1.2. Add constructor argument (+DONE: 12.07.2017)
* 1.2. In Loader:
* 1.2.1. ErrorHandling (+DONE: 12.07.2017)
* 1.2.2. Create (+DONE: 12.07.2017)
* 1.2.3. Unloading (+DONE: 12.07.2017)
* 1.3. In resource type:
* 1.3.1. Create shader reflection subresource role (+DONE: 12.07.2017)
* 2. Embed dynamic shader linkage support (+DONE: 12.07.2017)
* 2.1.TypedD3D11Shader:
* 2.1.1 Add variable (+DONE: 12.07.2017)
* 2.1.2.Add constructor argument (+DONE: 12.07.2017)
* 2.2. Create class linkage (+DONE: 12.07.2017)
****************************************************************************************************/

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/**************************************************************************************************************
			* D3D11 Shader object:
			* Includes D3D11 shader object with class linkage object and some extra info;
			* Additionaly may store:
			* - shader code blob;
			* - error blob;
			* - source code;
			* - shader reflection;
			*
			* TEMPALTE ARGUMENTS:
			* 1. ShaderObjectType: type of the underlying D3D11 object (ID3D11VertexShader/ID3D11PixelShader etc.)
			**************************************************************************************************************/
			template<class ShaderObjectType>
			class TypedD3D11Shader
			{
			public:
				using ShaderObject = ShaderObjectType;

				using SourceCodePtr       = std::shared_ptr<std::vector<std::string>>;
				using BlobPtr             = std::shared_ptr<ID3D10Blob>;
				using ShaderObjectPtr     = std::shared_ptr<ShaderObjectType>;
				using ReflectionPtr       = std::shared_ptr<ID3D11ShaderReflection>;
				using ClassLinkagePtr     = std::shared_ptr<ID3D11ClassLinkage>;
				
				/*************************************************************************************************************
				* Constructon:
				* - Source code unique pointer will be moved and owned by this object;
				* - Blobs unique pointers will be moved and owned by this object;
				* - If you want not to move a particular blob or source code, pass unique_ptr resetted to nullptr
				*************************************************************************************************************/
				TypedD3D11Shader
				(
					ShaderObjectPtr                                  pInShader,
					ClassLinkagePtr                                  pInClassLinkage,
					BlobPtr                                          pInBytecodeBlob,
					BlobPtr                                          pInErrBlob,
					SourceCodePtr                                    pInSourceCode,
					ReflectionPtr                                    pInReflection
				);


				ID3D11ClassLinkage                    *GetClassLinkage       () const                              { return _pClassLinkage.get(); }
				ID3D11ShaderReflection                *GetReflection         () const                              { return _pReflection.get(); }

				ShaderObjectType                      *GetShader             () const                              { return _pShader.get(); }

				ID3D10Blob                            *GetBytecodeBlob       () const                              { return _pBytecodeBlob.get(); }
				SIZE_T                                 GetBytecodeBlobSize   () const                              { return _pBytecodeBlob->GetBufferSize(); }

				ID3D10Blob                            *GetErrBlob            () const                              { return _pErrorBlob.get(); }
				SIZE_T                                 GetErrBlobSize        () const                              { return _pErrorBlob->GetBufferSize(); }

				const std::vector<std::string>        *GetSourceCode         () const                              { return _pSourceCode.get(); }

			private:
				ReflectionPtr                                                     _pReflection;
				ClassLinkagePtr                                                   _pClassLinkage;
				ShaderObjectPtr                                                   _pShader;
				BlobPtr                                                           _pBytecodeBlob; // blob can be nullptr
				BlobPtr                                                           _pErrorBlob;
				SourceCodePtr                                                     _pSourceCode;				
			};

			/******************************************************************************************
			* Typed D3D11 shaders
			******************************************************************************************/
			using D3D11VertexShader           = TypedD3D11Shader<ID3D11VertexShader>;
			using D3D11PixelShader            = TypedD3D11Shader<ID3D11PixelShader>;
			using D3D11GeometryShader         = TypedD3D11Shader<ID3D11GeometryShader>;
			using D3D11HullShader             = TypedD3D11Shader<ID3D11HullShader>;
			using D3D11DomainShader           = TypedD3D11Shader<ID3D11DomainShader>;
			using D3D11ComputeShader          = TypedD3D11Shader<ID3D11ComputeShader>;

			/******************************************************************************************
			* Typed resource
			******************************************************************************************/
			template<class ShaderObjectType>
			using D3D11ShaderResource          = ResMan::TypedManagedResource<TypedD3D11Shader<ShaderObjectType>>;

			using D3D11VertexShaderResource    = D3D11ShaderResource<ID3D11VertexShader>;
			using D3D11PixelShaderResource     = D3D11ShaderResource<ID3D11PixelShader>;
			using D3D11ComputeShaderResource   = D3D11ShaderResource<ID3D11ComputeShader>;
			using D3D11GeometryShaderResource  = D3D11ShaderResource<ID3D11GeometryShader>;
			using D3D11HullShaderResource      = D3D11ShaderResource<ID3D11HullShader>;
			using D3D11DomainShaderResource    = D3D11ShaderResource<ID3D11DomainShader>;

			/******************************************************************************************
			* Input D3D11 shader set of subresource roles
			******************************************************************************************/
			struct ResTypeDescr_Shader_Roles : public ResTypeDescr_Roles_Base
			{
				// temporary reflection subresource role
				// (warning: bytecode blob must be loaded)
				ResMan::ISubresourceRole                    *pTempReflection;
				// temporary bytecode blob subresource role
				ResMan::ISubresourceRole                    *pTempBlob;
				// error messages blob
				ResMan::ISubresourceRole                    *pTempErrBlob;
				// temporary source code subresource role 
				// (must be loaded to be able to create the main D3D11 object)
				ResMan::ISubresourceRole                    *pTempSourceCode;

				// WARNING! Class linkage is always regarded as part of the shader, 
				// so there'no separate subresource to represent it;
			};

			/*****************************************************************************************
			* ResTypeDescr_Shader
			*
			* TEMPLATE ARGUMENTS:
			* 1. ShaderObjectType: type of the underlying D3D11 object (ID3D11VertexShader/ID3D11PixelShader etc.)
			*****************************************************************************************/
			template<class ShaderObjectType>
			class ResTypeDescr_Shader :
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_Shader_Roles, TypedD3D11Shader<ShaderObjectType>>
			{
			public:
				ResTypeDescr_Shader                                     ();

				virtual void       RegisterApplicableRoles              (ResMan::IResourceManagerSubsystem *pInSubsys)    override;
				virtual void       Delete                               (void *pInResource)                               override;

			private:
			};

			/**************************************************************************************************************
			* Typed shader resource type
			**************************************************************************************************************/
			template<class ShaderObjectType>
			class D3D11ShaderResourceType : public ResMan::TypedResourceType<ResTypeDescr_Shader<ShaderObjectType>>
			{
			public:
			};

			/************************************************************************************************************
			* Parameters of the compilation that to be passed to D3DCompile function
			*************************************************************************************************************/
			struct D3DCompile_Params
			{
				UINT                               Flags1;
				UINT                               Flags2;
				std::string                        entry_point;
				std::string                        compile_target;
				std::vector<D3D_SHADER_MACRO>      macros_defines;

				D3DCompile_Params() = default;
				D3DCompile_Params(const std::string& InEntryPoint, const std::string& InCompileTarget) :
					Flags1(0),
					Flags2(0),
					entry_point(InEntryPoint),
					compile_target(InCompileTarget)
				{
				}
			};

			/**************************************************************************************************************
			* Shader loader base class
			*
			* To load shader, source code always must be available - compilation from bytecode only is not supported.
			**************************************************************************************************************/			
			template<class ShaderObjectType>
			class D3D11ShaderLoaderBase : public TypedD3D11ResourceLoaderBase<D3D11ShaderResourceType<ShaderObjectType>>
			{
			public:
				// Use this constructor for shaders that have no URL
				D3D11ShaderLoaderBase
				(
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);
				// Use this constructor for shaders that have URL
				D3D11ShaderLoaderBase
				(
					const std::string&                      InURL,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

				/************************************************************************************************************************
				* Create the shader object;
				* calls the virtual function to create the shader object
				*************************************************************************************************************************/
				virtual RealResourceType                   *Typed_CreateResourceAndGet                   (RealResourceType *pOldPtrOrNull) override;

				/************************************************************************************************************************
				* Load and unload functions are overloaded and final.
				* The overloads load necessary subresources, according to state of the roles argument:
				* 1) load source code (using the provided virtual functions)
				* 2) compile it to bytecode
				* Compilation is NOT performed in the loading functions, but is to be done by the create function.
				*************************************************************************************************************************/
				virtual bool                                LoadTemporaryOwnContent                      (ResMan::SubresourceRoleSet InRoleSet) override final;
				virtual void                                UnloadTemporaryOwnContent                    (ResMan::SubresourceRoleSet InRoleSet) override final;

			protected:
				/************************************************************************************************************************
				* This functions to be overloaded to implement loading shader's source code
				* (the overloads should decide whether to load shader source code from file, from memory, or generate it)
				*
				* The functions return unique pointer to vector of lines of the loaded text
				* (nullptr if the sourc code has not been loaded due to error)
				* 
				* The overloads must only load the source code, without settings the loaded roles flags.
				*************************************************************************************************************************/
				virtual std::shared_ptr<std::vector<std::string>>    _LoadTemporaryOwnContent_SourceCode           () =0;

				/************************************************************************************************************************
				* This function must be overloaded to create the shader object
				*
				* The shader object must use the specified parameters to pass to the shader object creation function.
				************************************************************************************************************************/
				virtual d3d_utils::HResultValue<ShaderObjectType *>  _CreateShaderObject
				(
					ID3D10Blob                                      *pInBytecodeBlob,
					ID3D11ClassLinkage                              *pInClassLinkage
				) =0;

				// Load the given lines of source code to the internal source code storage
				void                                                      _CopySourceCodeFrom(const std::vector<std::string> *pInStrings);

			private:
				// Load shader reflection and load dependencies if they have not been loaded
				// (shader reflection is always created, ever if loaded)
				bool                                                      _TryCreateShaderReflection_AndDependenciesIfDependenciesNotLoaded();
				// Load bytecode and load dependencies if they have not been loaded
				// (bytecode is always loaded, ever if already loaded, ever if loaded)
				bool                                                      _TryCompileBytecode_AndDependenciesIfDependenciesNotLoaded();
				// Perform the source-to-bytecode compilation
				// and mark both the error blob and bytecode blob as loaded
				bool                                                      _TryCompileBytecode_AndMarkLoaded           ();
				// load the shader source code and mark it as loaded if loaded successfully
				bool                                                      _TryLoadFromSourceCode_AndMarkLoaded        ();
				bool                                                      _TryCompileShaderReflection_AndMarkLoaded();

				std::shared_ptr<ID3D11ShaderReflection>                   _pReflection;
				std::shared_ptr<ID3D10Blob>                               _pBytecodeBlob;
				std::shared_ptr<ID3D10Blob>                               _pErrBlob;
				std::string                                               _errorBlobString = "{UNINITIALIZED}";
				std::shared_ptr<std::vector<std::string>>                 _pSourceCode;
				D3DCompile_Params                                         _compileParams;
			};

			/**************************************************************************************************************
			* Shader loader from memory
			**************************************************************************************************************/
			template<class ShaderObjectType>
			class D3D11ShaderLoaderFromMemory : public D3D11ShaderLoaderBase<ShaderObjectType>
			{
			public:
				D3D11ShaderLoaderFromMemory
				(
					const std::vector<std::string>         *pInSourceCode,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				/***************************************************************************************************************
				* Overload moves source code of this class to the base class;
				* So it's impossible to load second time.
				***************************************************************************************************************/
				virtual std::shared_ptr<std::vector<std::string>>    _LoadTemporaryOwnContent_SourceCode() override;

			private:
				std::shared_ptr<std::vector<std::string>>             _pSourceCodeInitialPtr;
			};

			/**************************************************************************************************************
			* Shader loader from file
			**************************************************************************************************************/
			template<class ShaderObjectType>
			class D3D11ShaderLoaderFromFile : public D3D11ShaderLoaderBase<ShaderObjectType>
			{
			public:
				/*************************************************************************************************
				* Constructor
				*
				* Takes full filename (with directory path and extension)
				*************************************************************************************************/
				D3D11ShaderLoaderFromFile
				(
					const std::string                      &InFilename,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual std::shared_ptr<std::vector<std::string>>    _LoadTemporaryOwnContent_SourceCode() override;

			private:
			};

			/**************************************************************************************************************
			* Vertex shader loader from file
			**************************************************************************************************************/
			class D3D11VertexShaderLoaderFromFile : public D3D11ShaderLoaderFromFile<ID3D11VertexShader>
			{
			public:
				/*************************************************************************************************
				* Constructor
				*
				* Takes full filename (with directory path and extension)
				*************************************************************************************************/
				D3D11VertexShaderLoaderFromFile
				(
					const std::string                      &InFilename,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual d3d_utils::HResultValue<ID3D11VertexShader *>  _CreateShaderObject
				(
					ID3D10Blob                                      *pInBytecodeBlob,
					ID3D11ClassLinkage                              *pInClassLinkage
				) override;
			};

			/**************************************************************************************************************
			* Pixel shader loader from file
			**************************************************************************************************************/
			class D3D11PixelShaderLoaderFromFile : public D3D11ShaderLoaderFromFile<ID3D11PixelShader>
			{
			public:
				/*************************************************************************************************
				* Constructor
				*
				* Takes full filename (with directory path and extension)
				*************************************************************************************************/
				D3D11PixelShaderLoaderFromFile
				(
					const std::string                      &InFilename,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual d3d_utils::HResultValue<ID3D11PixelShader *>  _CreateShaderObject
				(
					ID3D10Blob                                      *pInBytecodeBlob,
					ID3D11ClassLinkage                              *pInClassLinkage
				) override;
			};

			/**************************************************************************************************************
			* Compute shader loader from file
			**************************************************************************************************************/
			class D3D11ComputeShaderLoaderFromFile : public D3D11ShaderLoaderFromFile<ID3D11ComputeShader>
			{
			public:
				/*************************************************************************************************
				* Constructor
				*
				* Takes full filename (with directory path and extension)
				*************************************************************************************************/
				D3D11ComputeShaderLoaderFromFile
				(
					const std::string                      &InFilename,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual d3d_utils::HResultValue<ID3D11ComputeShader *>  _CreateShaderObject
				(
					ID3D10Blob                                      *pInBytecodeBlob,
					ID3D11ClassLinkage                              *pInClassLinkage
				) override;
			};

			/**************************************************************************************************************
			* Geometry shader loader from file
			**************************************************************************************************************/
			class D3D11GeometryShaderLoaderFromFile : public D3D11ShaderLoaderFromFile<ID3D11GeometryShader>
			{
			public:
				/*************************************************************************************************
				* Constructor
				*
				* Takes full filename (with directory path and extension)
				*************************************************************************************************/
				D3D11GeometryShaderLoaderFromFile
				(
					const std::string                      &InFilename,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual d3d_utils::HResultValue<ID3D11GeometryShader *>  _CreateShaderObject
				(
					ID3D10Blob                                           *pInBytecodeBlob,
					ID3D11ClassLinkage                                  *pInClassLinkage
				) override;
			};

			/**************************************************************************************************************
			* Hull shader loader from file
			**************************************************************************************************************/
			class D3D11HullShaderLoaderFromFile : public D3D11ShaderLoaderFromFile<ID3D11HullShader>
			{
			public:
				/*************************************************************************************************
				* Constructor
				*
				* Takes full filename (with directory path and extension)
				*************************************************************************************************/
				D3D11HullShaderLoaderFromFile
				(
					const std::string                      &InFilename,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual d3d_utils::HResultValue<ID3D11HullShader *>  _CreateShaderObject
				(
					ID3D10Blob                                      *pInBytecodeBlob,
					ID3D11ClassLinkage                              *pInClassLinkage
				) override;
			};

			/**************************************************************************************************************
			* Domain shader loader from file
			**************************************************************************************************************/
			class D3D11DomainShaderLoaderFromFile : public D3D11ShaderLoaderFromFile<ID3D11DomainShader>
			{
			public:
				/*************************************************************************************************
				* Constructor
				*
				* Takes full filename (with directory path and extension)
				*************************************************************************************************/
				D3D11DomainShaderLoaderFromFile
				(
					const std::string                      &InFilename,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual d3d_utils::HResultValue<ID3D11DomainShader *>  _CreateShaderObject
				(
					ID3D10Blob                                      *pInBytecodeBlob,
					ID3D11ClassLinkage                              *pInClassLinkage
				) override;
			};

			/**************************************************************************************************************
			* D3D11ShaderLoaderFromFile implementation
			**************************************************************************************************************/
			template<class ShaderObjectType>
			D3D11ShaderLoaderFromFile<ShaderObjectType>::D3D11ShaderLoaderFromFile
			(
				const std::string                      &InFilename,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) : D3D11ShaderLoaderBase(InFilename, InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{
			}

			template<class ShaderObjectType>
			std::shared_ptr<std::vector<std::string>> D3D11ShaderLoaderFromFile<ShaderObjectType>::_LoadTemporaryOwnContent_SourceCode()
			{				
				std::ifstream strm(_GetURL(), std::ios::in);
				if (!strm.is_open())
				{
					return nullptr;
				}
				else
				{
					std::unique_ptr<std::vector<std::string>> pResSourceCode;
					std::vector<std::string> *pSourceCode = new std::vector<std::string>();
					bool bLoadSucceeded = Util::LoadStringVectorFromStream(pSourceCode, &strm);
					pResSourceCode.reset(pSourceCode);
					if (bLoadSucceeded)
					{
						return std::move(pResSourceCode);
					}
					else
					{
						return nullptr;
					}
				}
			}

			/**************************************************************************************************************
			* Concrete shader loaders from memory for each shader type
			***************************************************************************************************************/
			class D3D11VertexShaderLoaderFromMemory : public D3D11ShaderLoaderFromMemory<ID3D11VertexShader>
			{
			public:
				D3D11VertexShaderLoaderFromMemory
				(
					const std::vector<std::string>         *pInSourceCode,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual d3d_utils::HResultValue<ID3D11VertexShader *>     _CreateShaderObject
				(
					ID3D10Blob                                      *pInBytecodeBlob,					
					ID3D11ClassLinkage                              *pInClassLinkage
				) override;
			};

			/*************************************************************************************************
			* Pixel shader loader from memory
			*************************************************************************************************/
			class D3D11PixelShaderLoaderFromMemory : public D3D11ShaderLoaderFromMemory<ID3D11PixelShader>
			{
			public:
				/*************************************************************************************************
				* Constructor
				*************************************************************************************************/
				D3D11PixelShaderLoaderFromMemory
				(
					const std::vector<std::string>         *pInSourceCode,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual d3d_utils::HResultValue<ID3D11PixelShader *>  _CreateShaderObject
				(
					ID3D10Blob                                      *pInBytecodeBlob,
					ID3D11ClassLinkage                              *pInClassLinkage
				) override;
			};

			/*************************************************************************************************
			* Compute shader loader from memory
			*************************************************************************************************/
			class D3D11ComputeShaderLoaderFromMemory : public D3D11ShaderLoaderFromMemory<ID3D11ComputeShader>
			{
			public:
				/*************************************************************************************************
				* Constructor
				*************************************************************************************************/
				D3D11ComputeShaderLoaderFromMemory
				(
					const std::vector<std::string>         *pInSourceCode,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual d3d_utils::HResultValue<ID3D11ComputeShader *>  _CreateShaderObject
				(
					ID3D10Blob                                      *pInBytecodeBlob,
					ID3D11ClassLinkage                              *pInClassLinkage
				) override;
			};

			/*************************************************************************************************
			* Geometry shader loader from memory
			*************************************************************************************************/
			class D3D11GeometryShaderLoaderFromMemory : public D3D11ShaderLoaderFromMemory<ID3D11GeometryShader>
			{
			public:
				/*************************************************************************************************
				* Constructor
				*************************************************************************************************/
				D3D11GeometryShaderLoaderFromMemory
				(
					const std::vector<std::string>         *pInSourceCode,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual d3d_utils::HResultValue<ID3D11GeometryShader *>  _CreateShaderObject
				(
					ID3D10Blob                                      *pInBytecodeBlob,
					ID3D11ClassLinkage                              *pInClassLinkage
				) override;
			};

			/*************************************************************************************************
			* Hull shader loader from memory
			*************************************************************************************************/
			class D3D11HullShaderLoaderFromMemory : public D3D11ShaderLoaderFromMemory<ID3D11HullShader>
			{
			public:
				/*************************************************************************************************
				* Constructor
				*************************************************************************************************/
				D3D11HullShaderLoaderFromMemory
				(
					const std::vector<std::string>         *pInSourceCode,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual d3d_utils::HResultValue<ID3D11HullShader *>  _CreateShaderObject
				(
					ID3D10Blob                                      *pInBytecodeBlob,
					ID3D11ClassLinkage                              *pInClassLinkage
				) override;
			};

			/*************************************************************************************************
			* Domain shader loader from memory
			*************************************************************************************************/
			class D3D11DomainShaderLoaderFromMemory : public D3D11ShaderLoaderFromMemory<ID3D11DomainShader>
			{
			public:
				/*************************************************************************************************
				* Constructor
				*************************************************************************************************/
				D3D11DomainShaderLoaderFromMemory
				(
					const std::vector<std::string>         *pInSourceCode,
					const D3DCompile_Params&                InCompileParams,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

			protected:
				virtual d3d_utils::HResultValue<ID3D11DomainShader *>  _CreateShaderObject
				(
					ID3D10Blob                                      *pInBytecodeBlob,
					ID3D11ClassLinkage                              *pInClassLinkage
				) override;
			};

			/**************************************************************************************************************
			* D3D11ShaderLoaderFromMemory implementation
			**************************************************************************************************************/
			template<class ShaderObjectType>
			D3D11ShaderLoaderFromMemory<ShaderObjectType>::D3D11ShaderLoaderFromMemory
			(
				const std::vector<std::string>         *pInSourceCode,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) : 
				D3D11ShaderLoaderBase(InCompileParams, pInDev, pInRole, pInTypedResourceType)
			{
				assert(pInSourceCode != nullptr);
				_pSourceCodeInitialPtr.reset(new std::vector<std::string>(*pInSourceCode));
			}

			template<class ShaderObjectType>
			std::shared_ptr<std::vector<std::string>> D3D11ShaderLoaderFromMemory<ShaderObjectType>::_LoadTemporaryOwnContent_SourceCode()
			{				
				return _pSourceCodeInitialPtr;
			}

			/**************************************************************************************************************
			* D3D11ShaderLoaderBase implementation
			**************************************************************************************************************/
			template<class ShaderObjectType>
			D3D11ShaderLoaderBase<ShaderObjectType>::D3D11ShaderLoaderBase
			(
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) : TypedD3D11ResourceLoaderBase(pInDev, pInRole, pInTypedResourceType),
				_pBytecodeBlob(nullptr),
				_pErrBlob(nullptr),
				_compileParams(InCompileParams)
			{
				static_assert(std::is_base_of<IUnknown, ShaderObjectType>::value, "Wrong base class of ShaderObjectType  template argument");
			}

			template<class ShaderObjectType>
			D3D11ShaderLoaderBase<ShaderObjectType>::D3D11ShaderLoaderBase
			(
				const std::string&                      InURL,
				const D3DCompile_Params&                InCompileParams,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) : TypedD3D11ResourceLoaderBase(InURL, pInDev, pInRole, pInTypedResourceType),
				_pBytecodeBlob(nullptr),
				_pErrBlob(nullptr),
				_compileParams(InCompileParams)
			{
				static_assert(std::is_base_of<IUnknown, ShaderObjectType>::value, "Wrong base class of ShaderObjectType  template argument");
			}
						
			template<class ShaderObjectType>
			typename D3D11ShaderLoaderBase<ShaderObjectType>::RealResourceType *D3D11ShaderLoaderBase<ShaderObjectType>::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				ID3D10Blob *pBytecodeBlobArg = nullptr;
				if (_GetLoadedTempContentRoles().IsSet(GetRoles().pTempBlob))
				{
					pBytecodeBlobArg     = _pBytecodeBlob.get();
				}
				else
				{
					if(pOldPtrOrNull != nullptr && pOldPtrOrNull->GetBytecodeBlob() != nullptr)
					{
						pOldPtrOrNull->GetBytecodeBlob()->AddRef();
						pBytecodeBlobArg = pOldPtrOrNull->GetBytecodeBlob();
					}
				}
				if (nullptr == pBytecodeBlobArg)
				{
					_SetHResult_Create(E_FAIL, _errorBlobString);
					return nullptr;
				}

				// TODO Here: Create class linkage (+DONE)
				ID3D11ClassLinkage *pClassLinkage = nullptr;
				HRESULT hr_create_class_linkage = _GetDev()->CreateClassLinkage(&pClassLinkage);
				if (FAILED(hr_create_class_linkage))
				{
					_SetHResult_Create(hr_create_class_linkage, "Class linkage creation failed");
					return nullptr;
				}
				std::shared_ptr<ID3D11ClassLinkage> pClassLinkageObject(pClassLinkage, d3d_utils::Releaser());

				// d3d11 shader object
				auto create_shader_object_result = _CreateShaderObject
				(
					pBytecodeBlobArg,
					pClassLinkage // TODO: Pass class linkage here (+DONE)
				);
				_SetHResult_Create(create_shader_object_result.GetHResult(), _errorBlobString);
				std::shared_ptr<ShaderObjectType> pShaderObject(create_shader_object_result.GetValue(), d3d_utils::Releaser());
				if (pShaderObject.get() == nullptr)
				{ 
					return nullptr;
				}
				else
				{		
					auto pResultShader = new TypedD3D11Shader<ShaderObjectType>
					(
						pShaderObject,
						pClassLinkageObject,
						_pBytecodeBlob,
						_pErrBlob,
						_pSourceCode,
						_pReflection // TODO Here: Add shader reflection arguments
					);
					return pResultShader;
				}
			}

			template<class ShaderObjectType>
			bool D3D11ShaderLoaderBase<ShaderObjectType>::LoadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				// WARNING! Many roles may be specified, so:
				// 1. Return only at the end;
				// 2. Load independent resources first, then first-order dependencies, and so and so...
				bool bResSuccess = true; // true if no error occured during the loading				

				if (_ShouldLoadTempContent(InRoleSet, GetRoles().pTempSourceCode))
				{					
					if (false == _TryLoadFromSourceCode_AndMarkLoaded())
					{
						bResSuccess = false;
					}
				}

				if (_ShouldLoadTempContent(InRoleSet, GetRoles().pTempBlob))
				{						
					if (false == _TryCompileBytecode_AndDependenciesIfDependenciesNotLoaded())
					{
						bResSuccess = false;
					}
				}

				if (_ShouldLoadTempContent(InRoleSet, GetRoles().pTempErrBlob))
				{
					// WARNING!!! Attention here: equivalent to load temp blob: 
					// if we recompile main blob, then we always recompile temp blob as well
					if (false == _TryCompileBytecode_AndDependenciesIfDependenciesNotLoaded())
					{
						bResSuccess = false;
					}
				}

				// TODO Here : Create shader reflection (+DONE: 12.07.2017)
				if (_ShouldLoadTempContent(InRoleSet, GetRoles().pTempReflection))
				{
					if (false == _TryCreateShaderReflection_AndDependenciesIfDependenciesNotLoaded())
					{
						bResSuccess = false;
					}
				}

				return true;
			}

			template<class ShaderObjectType>
			bool D3D11ShaderLoaderBase<ShaderObjectType>::_TryCreateShaderReflection_AndDependenciesIfDependenciesNotLoaded()
			{
				if (false == _IsLoaded(GetRoles().pTempBlob))
				{
					if (false == _TryCompileBytecode_AndDependenciesIfDependenciesNotLoaded())
					{
						return false;
					}
				}

				if (false == _TryCompileShaderReflection_AndMarkLoaded())
				{
					return false;
				}

				return true;
			}

			template<class ShaderObjectType>
			bool D3D11ShaderLoaderBase<ShaderObjectType>::_TryCompileBytecode_AndDependenciesIfDependenciesNotLoaded()
			{
				// If the source code has not yet been loaded,
				// we need to load it now in order to compile it to bytecode
				if (false == _IsLoaded(GetRoles().pTempSourceCode))
				{
					if (false == _TryLoadFromSourceCode_AndMarkLoaded())
					{
						return false;
					}
				}

				if (false == _TryCompileBytecode_AndMarkLoaded())
				{
					return false;
				}

				return true;
			}

			template<class ShaderObjectType>
			bool D3D11ShaderLoaderBase<ShaderObjectType>::_TryCompileShaderReflection_AndMarkLoaded()
			{
				// Skip shader reflection creation if already created:
				if(_pReflection.get())
				{
					return true;
				}

				ID3D11ShaderReflection *pShaderReflection = nullptr;
				HRESULT hr = D3DReflect(_pBytecodeBlob->GetBufferPointer(), _pBytecodeBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void **)&pShaderReflection);
				if (FAILED(hr))
				{
					_SetHResult_Create(hr, "Shader reflection creation failed");
					return false;
				}
				_pReflection.reset(pShaderReflection, d3d_utils::Releaser());
				_MarkTempContentRole_Loaded(GetRoles().pTempReflection);
				return true;
			}

			template<class ShaderObjectType>
			bool D3D11ShaderLoaderBase<ShaderObjectType>::_TryLoadFromSourceCode_AndMarkLoaded()
			{
				_pSourceCode = _LoadTemporaryOwnContent_SourceCode();
				if (_pSourceCode.get() != nullptr)
				{
					// mark source code as loaded ONLY IF it has been loaded SUCCESSFULLY
					_MarkTempContentRole_Loaded(GetRoles().pTempSourceCode);
					return true;
				}
				else
				{
					return false;
				}
			}

			template<class ShaderObjectType>
			bool D3D11ShaderLoaderBase<ShaderObjectType>::_TryCompileBytecode_AndMarkLoaded()
			{				
				std::string sourceCodeStr = Util::linearize_str(_pSourceCode.get()->cbegin(), _pSourceCode.get()->cend());
				std::string source_name = _HasNoURL() ? std::string("{NoUrl}") : _GetURL();
				ID3D10Blob *pBytecodeBlob = nullptr;
				ID3D10Blob *pErrBlob = nullptr;
				D3D_SHADER_MACRO *pDefinesArg = nullptr;
				if (_compileParams.macros_defines.size() > 0)
				{
					pDefinesArg = &(_compileParams.macros_defines.front());
				}
				HRESULT hr = D3DCompile
				(
					sourceCodeStr.c_str(),
					sourceCodeStr.size(),
					source_name.c_str(),
					pDefinesArg,
					nullptr, // pInclude,
					_compileParams.entry_point.c_str(),
					_compileParams.compile_target.c_str(),
					_compileParams.Flags1,
					_compileParams.Flags2,
					&pBytecodeBlob,
					&pErrBlob
				);
				if (pErrBlob != nullptr)
				{
					if (pErrBlob->GetBufferPointer() != nullptr)
					{
						auto pCharBlobPtr = static_cast<char *>(pErrBlob->GetBufferPointer());
						const std::string errorBlobStrPrefix = "ErrorBlob:\n ";
						_errorBlobString.resize(errorBlobStrPrefix.size() + pErrBlob->GetBufferSize());
						std::copy_n(errorBlobStrPrefix.begin(), errorBlobStrPrefix.size(), _errorBlobString.begin());
						std::copy_n(pCharBlobPtr, pErrBlob->GetBufferSize(), _errorBlobString.begin() + errorBlobStrPrefix.size());
					}
					else
					{
						_errorBlobString.append("{pErrBlob->GetBufferPointer() is nullptr}");
					}
				}
				else
				{
					_errorBlobString.append("{pErrBlob is nullptr}");
				}
				_SetHResult_Create(hr, _errorBlobString);
				_pBytecodeBlob.reset(pBytecodeBlob, d3d_utils::Releaser());
				_pErrBlob.reset(pErrBlob, d3d_utils::Releaser());
				if (FAILED(hr))
				{
					return false;
				}
				else
				{
					// Mark both the temp blob and err blob as loaded
					_MarkTempContentRole_Loaded(GetRoles().pTempBlob);
					_MarkTempContentRole_Loaded(GetRoles().pTempErrBlob);
				}
				return true;
			}

			template<class ShaderObjectType>
			void D3D11ShaderLoaderBase<ShaderObjectType>::UnloadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				if (_ShouldTempContentBeUnloaded(InRoleSet, GetRoles().pTempReflection))
				{
					_pReflection.reset(static_cast<ID3D11ShaderReflection *>(nullptr));
					_MarkTempContentRole_Unloaded(GetRoles().pTempReflection);
				}

				if (_ShouldTempContentBeUnloaded(InRoleSet, GetRoles().pTempSourceCode))
				{
					_pSourceCode.reset(static_cast<Dv::Utils::LineVector *>(nullptr));
					_MarkTempContentRole_Unloaded(GetRoles().pTempSourceCode);
				}

				if (_ShouldTempContentBeUnloaded(InRoleSet, GetRoles().pTempBlob))
				{
					_pBytecodeBlob.reset(static_cast<ID3D10Blob *>(nullptr), d3d_utils::Releaser());
					_MarkTempContentRole_Unloaded(GetRoles().pTempBlob);
				}

				if (_ShouldTempContentBeUnloaded(InRoleSet, GetRoles().pTempErrBlob))
				{
					_pErrBlob.reset(static_cast<ID3D10Blob *>(nullptr), d3d_utils::Releaser());
					_MarkTempContentRole_Unloaded(GetRoles().pTempErrBlob);
				}
			}

			template<class ShaderObjectType>
			void D3D11ShaderLoaderBase<ShaderObjectType>::_CopySourceCodeFrom(const std::vector<std::string> *pInStrings)
			{
				assert(pInStrings != nullptr);
				_pSourceCode.reset(new std::vector<std::string>(*pInStrings));
			}

			/**************************************************************************************************************
			* ResTypeDescr_Shader IMPLEMENTATION
			**************************************************************************************************************/
			template<class ShaderObjectType>
			ResTypeDescr_Shader<ShaderObjectType>::ResTypeDescr_Shader() :
				TypedD3D11ResTypeDescrBase("Shader", "Shader")
			{
			}

			template<class ShaderObjectType>
			void ResTypeDescr_Shader<ShaderObjectType>::RegisterApplicableRoles(ResMan::IResourceManagerSubsystem *pInSubsys)
			{
				_RegisterDefaultRoles(pInSubsys);
				_roles.pTempBlob = ResMan::RegisterSubresourceRole_ThatMustBeLoaded(pInSubsys, _params.GetSubresourceRolesPtr(),"TempBlob");
				_roles.pTempErrBlob = ResMan::RegisterSubresourceRole(pInSubsys, _params.GetSubresourceRolesPtr(), "ErrorBlob");
				_roles.pTempSourceCode = ResMan::RegisterSubresourceRole(pInSubsys, _params.GetSubresourceRolesPtr(), "TempSourceCode");
				_roles.pTempReflection = ResMan::RegisterSubresourceRole(pInSubsys, _params.GetSubresourceRolesPtr(), "Reflection");
			}

			template<class ShaderObjectType>
			void ResTypeDescr_Shader<ShaderObjectType>::Delete(void *pInResource)
			{
				delete pInResource;
			}

			/**************************************************************************************************************
			* TypedD3D11Shader IMPLEMENTATION
			**************************************************************************************************************/
			template<class ShaderObjectType>
			TypedD3D11Shader<ShaderObjectType>::TypedD3D11Shader
			(
				ShaderObjectPtr                                  pInShader,
				ClassLinkagePtr                                  pInClassLinkage,
				BlobPtr                                          pInBytecodeBlob,
				BlobPtr                                          pInErrBlob,
				SourceCodePtr                                    pInSourceCode,
				ReflectionPtr                                    pInReflection
			) :
				_pShader(pInShader),
				_pClassLinkage(pInClassLinkage),
				_pBytecodeBlob(pInBytecodeBlob),
				_pErrorBlob(pInErrBlob),
				_pSourceCode(pInSourceCode),
				_pReflection(pInReflection)
			{
				assert(_pShader.get() != nullptr);
				assert(_pClassLinkage.get());
			}
		}
	}
}