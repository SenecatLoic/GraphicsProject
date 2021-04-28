#include "Personnage.h"
#include <Camera.h>
#include <stack>
Personnage::Personnage(BodyElement& body)
{
	this->body = body;
}

void Personnage::move(glm::vec3 mouvement) {
	body.translate(mouvement);
}

void Personnage::draw(Shader* shader,std::stack<glm::mat4> stack,Camera camera, struct light light) {
	body.draw(shader,stack,body.getModel(),camera, light);
}