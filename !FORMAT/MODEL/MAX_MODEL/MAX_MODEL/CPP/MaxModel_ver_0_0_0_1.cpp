#include "../H/MaxModel_0_0_0_1.h"

#include "../PrivateH/MatModel_Utils_Private.h"

namespace Dv
{
	namespace MaxModel
	{
		namespace Ver_0_0_0_1
		{
			/*******************************************************************************************
			* Fwd declarations
			*******************************************************************************************/
			bool TryLoadNodeInfo(std::ifstream *pStrm, NodeInfo *pOutData);
			bool TryLoadMaterial(std::ifstream *pStrm, Material *pOutData);
			bool TryLoadMaterialInfo(std::ifstream *pStrm, MaterialInfo *pOutData);
			bool TryLoadSubmaterials(std::ifstream *pStrm, Material *pOutData);
			bool TryLoadMaterialTexMaps(std::ifstream *pStrm, Material *pOutData);
			
			// Load tex map and create it in dynamic memory using C++ new operator
			bool TryLoadNewTexMap(std::ifstream *pStrm, TexMapBase **ppOutTexMap);
			bool TryLoadBitmapTexMap(std::ifstream *pStrm, BitmapTexMap **ppOutTexMap);

			bool TryLoadSkeletonAndAnimations(std::ifstream *pStrm, SkeletonAndAnimations *pOutSkelAndAnims);
			bool TryLoadMeshBoneWeights(std::ifstream *pStrm, MeshBoneWeights *pOutMeshBoneWeights);
			bool TryLoadSkeletonStaticInfo(std::ifstream *pStrm, SkeletonStaticInfo *pOutSkeletonStaticInfo);
			bool TryLoadSkeletonState(std::ifstream *pStrm, SkeletonState *pOutSkeleton, size_t InNumBones);
			bool TryLoadBoneState(std::ifstream *pStrm, BoneState *pOutBone);
			bool TryLoadSkeletonFrames(std::ifstream *pStrm, SkeletonFrames *pOutSkeletonFrames, const SkeletonAndAnimations *pSkelAndAnims);
			bool TryLoadAnimStaticInfo(std::ifstream *pStrm, AnimStaticInfo *pOutAnimStaticInfo, const SkeletonAndAnimations *pSkelAndAnims);

			/***************************************************************************************************************************
			* Fwd declarations copied from version 0.0.0.0
			***************************************************************************************************************************/
			boost::optional<Error>        TryLoadVertices(std::ifstream *pStrm, File *pOutFile);
			boost::optional<Error>        TryLoadFaces(std::ifstream *pStrm, File *pOutFile);
			boost::optional<Error>        TryLoadTextureVertices(std::ifstream *pStrm, File *pOutFile);
			boost::optional<Error>        TryLoadTextureFaces(std::ifstream *pStrm, File *pOutFile);

			bool                          TryLoadMeshVertex(std::ifstream& strm, Vertex *pOutVertex);
			bool                          TryLoadFace(std::ifstream& strm, Face *pOutFace);

			bool                          TryLoadTextureVertex(std::ifstream& strm, TextureVertex *pOutVertex);
			bool                          TryLoadTextureFace(std::ifstream& strm, TextureFace *pOutFace);
			
			// Load extra data of the version 0.0.0.1 file format that is situated before start of the ver0.0.0.0 mesh data
			boost::optional<Error>        TryLoad_Ver0_0_0_1_MeshExtraDataStart(std::ifstream *pStrm, File *pOutFile);			
			// Load extra data of the version 0.0.0.1 file format that is situated at the end of the file (after data of ver 0.0.0.0 )
			boost::optional<Error>        TryLoad_Ver0_0_0_1_EndData(std::ifstream *pStrm, File *pOutFile);

