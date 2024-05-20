#include "skeletalModel.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "engine/util/util.h"
WARNING_DISABLE
#include <LinearMath/btVector3.h>
WARNING_ENABLE

SkeletalModel::SkeletalModel(string const& path, bool gamma): gammaCorrection(gamma)
{
	loadModel(path);
}

void SkeletalModel::Draw() const
{
	for (const auto& mesh : meshes)
	{
		mesh.Draw(*Util::GetBasicShader());
	}
}

std::map<string, BoneInfo>& SkeletalModel::GetBoneInfoMap()
{
	return boneInfoMap;
}

int& SkeletalModel::GetBoneCount()
{
	return boneCounter;
}

btBoxShape* SkeletalModel::GetBoxCollider() const
{
	return boxCollider;
}

btCapsuleShape* SkeletalModel::GetCapsuleCollider() const
{
	return capsuleCollider;
}

void SkeletalModel::loadModel(string const& path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	constexpr unsigned int flags = aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_ValidateDataStructure |
		aiProcess_OptimizeMeshes |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ImproveCacheLocality;

	const aiScene* scene = importer.ReadFile(path, flags);

	// check for errors
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
	CalculateBoundingBox(scene);
}

void SkeletalModel::CalculateBoundingBox(const aiScene* scene)
{
	const aiMatrix4x4 identityMatrix;
	Vector3 minExtents(FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3 maxExtents(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	CalculateBoundingBoxForNode(scene->mRootNode, scene, identityMatrix, minExtents, maxExtents);

	std::cout << "Bounding Box Min: (" << minExtents.x << ", " << minExtents.y << ", " << minExtents.z << ")" << std::endl;
	std::cout << "Bounding Box Max: (" << maxExtents.x << ", " << maxExtents.y << ", " << maxExtents.z << ")" << std::endl;

	const Vector3 halfExtents = (maxExtents - minExtents) * 0.5f * scale;
	boxCollider = new btBoxShape(btVector3(halfExtents));
	capsuleCollider = new btCapsuleShape(halfExtents.x/2, halfExtents.y);
}

void SkeletalModel::CalculateBoundingBoxForNode(const aiNode* node, const aiScene* scene, aiMatrix4x4 transform,
	Vector3& minExtents, Vector3& maxExtents)
{
	transform *= node->mTransformation;

	for (unsigned int m = 0; m < node->mNumMeshes; ++m) 
	{
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[m]];
		for (unsigned int v = 0; v < mesh->mNumVertices; ++v) 
		{
			aiVector3D vertex = mesh->mVertices[v];
			vertex *= transform;

			minExtents.x = std::min(minExtents.x, vertex.x);
			minExtents.y = std::min(minExtents.y, vertex.y);
			minExtents.z = std::min(minExtents.z, vertex.z);
			maxExtents.x = std::max(maxExtents.x, vertex.x);
			maxExtents.y = std::max(maxExtents.y, vertex.y);
			maxExtents.z = std::max(maxExtents.z, vertex.z);
		}
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) 
	{
		CalculateBoundingBoxForNode(node->mChildren[i], scene, transform, minExtents, maxExtents);
	}
}

void SkeletalModel::processNode(const aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all the meshes (if any) we then recursively process each of the children nodes
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

void SkeletalModel::SetVertexBoneDataToDefault(Vertex& vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.m_BoneIDs[i] = -1;
		vertex.m_Weights[i] = 0.0f;
	}
}

Mesh SkeletalModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		SetVertexBoneDataToDefault(vertex);
		vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
		vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);
			
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	ExtractBoneWeightForVertices(vertices,mesh,scene);

	return Mesh(vertices, indices, textures);
}

void SkeletalModel::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (vertex.m_BoneIDs[i] < 0)
		{
			vertex.m_Weights[i] = weight;
			vertex.m_BoneIDs[i] = boneID;
			break;
		}
	}
}

void SkeletalModel::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, const aiMesh* mesh, const aiScene* scene)
{
	int& boneCount = boneCounter;

	for (unsigned boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCount;
			newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCount;
			boneCount++;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
		}
		assert(boneID != -1);
		const auto weights = mesh->mBones[boneIndex]->mWeights;
		const unsigned numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (unsigned weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			const unsigned vertexId = weights[weightIndex].mVertexId;
			const float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

vector<Texture> SkeletalModel::LoadMaterialTextures(const aiMaterial* mat, const aiTextureType type, const string& typeName)
{
	vector<Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (auto& j : textures_loaded)
		{
			if(std::strcmp(j.path.data(), str.C_Str()) == 0)
			{
				textures.push_back(j);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if(!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = Util::TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessarily load duplicate textures.
		}
	}
	return textures;
}
