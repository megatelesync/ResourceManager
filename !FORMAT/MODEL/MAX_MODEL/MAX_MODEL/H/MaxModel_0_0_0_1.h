#pragma once

#include "MaxModel_Utils.h"

#include <memory>

namespace Dv
{
	namespace MaxModel
	{
		namespace Ver_0_0_0_1
		{			
			const std::string                 CURRENT_SIGNATURE          = "DML";
			const Version                     CURRENT_VERSION            = { 1,0,0,0 };
			const FileHeader                  REFERENCE_HEADER           = FileHeader{ CURRENT_SIGNATURE, CURRENT_VERSION };
			const std::string                 BITMAP_TEXMAP_CLASS_NAME   = "BitmapTexture";
			using EffectsChannelType                                      = uint32_t;

			/************************************************************************************************
			* Node info
			************************************************************************************************/
			struct NodeInfo
			{
				std::string                     name;
			};

			/************************************************************************************************
			* Info about the material
			************************************************************************************************/
			struct MaterialInfo
			{
				std::string                            name;
				EffectsChannelType                     effectsChannel;
			};
			
			/************************************************************************************************
			* Describes texture map
			************************************************************************************************/
			class TexMapBase
			{
			public:
				TexMapBase(const std::string& InName) : _className(InName) {}

				const std::string& GetClassName() const { return _className; }

			private:
				std::string _className;
			};

			class BitmapTexMap : public TexMapBase
			{
			public:
				BitmapTexMap(const std::string& InFilename) : TexMapBase(BITMAP_TEXMAP_CLASS_NAME), _filename(InFilename) {}

				const std::string                     GetFilename() const { return _filename; }

			private:
				std::string                           _filename;
			};
			// Converts to bitmap or return nullptr if cannot convert
			BitmapTexMap *ToBitmapTexMap(TexMapBase *pInTexMap);

			class Material
			{
			public:
				// Container of submaterials (map submaterial slot names to materials)
				using SubMatCont = std::multimap<std::string, std::unique_ptr<Material>>;
				// Container of texture maps (map tex map slot names to texture maps)
				using TexMapCont = std::multimap<std::string, std::unique_ptr<TexMapBase>>;

				const MaterialInfo&                    GetInfo                   () const { return _info; }
				void                                   SetMaterialInfo           (const MaterialInfo& InInfo) { _info = InInfo; }

				Material                              *RegisterSubMaterial       (const std::string& InSlotName);
				void                                   RegisterTexMap            (const std::string& InSlotName, TexMapBase *pInTexMap);

			private:
				MaterialInfo                           _info;
				SubMatCont                             _subMaterials;
				TexMapCont                             _texMaps;
			};

			/************************************************************************************************
			* Bone weights of the given vertex
			************************************************************************************************/
			using VertexBoneWeights = std::map<std::string, float>;

			/***********************************************************************************************
			* Bone weights of all vertices of the mesh
			***********************************************************************************************/
			class MeshBoneWeights
			{
			public:
				// Cont of size of number of vertices in the mesh that stores weights of the bones for each vertex
				using Cont                              = std::vector<VertexBoneWeights>;
				using VertexBoneWeightsHandle           = Cont::iterator;
				using VertexIndex                       = size_t;

				/********************************************************************************************************************
				* Getters
				********************************************************************************************************************/
				size_t                                  GetNumVertices                   () const                        { return _vertexBoneWeights.size(); }
				const VertexBoneWeights&                GetBoneWeights_ForVertex         (VertexIndex InVertexIndex) const { return _vertexBoneWeights[InVertexIndex]; }
				const Cont&                             GetVertexBoneWeights             () const                        { return _vertexBoneWeights; }

				
				size_t                                  GetNumBoneWeights_ForVertex      (VertexBoneWeightsHandle InVertexHandle) const { return InVertexHandle->size(); }
				size_t                                  GetNumBoneWeights_ForVertex      (VertexIndex InVertexIndex) const { return _vertexBoneWeights[InVertexIndex].size(); }
				float                                   GetBoneWeight_ForVertex          (VertexBoneWeightsHandle InVertexHandle, const std::string& InBoneName);
				float                                   GetBoneWeight_ForVertex          (VertexIndex InVertexIndex, const std::string& InBoneName);