			/*******************************************************************************************
			* implementation
			*******************************************************************************************/
			// Loading from file: returns non-initialized value if successfully loaded
			boost::optional<Error> TryLoadFromFile(std::ifstream *pStrm, File *pOutFile)
			{
				assert(pOutFile);

				FileHeader fileHeader;
				auto resultError = TryLoadAndCheckHeader(pStrm, &fileHeader, &REFERENCE_HEADER);
				if (resultError.is_initialized())
				{
					return resultError;
				}
				pOutFile->SetHeader(&fileHeader);

				/******************************************************************************************************
				* WARNING!!! Ver0.0.0.0 new data!!!
				******************************************************************************************************/
				boost::optional<Error> beforeMeshDataLoadError = TryLoad_Ver0_0_0_1_MeshExtraDataStart(pStrm, pOutFile);
				if (beforeMeshDataLoadError.is_initialized())
				{
					return beforeMeshDataLoadError;
				}

				auto verticesLoadedResult = TryLoadVertices(pStrm, pOutFile);
				if (verticesLoadedResult.is_initialized())
				{
					return verticesLoadedResult;
				}

				auto facesLoadedResult = TryLoadFaces(pStrm, pOutFile);
				if (facesLoadedResult.is_initialized())
				{
					return facesLoadedResult;
				}

				auto textureVerticesLoadedResult = TryLoadTextureVertices(pStrm, pOutFile);
				if (textureVerticesLoadedResult.is_initialized())
				{
					return textureVerticesLoadedResult;
				}

				auto textureFacesLoadedResult = TryLoadTextureFaces(pStrm, pOutFile);
				if (textureFacesLoadedResult.is_initialized())
				{
					return textureFacesLoadedResult;
				}

				/******************************************************************************************************
				* WARNING!!! Ver0.0.0.0 new data!!!
				******************************************************************************************************/
				auto ver_0_0_0_1_load_result = TryLoad_Ver0_0_0_1_EndData(pStrm, pOutFile);
				if (ver_0_0_0_1_load_result.is_initialized())
				{
					return ver_0_0_0_1_load_result;
				}

				// Successfully loaded here
				return boost::optional<Error>();
			}

			boost::optional<Error> TryLoad_Ver0_0_0_1_MeshExtraDataStart(std::ifstream *pStrm, File *pOutFile)
			{
				bool bNodeInfoLoaded = TryLoadNodeInfo(pStrm, &(pOutFile->GetNodeInfo()));
				if(false == bNodeInfoLoaded)
				{
					return Error("Error while reading node data");
				}
				bool bMaterialLoaded = TryLoadMaterial(pStrm, &(pOutFile->GetMaterial()));
				if (false == bMaterialLoaded)
				{
					return Error("Error while loading material");
				}
				// Successfully loaded here
				return boost::optional<Error>();
			}

			boost::optional<Error> TryLoad_Ver0_0_0_1_EndData(std::ifstream *pStrm, File *pOutFile)
			{
				bool bSkelAndAnimsLoaded = TryLoadSkeletonAndAnimations(pStrm, &(pOutFile->GetSkelAndAnims()));
				if (false == bSkelAndAnimsLoaded)
				{
					return Error("Error while trying to read extra data of ver0.0.0.1 after ver0.0.0.0");
				}
				// Successfully loaded here
				return boost::optional<Error>();
			}

