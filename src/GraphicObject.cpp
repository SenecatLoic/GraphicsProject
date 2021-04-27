#include "GraphicObject.h"

GraphicObject::GraphicObject(const Geometry& geometry, GLuint vao, glm::vec3 position,struct material material) : Geometry(geometry)
{
	this->vao = vao;
	this->position = position;
	this->material = material;
	this->model = glm::mat4(1.f);
	this->localTransformation = glm::mat4(1.f);
}

GraphicObject::GraphicObject() {

}

void GraphicObject::translate(glm::vec3 move) {
	model = glm::translate(model,move);
}

void GraphicObject::rotate(float angle, glm::vec3 axes) {
	model = glm::rotate(model,angle,axes);
}

void GraphicObject::scale(glm::vec3 v) {
	localTransformation = glm::scale(localTransformation,v);
}