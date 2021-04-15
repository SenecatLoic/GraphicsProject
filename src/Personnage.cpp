#include "Personnage.h"

Personnage::Personnage(BodyElement& body, glm::vec3 position = glm::vec3(0, 1, 0))
{
	this->body = body;
	this->position = position;

}

void Personnage::move(glm::vec3 mouvement) {

}