			bool TryLoadSkeletonAndAnimations(std::ifstream *pStrm, SkeletonAndAnimations *pOutSkelAndAnims)
			{
				assert(pStrm);
				assert(pOutSkelAndAnims);
				bool bSkelStaticInfo_Loaded = TryLoadSkeletonStaticInfo(pStrm, &(pOutSkelAndAnims->GetSkeletonStaticInfo()));
				if (false == bSkelStaticInfo_Loaded)
				{
					return false;
				}
				bool bAnimStaticInfo_Loaded = TryLoadAnimStaticInfo(pStrm, &pOutSkelAndAnims->GetAnimStaticInfo(), pOutSkelAndAnims);
				if (false == bAnimStaticInfo_Loaded)
				{
					return false;
				}
				bool bSkeletonFrames_Loaded = TryLoadSkeletonFrames(pStrm, &(pOutSkelAndAnims->GetSkeletonFrames()), pOutSkelAndAnims);
				if(false == bSkeletonFrames_Loaded)
				{
					return false;
				}
				return true;
			}
			bool TryLoadMeshBoneWeights(std::ifstream *pStrm, MeshBoneWeights *pOutMeshBoneWeights)
			{
				assert(pStrm);
				assert(pOutMeshBoneWeights);
				uint32_t numVertices = 0;
				bool bNumVerticesLoaded = Util::TryLoadUInt32(pStrm, &numVertices);
				if (false == bNumVerticesLoaded)
				{
					return false;
				}
				for (int vert_index = 0; vert_index < numVertices; vert_index++)
				{
					uint32_t numWeights_ForVertex = 0;
					bool bVertexWeightCountLoaded = Util::TryLoadUInt32(pStrm, &numWeights_ForVertex);
					if (false == bVertexWeightCountLoaded)
					{
						return false;
					}
					auto vertexHandle = pOutMeshBoneWeights->AppendVertexBoneWeights();
					for (int bone_weight_index = 0; bone_weight_index < numWeights_ForVertex; bone_weight_index++)
					{
						std::string boneName;
						float boneWeight = 0.0F;
						bool bBoneNameLoaded = Util::TryLoadString_ZeroTerminated(pStrm, &boneName);
						if (false == bBoneNameLoaded)
						{
							return false;
						}
						bool bBoneWeightLoaded = Util::TryLoadFloat32(pStrm, &boneWeight);
						if (false == bBoneWeightLoaded)
						{
							return false;
						}
						pOutMeshBoneWeights->AppendBoneWeight_ForVertex(vertexHandle, boneName, boneWeight);
					}
				}
				return true;
			}
			bool TryLoadSkeletonStaticInfo(std::ifstream *pStrm, SkeletonStaticInfo *pOutSkeletonStaticInfo)
			{
				assert(pStrm);
				assert(pOutSkeletonStaticInfo);
				bool bNumBonesLoaded = Util::TryLoadUInt32(pStrm, &(pOutSkeletonStaticInfo->numBones));
				if (false == bNumBonesLoaded)
				{
					return false;
				}
				bool bWeightsLoaded = TryLoadMeshBoneWeights(pStrm, &(pOutSkeletonStaticInfo->meshBoneWeights));
				if (false == bWeightsLoaded)
				{
					return false;
				}
				return true;
			}
			bool TryLoadSkeletonState(std::ifstream *pStrm, SkeletonState *pOutSkeleton, size_t InNumBones)
			{
				assert(pStrm);
				assert(pOutSkeleton);
				for (int i = 0; i < InNumBones; i++)
				{
					std::string boneName;
					bool bBoneNameLoaded = Util::TryLoadString_ZeroTerminated(pStrm, &boneName);
					if (false == bBoneNameLoaded)
					{
						return false;
					}

					BoneState boneState;
					bool bBoneLoaded = TryLoadBoneState(pStrm, &boneState);
					if (false == bBoneLoaded)
					{
						return false;
					}
					pOutSkeleton->AddBoneState(boneName, boneState);
				}
			}
			bool TryLoadBoneState(std::ifstream *pStrm, BoneState *pOutBone)
			{
				assert(pStrm);
				assert(pOutBone);
				bool bPositionLoaded = TryLoadPoint3(*pStrm, &(pOutBone->position));
				if (false == bPositionLoaded)
				{
					return false;
				}
				bool bRotLoaded = TryLoadPoint4(*pStrm, &(pOutBone->rotation));
				if(false == bRotLoaded)
				{
					return false;
				}
				return true;
			}
			bool TryLoadSkeletonFrames(std::ifstream *pStrm, SkeletonFrames *pOutSkeletonFrames, const SkeletonAndAnimations *pSkelAndAnims)
			{
				assert(pStrm);
				assert(pOutSkeletonFrames);
				for (int i = 0; i < pSkelAndAnims->GetAnimStaticInfo().NumFrames; i++)
				{
					SkeletonState skelState;
					bool bFrameLoaded = TryLoadSkeletonState(pStrm, &skelState, pSkelAndAnims->GetSkeletonStaticInfo().numBones);
					if (false == bFrameLoaded)
					{

						return false;
					}
				}
				return true;
			}
			bool TryLoadAnimStaticInfo(std::ifstream *pStrm, AnimStaticInfo *pOutAnimStaticInfo, const SkeletonAndAnimations *pSkelAndAnims)
			{
				assert(pStrm);
				assert(pOutAnimStaticInfo);
				bool bStartFrameLoaded = Util::TryLoadUInt32(pStrm, &(pOutAnimStaticInfo->startFrame));
				if (false == bStartFrameLoaded)
				{
					return false;
				}
				bool bEndFrameLoaded = Util::TryLoadUInt32(pStrm, &(pOutAnimStaticInfo->endEnd));
				if (false == bEndFrameLoaded)
				{
					return false;
				}
				bool bNumFramesLoaded = Util::TryLoadUInt32(pStrm, &(pOutAnimStaticInfo->NumFrames));
				if (false == bNumFramesLoaded)
				{
					return false;
				}
				return true;
			}

