#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../mesh/mesh.h"
#include "../shader/shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include "math/vector3.h"
#include "util/util.h"

class btConvexHullShape;
using namespace std;

// Referenced from LearnOpenGL.com

class StaticModel
{
public:
    // constructor, expects a filepath to a 3D model.
	StaticModel(string const& path, Vector3 scale = Vector3(1), Vector3 offset = Vector3(), Vector3 rot = Vector3(),
	            Vector2 texScale = Vector2());
    void Draw(Shader& shader) const;
    void SetupInstancedMesh(const vector<glm::mat4>& mats) const;
    void DrawInstanced(size_t size) const;
    btConvexHullShape* GetBoxCollider() const;


    Vector3 rot = Vector3(0, 0, 0);
    Vector3 scale = Vector3(1, 1, 1);
    Vector3 offset = Vector3(0, 0, 0);
    Vector2 textureScale = Vector2(1, 1);

private:
    void loadModel(string const& path);
    void CalculateBoundingBox(const aiScene* scene);
    void CalculateBoundingBoxForNode(const aiNode* node, const aiScene* scene, aiMatrix4x4 transform, std::vector<btVector3>& vertices);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);


    btConvexHullShape* convexCollider = nullptr;
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
};
