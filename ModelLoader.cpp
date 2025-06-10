#include "ModelLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

bool ModelLoader::loadModel(const std::string &path)
{
    vertices.clear();
    normals.clear();
    indices.clear();

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        std::cerr << "Assimp Fehler: " << importer.GetErrorString() << std::endl;
        return false;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[i];
        processMesh(mesh);
    }

    std::cout << "Model geladen: " << vertices.size() << " Vertices, "
              << normals.size() << " Normalen, "
              << indices.size() << " Indices." << std::endl;

    return true;
}

void ModelLoader::processMesh(aiMesh *mesh)
{
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        aiVector3D v = mesh->mVertices[i];
        vertices.push_back(glm::vec3(v.x, v.y, v.z));

        if (mesh->HasNormals())
        {
            aiVector3D n = mesh->mNormals[i];
            normals.push_back(glm::vec3(n.x, n.y, n.z));
        }
        else
        {
            normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // Standard-Normale
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
}