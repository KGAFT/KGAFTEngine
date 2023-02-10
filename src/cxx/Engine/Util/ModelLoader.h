//
// Created by Daniil on 05.02.2023.
//
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include <vector>

#include "../GraphicalObjects/Mesh.h"


using namespace std;
class ModelLoader{
private:
    vector<Mesh*> meshes;
    string directory;
    VulkanDevice* device;
private:
    void processNode(aiNode *node, const aiScene *scene){
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }
    Mesh* processMesh(aiMesh *mesh, const aiScene *scene)
    {

        vector<unsigned int> indices;
        vector<float> data;
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            data.push_back(mesh->mVertices[i].x);
            data.push_back(mesh->mVertices[i].y);
            data.push_back(mesh->mVertices[i].z);
            data.push_back(mesh->mNormals[i].x);
            data.push_back(mesh->mNormals[i].y);
            data.push_back(mesh->mNormals[i].z);

            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                data.push_back(mesh->mTextureCoords[0][i].x);
                data.push_back(mesh->mTextureCoords[0][i].y);
            }
        }
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        VertexBuffer* vbo = new VertexBuffer(data.size()/8, sizeof(float), 8, device, data.data());
        IndexBuffer* ibo = new IndexBuffer(device, indices.data(), indices.size());

        return new Mesh(vbo, ibo);
    }
public:
    ModelLoader(VulkanDevice *device) : device(device) {}

    std::vector<Mesh*> loadModel(string path){
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
            return std::vector<Mesh*>();
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
        return meshes;
    }


    void clear()
    {
        meshes.clear();
        directory = "";
    }
};