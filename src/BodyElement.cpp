#include "BodyElement.h"

BodyElement::BodyElement() {

}

BodyElement::BodyElement(Geometry geometry, GLuint vao, glm::vec3 position)
{
	this->vao = vao;
	this->position = position;
	this->m_nbVertices = geometry.getNbVertices();
	this->m_normals = (float*)geometry.getNormals();
	this->m_vertices = (float*)geometry.getVertices();
	this->m_uvs = (float*)geometry.getUVs();

}

void BodyElement::draw(Shader shader) {

}