				/********************************************************************************************************************
				* Filling
				********************************************************************************************************************/
				VertexBoneWeightsHandle                 AppendVertexBoneWeights          ();
				void                                    AppendBoneWeight_ForVertex       (VertexBoneWeightsHandle InVertexHandle, const std::string& InBoneName, float InBoneWeight);

			private:
				Cont                                    _vertexBoneWeights;
			};

			struct SkeletonStaticInfo
			{
				size_t                                  numBones = 0;
				MeshBoneWeights                         meshBoneWeights;
			};

			/*********************************************************************************
			* State of a single bone
			*********************************************************************************/
			struct BoneState
			{
				Point3                                  position;
				Point4                                  rotation; 
			};

			/*********************************************************************************
			* State of the entire skeleton
			*********************************************************************************/
			class SkeletonState
			{
			public:
				// Associates each bone name with state of the bone
				using Cont = std::map<std::string, BoneState>;
				using const_iterator = Cont::const_iterator;

				/********************************************************************************************************************
				* Getters
				********************************************************************************************************************/
				size_t                                  GetNumBones               () const                      { return _boneStates.size(); }

				const_iterator                          GetBones_Begin            () const                      { return _boneStates.cbegin(); }
				const_iterator                          GetBones_End              () const                      { return _boneStates.cend(); }
				const BoneState&                        GetBoneStateByName        (const std::string& InBoneName) const { return _boneStates.at(InBoneName); }

				/********************************************************************************************************************
				* Filling
				********************************************************************************************************************/
				void                                    AddBoneState              (const std::string& InBoneName, const BoneState& InBoneState) { _boneStates.insert(std::make_pair(InBoneName, InBoneState)); }


			private:
				Cont                                    _boneStates;
			};

			/*******************************************************************************************
			* Set of frames of the skeleton
			*******************************************************************************************/
			class SkeletonFrames
			{
			public:
				// Container that with associates state of the skeleton with each frame index
				using Cont = std::vector<SkeletonState>;
				using FrameIndex = size_t;

				/*****************************************************************************************
				* Get num frames
				*****************************************************************************************/
				size_t                                GetNumFrames               () const { return _frames.size(); }
				// Get frame by zero-based index
				const SkeletonState&                  GetFrameByIndex            (FrameIndex InFrameIndex) const { return _frames[InFrameIndex]; }

				/*****************************************************************************************
				* Filling with frames
				*****************************************************************************************/
				SkeletonState                        *AppendFrame                () { return &(*_frames.insert(_frames.end(), SkeletonState())); }

			private:
				Cont                                  _frames;
			};


			/*******************************************************************************************
			* Animation static (i.e. time-independent) info
			*******************************************************************************************/
			struct AnimStaticInfo
			{
				using FrameIndex                             = SkeletonFrames::FrameIndex;

				FrameIndex                                   startFrame;
				FrameIndex                                   endEnd;
				size_t                                       NumFrames;
			};

			/*******************************************************************************************
			* Skeleton and corresponding animations
			*******************************************************************************************/
			class SkeletonAndAnimations
			{
			public:
				/**********************************************************************************
				* Getters
				**********************************************************************************/
				const SkeletonStaticInfo&          GetSkeletonStaticInfo      () const               { return _skeletonStaticInfo; }
				const AnimStaticInfo&              GetAnimStaticInfo          () const               { return _animStaticInfo; }
				const SkeletonFrames&              GetSkeletonFrames          () const               { return _skeletonFrames; }

