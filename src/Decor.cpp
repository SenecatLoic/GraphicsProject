#include "Decor.h"

Decor::Decor(const Geometry& geometry,GLuint vao, glm::vec3 position, struct material material) 
	: GraphicObject(geometry,vao,position,material)
{

}

void Decor::draw(Shader shader) {

}