			bool TryLoadMaterial(std::ifstream *pStrm, Material *pOutData)
			{
				assert(pStrm);
				assert(pOutData);
				MaterialInfo info;
				bool bInfoLoaded = TryLoadMaterialInfo(pStrm, &info);
				if (false == bInfoLoaded)
				{
					return false;
				}
				pOutData->SetMaterialInfo(info);
				if (false == TryLoadSubmaterials(pStrm, pOutData))
				{
					return false;
				}
				if (false == TryLoadMaterialTexMaps(pStrm, pOutData))
				{
					return false;
				}
				return true;
			}

			bool TryLoadMaterialInfo(std::ifstream *pStrm, MaterialInfo *pOutData)
			{
				assert(pStrm);
				assert(pOutData);
				bool bNameLoaded = Util::TryLoadString_ZeroTerminated(pStrm, &(pOutData->name));
				if (false == bNameLoaded)
				{
					return false;
				}
				bool bEffectsChannelLoaded = Util::TryLoadUInt32(pStrm, &(pOutData->effectsChannel));
				if (false == bEffectsChannelLoaded)
				{
					return false;
				}
				return true;
			}

			bool TryLoadSubmaterials(std::ifstream *pStrm, Material *pOutData)
			{
				uint32_t numSubmaterials = 0;
				bool bNumSubmaterialsLoaded = Util::TryLoadUInt32(pStrm, &numSubmaterials);
				if (false == bNumSubmaterialsLoaded)
				{
					return false;
				}
				for (int i = 0; i < numSubmaterials; i++)
				{
					Material subMaterial;
					std::string submaterialName;
					bool bSubmaterialNameLoaded = Util::TryLoadString_ZeroTerminated(pStrm, &submaterialName);
					if (false == bSubmaterialNameLoaded)
					{
						return false;
					}
					auto pSubmaterial = pOutData->RegisterSubMaterial(submaterialName);
					bool bSubmaterialLoaded = TryLoadMaterial(pStrm, pSubmaterial);
					if (false == bSubmaterialLoaded)
					{
						return false;
					}
				}
				return true;
			}
			bool TryLoadMaterialTexMaps(std::ifstream *pStrm, Material *pOutData)
			{
				uint32_t numTexMaps = 0;
				bool bNumTexMapsLoaded = Util::TryLoadUInt32(pStrm, &numTexMaps);
				if (false == bNumTexMapsLoaded)
				{
					return false;
				}
				for (int i = 0; i < numTexMaps; i++)
				{
					std::string texMapSlotName;
					bool bSlotNameLoaded = Util::TryLoadString_ZeroTerminated(pStrm, &texMapSlotName);
					if (false == bSlotNameLoaded)
					{
						return false;
					}

					TexMapBase *pTexMap = nullptr;
					bool bTexMapLoaded = TryLoadNewTexMap(pStrm, &pTexMap);
					if (false == bTexMapLoaded)
					{
						return false;
					}
					pOutData->RegisterTexMap(texMapSlotName, pTexMap);
				}
				return true;
			}

			bool TryLoadNewTexMap(std::ifstream *pStrm, TexMapBase **ppOutTexMap)
			{
				assert(pStrm);
				assert(ppOutTexMap);
				std::string className;
				bool bClassNameLoaded = Util::TryLoadString_ZeroTerminated(pStrm, &className);
				if (false == bClassNameLoaded)
				{
					return false;
				}		
				bool bLoaded = false;
				if (className == "BitmapTexture")
				{
					void *pVoidOutTexMap = ppOutTexMap;
					bLoaded = TryLoadBitmapTexMap(pStrm, static_cast<BitmapTexMap **>(pVoidOutTexMap));
				}
				else if (className == "UNKNOWN_CLASS")
				{
					// Always true, if some tex map class is unknown
					*ppOutTexMap = nullptr;
					bLoaded = true;
				}
				return bLoaded;
			}
			bool TryLoadBitmapTexMap(std::ifstream *pStrm, BitmapTexMap **ppOutTexMap)
			{
				assert(pStrm);
				assert(ppOutTexMap);
				std::string filename;
				bool bFilenameLoaded = Util::TryLoadString_ZeroTerminated(pStrm, &filename);
				if (false == bFilenameLoaded)
				{
					return false;
				}
				auto pBitmap = new BitmapTexMap(filename);
				return true;
			}

			bool TryLoadNodeInfo(std::ifstream *pStrm, NodeInfo *pOutData)
			{
				assert(pStrm);
				assert(pOutData);
				bool bNameLoaded = Util::TryLoadString_ZeroTerminated(pStrm, &(pOutData->name));
				if (false == bNameLoaded)
				{
					return false;
				}
				return true;
			}