				SkeletonStaticInfo&                GetSkeletonStaticInfo      ()                     { return _skeletonStaticInfo; }
				AnimStaticInfo&                    GetAnimStaticInfo          ()                     { return _animStaticInfo; }
				SkeletonFrames&                    GetSkeletonFrames          ()                     { return _skeletonFrames; }

			private:
				SkeletonStaticInfo                           _skeletonStaticInfo;
				AnimStaticInfo                               _animStaticInfo;
				SkeletonFrames                               _skeletonFrames;
			};

			struct Vertex
			{
				Point3                                  position;
				Point3                                  normal;
			};

			struct Face
			{
				MaterialID                             materialId;
				Point3                                 normal;
				VertIndices3                           vertexIndices;
			};

			struct TextureVertex
			{
				Point3                                  UVW;
			};

			struct TextureFace
			{
				VertIndices3                            textureVertexIndices;
			};

			/*****************************************************************************************************************
			* Entire loaded file
			*****************************************************************************************************************/
			class File
			{
			public:
				const FileHeader                       *GetHeader() const { return &_header; }

				/********************************************************************************************************************
				* Ver 0.0.0.1-related stuff
				********************************************************************************************************************/
				const NodeInfo&                        GetNodeInfo         () const               { return _nodeInfo; }
				const Material&                        GetMaterial         () const               { return _material; }
				const SkeletonAndAnimations&           GetSkelAndAnims     () const               { return _skelAndAnims; }

				NodeInfo&                              GetNodeInfo         ()                     { return _nodeInfo; }
				Material&                              GetMaterial         ()                     { return _material; }
				SkeletonAndAnimations&                 GetSkelAndAnims     ()                     { return _skelAndAnims; }

				/********************************************************************************************************************
				* Ver 0.0.0.0-related stuff
				********************************************************************************************************************/
				int                                     GetNumVertices() const { return _vertices.size(); }
				const Vertex                           *GetVertex(int InIndex) const { assert(InIndex >= 0);  assert(InIndex < GetNumVertices()); return &(_vertices[InIndex]); }

				int                                     GetNumFaces() const { return _faces.size(); }
				const Face                             *GetFace(int InIndex) const { assert(InIndex >= 0);  assert(InIndex < GetNumFaces()); return &(_faces[InIndex]); }

				int                                     GetNumTexturesVertices() const { return _textureVertices.size(); }
				const TextureVertex                    *GetTextureVertex(int InIndex) const { return &(_textureVertices[InIndex]); }

				int                                     GetNumTextureFaces() const { return _textureFaces.size(); }
				const TextureFace                      *GetTextureFace(int InIndex) const { return &(_textureFaces[InIndex]); }

				void                                    SetHeader(const FileHeader *pInHeader) { assert(pInHeader);  _header = *pInHeader; }
				void                                    AppendVertex(const Vertex *pInVertex) { _vertices.push_back(*pInVertex); }
				void                                    AppendFace(const Face *pInFace) { _faces.push_back(*pInFace); }
				void                                    AppendTextureVertex(const TextureVertex *pInTextureVertex) { _textureVertices.push_back(*pInTextureVertex); }
				void                                    AppendTextureFace(const TextureFace *pInTextureFace) { _textureFaces.push_back(*pInTextureFace); }

			private:
				FileHeader                              _header;

				/*********************************************************************************************************
				* Ver 0.0.0.1-related stuff
				**********************************************************************************************************/
				NodeInfo                               _nodeInfo;
				Material                               _material;
				SkeletonAndAnimations                  _skelAndAnims;

				/*********************************************************************************************************
				* Ver 0.0.0.0-related stuff
				**********************************************************************************************************/
				std::vector<Vertex>                     _vertices;
				std::vector<Face>                       _faces;
				std::vector<TextureVertex>              _textureVertices;
				std::vector<TextureFace>                _textureFaces;
			};

			// Loading from file: returns non-initialized value if successfully loaded
			boost::optional<Error> TryLoadFromFile(std::ifstream *pStrm, File *pOutFile);
		}
	}
}