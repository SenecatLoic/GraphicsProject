#include "Personnage.h"
#include <Camera.h>
#include <stack>
#include <queue>

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

void Personnage::setArms(BodyElement* arms) {
	this->arms = arms;
}
void Personnage::setForearms(BodyElement* forearms){
	this->forearms = forearms;
}
void Personnage::setThighs(BodyElement* thighs){
	this->thighs = thighs;
}
void Personnage::setLegs(BodyElement* legs){
	this->legs = legs;
}

void Personnage::setCollisionPerso(CollisionPerso* collision) {
	collisionPerso = collision;
}

BodyElement* Personnage::getBody() {
	return &body;
}

bool Personnage::collision(GraphicObject &object,glm::vec3 mouvement) {
	return body.collision(object,mouvement);
}

bool Personnage::collision(Personnage perso, glm::vec3 mouvement) {
	BodyElement* current;
	std::queue<BodyElement*> file;
	BodyElement* other_perso;
	std::vector<BodyElement*> elements;

	file.push(&perso.body);
	elements.push_back(&perso.body);
	//si la distance entre les 2 persos est trop loin on s'embête pas à tester la collision sur chaque élément des persos
	bool collision = false;

	CollisionElement elementCol(&body);

	if (body.collision(perso.body, mouvement)) {
		collision = true;
		elementCol.addCollision(&perso.body);
	}
	
	while (!file.empty()) {
		other_perso = file.front();
		file.pop();

		for (BodyElement* child : other_perso->getChilds()) {
			if (body.collision(*child, mouvement)) {
				collision = true;
				elementCol.addCollision(child);
			}
			elements.push_back(child);
			file.push(child);
		}
	}

	if (!elementCol.getCollisionObjs().empty()) {
		collisionPerso->addCollision(elementCol);
	}

	file.push(&body);

	while (!file.empty()) {
		current = file.front();
		file.pop();

		for (BodyElement* child : current->getChilds()) {
			CollisionElement collisionElement(child);
			//child collision
			for(BodyElement* persoElement : elements){
				if (child->collision(*persoElement, mouvement)) {
					collision = true;
					collisionElement.addCollision(persoElement);
				}
			}
			//on rajoute la collision dans le gestionnaire
			if (!collisionElement.getCollisionObjs().empty()) {
				collisionPerso->addCollision(collisionElement);
			}
			//push child
			file.push(child);
		}
	}

	return collision;
}