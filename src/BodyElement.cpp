#include "BodyElement.h"

BodyElement::BodyElement() {
}

BodyElement::BodyElement(Geometry geometry, GLuint vao, glm::vec3 position) : GraphicObject(geometry,vao,position)
{
}

void BodyElement::draw(Shader shader) {

}