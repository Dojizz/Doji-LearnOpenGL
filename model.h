#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"
#include "texture.h"

class Model {
private:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	std::string directory;
	void loadModel(const std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);

public:
	Model(const std::string path) {
		loadModel(path);
	}
	// 每个mesh可能texture不同, 用一个相同的shader可能不妥
	void Draw(ShaderProgram shader);
};

void Model::Draw(ShaderProgram shader) {
	for (int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Model::loadModel(const std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// 处理顶点位置、法线和纹理坐标
		vertex.position = glm::vec3(mesh->mVertices[i].x, 
									mesh->mVertices[i].y,
									mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, 
								mesh->mNormals[i].y,
								mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.f, 0.f);
		vertices.push_back(vertex);
	}
	// 处理索引
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	
	// 处理材质
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		// fetch texture file name
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string texName = str.C_Str();
		std::string texPath = directory + '/' + texName; // the texture is in the same directory as the model

		// find if texture is already loaded
		bool skip = false;
		for (int j = 0; j < textures_loaded.size(); j++) {
			if (textures_loaded[j].dataPath == texPath) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if(!skip){
			std::string suffix = texName.substr(texName.find_last_of('.') + 1);
			Texture texture(Texture::TEX2D, suffix == "png" ? Texture::PNG : Texture::JPG, texPath);
			if (typeName == "texture_diffuse")
				texture.attrType = Texture::DIFFUSE;
			else if (typeName == "texture_specular")
				texture.attrType = Texture::SPECULAR;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

#endif