			/*******************************************************************************************************
			* MeshBoneWeights implementation
			*******************************************************************************************************/
			MeshBoneWeights::VertexBoneWeightsHandle MeshBoneWeights::AppendVertexBoneWeights()
			{
				auto it = _vertexBoneWeights.emplace(_vertexBoneWeights.end());
				return it;
			}
			void MeshBoneWeights::AppendBoneWeight_ForVertex(VertexBoneWeightsHandle InVertexHandle, const std::string& InBoneName, float InBoneWeight)
			{
				InVertexHandle->insert(InVertexHandle->end(), std::make_pair(InBoneName, InBoneWeight));
			}

			float MeshBoneWeights::GetBoneWeight_ForVertex(VertexBoneWeightsHandle InVertexHandle, const std::string& InBoneName)
			{
				return InVertexHandle->at(InBoneName);
			}
			float MeshBoneWeights::GetBoneWeight_ForVertex(VertexIndex InVertexIndex, const std::string& InBoneName)
			{
				return _vertexBoneWeights[InVertexIndex][InBoneName];
			}

			/*******************************************************************************************************
			* Material implementation
			*******************************************************************************************************/
			Material *Material::RegisterSubMaterial(const std::string& InSlotName)
			{
				auto it = _subMaterials.emplace(std::move(std::make_pair(InSlotName, new Material())));
				return it->second.get();
			}
			void Material::RegisterTexMap(const std::string& InSlotName, TexMapBase *pInTexMap)
			{
				auto it = _texMaps.emplace(std::move(std::make_pair(InSlotName, pInTexMap)));
			}

			/*******************************************************************************************************
			* Bitmap tex map implementation
			*******************************************************************************************************/
			BitmapTexMap *ToBitmapTexMap(TexMapBase *pInTexMap)
			{
				if (pInTexMap->GetClassName() == BITMAP_TEXMAP_CLASS_NAME)
				{
					return static_cast<BitmapTexMap  *>(pInTexMap);
				}
				else
				{
					return nullptr;
				}
			}

			/*******************************************************************************************************
			* Copy-paste from ver_0_0_0_0
			*******************************************************************************************************/
			boost::optional<Error> TryLoadVertices(std::ifstream *pStrm, File *pOutFile)
			{
				assert(pStrm);
				assert(pOutFile);

				uint32_t numVertices = 0;
				bool bCountLoaded = Dv::Util::TryLoadUInt32(pStrm, &numVertices);
				if (false == bCountLoaded)
				{
					const uint32_t countShouldBe = 0;
					const uint32_t countReallyLoaded = 0;
					return Error_EofWhileReadingVertices(countShouldBe, countReallyLoaded);
				}
				for (int i = 0; i < numVertices; i++)
				{
					Vertex vert;
					bool bVertexLoaded = TryLoadMeshVertex(*pStrm, &vert);
					if (false == bVertexLoaded)
					{
						// WARNING!!! Here "i" is written correctly (NOT "i+1")
						const uint32_t countReallyLoaded = i;
						if (pStrm->eof())
						{
							return Error_EofWhileReadingVertices(numVertices, countReallyLoaded);
						}
						else
						{
							return Error("Unknown error while reading vertices");
						}
					}
					pOutFile->AppendVertex(&vert);
				}
				return boost::optional<Error>();
			}

			boost::optional<Error> TryLoadFaces(std::ifstream *pStrm, File *pOutFile)
			{
				assert(pStrm);
				assert(pOutFile);

				uint32_t numFaces = 0;
				bool bCountLoaded = Dv::Util::TryLoadUInt32(pStrm, &numFaces);
				if (false == bCountLoaded)
				{
					const uint32_t countShouldBe = 0;
					const uint32_t countReallyLoaded = 0;
					return Error_EofWhileReadingFaces(countShouldBe, countReallyLoaded);
				}
				for (int i = 0; i < numFaces; i++)
				{
					Face face;
					bool bFaceLoaded = TryLoadFace(*pStrm, &face);
					if (false == bFaceLoaded)
					{
						// WARNING!!! Here "i" is written correctly (NOT "i+1")
						const uint32_t countReallyLoaded = i;
						return Error_EofWhileReadingFaces(numFaces, countReallyLoaded);
					}
					pOutFile->AppendFace(&face);
				}
				return boost::optional<Error>();
			}

