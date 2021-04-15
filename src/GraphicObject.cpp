#include "GraphicObject.h"

GraphicObject::GraphicObject(const Geometry& geometry, GLuint vao, glm::vec3 position) : Geometry(geometry)
{
	this->vao = vao;
	this->position = position;
}
