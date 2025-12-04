#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "graphics/mesh/mesh.h"
#include "graphics/shader/shader.h"

#include <string>
#include <map>
#include <vector>

#include "util/common.h"
WARNING_DISABLE
#include "bullet/BulletCollision/CollisionShapes/btBoxShape.h"
#include "bullet/BulletCollision/CollisionShapes/btCapsuleShape.h"
WARNING_ENABLE
#include "util/assimpGLMHelpers.h"
#include "graphics/animData/animData.h"
#include "math/vector3.h"

using namespace std;

// Referenced from LearnOpenGL.com

class SkeletalModel 
{
public:
    // constructor, expects a filepath to a 3D model.
    SkeletalModel(string const &path, bool gamma = false);

    // draws the model, and thus all its meshes
    void Draw() const;

    std::map<string, BoneInfo>& GetBoneInfoMap();
    int& GetBoneCount();

    btBoxShape* GetBoxCollider() const;

    btCapsuleShape* GetCapsuleCollider() const;


	// model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path);

    void CalculateBoundingBox(const aiScene* scene);
    static void CalculateBoundingBoxForNode(const aiNode* node, const aiScene* scene, aiMatrix4x4 transform,
                                            Vector3& minExtents, Vector3& maxExtents);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(const aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    static void SetVertexBoneDataToDefault(Vertex& vertex);
    static void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, const aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> LoadMaterialTextures(const aiMaterial *mat, aiTextureType type, const string& typeName);

	std::map<string, BoneInfo> boneInfoMap;
	int boneCounter = 0;
	Vector3 scale = Vector3(1,1,1);
	btBoxShape* boxCollider;
	btCapsuleShape* capsuleCollider;
};