			boost::optional<Error> TryLoadTextureVertices(std::ifstream *pStrm, File *pOutFile)
			{
				assert(pStrm);
				assert(pOutFile);

				uint32_t numVertices = 0;
				bool bCountLoaded = Dv::Util::TryLoadUInt32(pStrm, &numVertices);
				if (false == bCountLoaded)
				{
					const uint32_t countShouldBe = 0;
					const uint32_t countReallyLoaded = 0;
					return Error_EofWhileReadingTextureVertices(countShouldBe, countReallyLoaded);
				}
				for (int i = 0; i < numVertices; i++)
				{
					TextureVertex vert;
					bool bVertexLoaded = TryLoadTextureVertex(*pStrm, &vert);
					if (false == bVertexLoaded)
					{
						// WARNING!!! Here "i" is written correctly (NOT "i+1")
						const uint32_t countReallyLoaded = i;
						return Error_EofWhileReadingTextureVertices(numVertices, countReallyLoaded);
					}
					pOutFile->AppendTextureVertex(&vert);
				}
				return boost::optional<Error>();
			}

			boost::optional<Error> TryLoadTextureFaces(std::ifstream *pStrm, File *pOutFile)
			{
				assert(pStrm);
				assert(pOutFile);

				uint32_t numFaces = 0;
				bool bCountLoaded = Dv::Util::TryLoadUInt32(pStrm, &numFaces);
				if (false == bCountLoaded)
				{
					const uint32_t countShouldBe = 0;
					const uint32_t countReallyLoaded = 0;
					return Error_EofWhileReadingTextureFaces(countShouldBe, countReallyLoaded);
				}
				for (int i = 0; i < numFaces; i++)
				{
					TextureFace face;
					bool bFaceLoaded = TryLoadTextureFace(*pStrm, &face);
					if (false == bFaceLoaded)
					{
						// WARNING!!! Here "i" is written correctly (NOT "i+1")
						const uint32_t countReallyLoaded = i;
						return Error_EofWhileReadingTextureFaces(numFaces, countReallyLoaded);
					}
					pOutFile->AppendTextureFace(&face);
				}
				return boost::optional<Error>();
			}

			bool TryLoadTextureVertex(std::ifstream& strm, TextureVertex *pOutVertex)
			{
				Point3 uvw;
				bool bUVWloaded = TryLoadPoint3(strm, &uvw);
				pOutVertex->UVW = uvw; // It can be helpful when debugging to copy ever partly-loaded value
				if (false == bUVWloaded)
				{
					return false;
				}
				return true;
			}

			bool TryLoadTextureFace(std::ifstream& strm, TextureFace *pOutFace)
			{
				VertIndices3 vertIndices;
				bool bIndicesLoaded = TryLoadVertexIndices3(&strm, &vertIndices);
				pOutFace->textureVertexIndices[0] = vertIndices[0];
				pOutFace->textureVertexIndices[1] = vertIndices[1];
				pOutFace->textureVertexIndices[2] = vertIndices[2];
				if (false == bIndicesLoaded)
				{
					return false;
				}
				return true;
			}

			bool TryLoadMeshVertex(std::ifstream& strm, Vertex *pOutVertex)
			{
				assert(pOutVertex);
				Point3 position, normal;
				bool bLoadPositionSucceeded = TryLoadPoint3(strm, &position);
				if (false == bLoadPositionSucceeded)
				{
					return false;
				}
				bool bLoadNormalSucceeded = TryLoadPoint3(strm, &normal);
				if (false == bLoadNormalSucceeded)
				{
					return false;
				}
				pOutVertex->position = position;
				pOutVertex->normal = normal;
				return true;
			}

			bool TryLoadFace(std::ifstream& strm, Face *pOutFace)
			{
				assert(pOutFace);
				bool bMaterialID_loaded = Util::TryLoadUInt32(&strm, &(pOutFace->materialId));
				if (false == bMaterialID_loaded)
				{
					return false;
				}
				bool bNormalLoaded = TryLoadPoint3(strm, &(pOutFace->normal));
				if (false == bNormalLoaded)
				{
					return false;
				}
				bool bVertIndices_loaded = TryLoadVertexIndices3(&strm, &(pOutFace->vertexIndices));
				if (false == bVertIndices_loaded)
				{
					return false;
				}
				return true;
			}
		}
	}
}