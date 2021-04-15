#include "Decor.h"

Decor::Decor(const Geometry& geometry, GLuint vao, glm::vec3 position) : Geometry(geometry)
{
	this->vao = vao;
	this->position = position;
	/*this->m_nbVertices = geometry.getNbVertices();
	this->m_normals = (float*)geometry.getNormals();
	this->m_vertices = (float*)geometry.getVertices();
	this->m_uvs = (float*)geometry.getUVs();*/
}

void Decor::draw(Shader shader) {

}
