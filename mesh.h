#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"
#include "shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh {
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
	std::vector<Texture> textures;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

public:
	Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<Texture> t);
	void Draw(ShaderProgram shader);
};

Mesh::Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<Texture> t) {
	vertices = v;
	indices = i;
	textures = t;
	setupMesh();
}

// set vao, vbo, ebo
void Mesh::setupMesh() {
	// vao
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// vbo
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	//ebo
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

// before draw, ensure shader material correspond to mesh textures
void Mesh::Draw(ShaderProgram shader) {
	// bind tex to shader
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	for (int i = 0; i < textures.size(); i++) {
		// activate proper texture unit before binding
		glActiveTexture(GL_TEXTURE0 + i);
		if (textures[i].attrType == Texture::DIFFUSE) {
			diffuseNr++;
			unsigned int texUnit = textures[i].GetTexUnit();
			shader.SetInt("material.texture_diffuse" + std::to_string(diffuseNr), texUnit);
		}
		else if (textures[i].attrType == Texture::SPECULAR) {
			specularNr++;
			unsigned int texUnit = textures[i].GetTexUnit();
			shader.SetInt("material.texture_specular" + std::to_string(specularNr), texUnit);
		}
		textures[i].BindTexUnit(i);
	}
	glActiveTexture(GL_TEXTURE0);
	// draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


#endif