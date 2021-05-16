#include "Personnage.h"
#include <Camera.h>
#include <stack>
#include <queue>

Personnage::Personnage(BodyElement& body,CollisionPerso* collisionPerso)
{
	this->body = body;	
	this->collisionPerso = collisionPerso;
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

bool Personnage::collision(Personnage& perso, glm::vec3 mouvement) {
	BodyElement* current;
	std::queue<BodyElement*> file;


	BodyElement* other_perso;
	std::vector<BodyElement*> elements;

	file.push(&perso.body);
	elements.push_back(&perso.body);

	bool collision = false;



	if (body.collision(perso.body, mouvement) && !collisionPerso->contains(body)) {
		collision = true;
		CollisionElement elementCol(body, perso.body);
		collisionPerso->addCollision(elementCol);
	}
	
	while (!file.empty() && !collision) {
		other_perso = file.front();
		file.pop();

		for (BodyElement* child : other_perso->getChilds()) {
			if (body.collision(*child, mouvement)) {
				collision = true;
				CollisionElement elementCol(body, *child);
				collisionPerso->addCollision(elementCol);

				return collision;
			}
			elements.push_back(child);
			file.push(child);
		}
	}

	file.push(&body);

	while (!file.empty() && !collision) {
		current = file.front();
		file.pop();

		for (BodyElement* child : current->getChilds()) {
			//child collision
			for(BodyElement* persoElement : elements){
				if (child->collision(*persoElement, mouvement)) {
					collision = true;
					CollisionElement elementCol(*child, *persoElement);
					collisionPerso->addCollision(elementCol);
					//moche
					return collision;
				}
			}
			//push child
			file.push(child);
		}
	}

	return collision;
}

void Personnage::walk(float time) {
	
		// arm_left
		body.getChilds()[2]->translate(glm::vec3(0, body.getChilds()[2]->getWidth(), 0));
		body.getChilds()[2]->rotate(time, glm::vec3(0.1, 0, 0));
		body.getChilds()[2]->translate(glm::vec3(0, -body.getChilds()[2]->getWidth(), 0));

		// arm_right
		body.getChilds()[3]->translate(glm::vec3(0, body.getChilds()[3]->getWidth(), 0));
		body.getChilds()[3]->rotate(time, glm::vec3(-0.1, 0, 0));
		body.getChilds()[3]->translate(glm::vec3(0, -body.getChilds()[3]->getWidth(), 0));

		// hip_left
		body.getChilds()[4]->translate(glm::vec3(0, body.getChilds()[4]->getWidth(), 0));
		body.getChilds()[4]->rotate(time, glm::vec3(-0.1, 0, 0));
		body.getChilds()[4]->translate(glm::vec3(0, -body.getChilds()[4]->getWidth(), 0));

		// hip_right
		body.getChilds()[5]->translate(glm::vec3(0, body.getChilds()[5]->getWidth(), 0));
		body.getChilds()[5]->rotate(time, glm::vec3(0.1, 0, 0));
		body.getChilds()[5]->translate(glm::vec3(0, -body.getChilds()[5]->getWidth(), 0));
	
}

// Mettre le perso en garde
void Personnage::enGarde(float t, float duree_garde, float t_enGarde) {
	if (t >= t_enGarde && t < t_enGarde + duree_garde) {
		// arm_left
		body.getChilds()[2]->translate(glm::vec3(0, body.getChilds()[2]->getWidth(), 0));
		body.getChilds()[2]->rotate(-1 / duree_garde, glm::vec3(0.2, 0.1, 0));
		body.getChilds()[2]->translate(glm::vec3(0, -body.getChilds()[2]->getWidth(), 0));

		// arm_right
		body.getChilds()[3]->translate(glm::vec3(0, body.getChilds()[3]->getWidth(), 0));
		body.getChilds()[3]->rotate(1 / duree_garde, glm::vec3(-0.2, 0.1, 0));
		body.getChilds()[3]->translate(glm::vec3(0, -body.getChilds()[3]->getWidth(), 0));

		// elbow_left
		body.getChilds()[2]->getChilds()[0]->rotate(-1.2 / duree_garde, glm::vec3(0.1, 0, 0));

		// elbow_right
		body.getChilds()[3]->getChilds()[0]->rotate(1.2 / duree_garde, glm::vec3(-0.1, 0, 0));
	}
}

// Baisser la garde
bool Personnage::baisseEnGarde(float t, float duree_garde, float t_baisseGarde) {
	// à la fin des coups, on enlève sa garde
	if (t >= t_baisseGarde && t < t_baisseGarde + duree_garde) {
		// arm_left
		body.getChilds()[2]->translate(glm::vec3(0, body.getChilds()[2]->getWidth(), 0));
		body.getChilds()[2]->rotate((1 / duree_garde)-0.014, glm::vec3(0.2, 0.1, 0));
		body.getChilds()[2]->translate(glm::vec3(0, -body.getChilds()[2]->getWidth(), 0));

		// arm_right
		body.getChilds()[3]->translate(glm::vec3(0, body.getChilds()[3]->getWidth(), 0));
		body.getChilds()[3]->rotate(-1 / duree_garde, glm::vec3(-0.2, 0.1, 0));
		body.getChilds()[3]->translate(glm::vec3(0, -body.getChilds()[3]->getWidth(), 0));

		// elbow_left
		body.getChilds()[2]->getChilds()[0]->rotate(1.2 / duree_garde, glm::vec3(0.1, 0, 0));

		// elbow_right
		body.getChilds()[3]->getChilds()[0]->rotate(-1.2 / duree_garde, glm::vec3(-0.1, 0, 0));
		return true;
	}else {
		return false;
	}
}

// Faire boxer le perso un certain nombre de fois
int Personnage::boxe(float t, float duree_coup, float t_boxe, int nb_coups, int nb_coups_elapsed) {
	if (t >= t_boxe && nb_coups_elapsed < nb_coups) {

		if (t_boxe + nb_coups_elapsed * 4 * duree_coup <= t && t < t_boxe + nb_coups_elapsed * 4 * duree_coup + duree_coup) {
			// arm_left
			body.getChilds()[2]->translate(glm::vec3(0, body.getChilds()[2]->getWidth(), 0.1));
			body.getChilds()[2]->rotate(-0.8 / duree_coup, glm::vec3(0.1, 0, 0));
			body.getChilds()[2]->translate(glm::vec3(0, -body.getChilds()[2]->getWidth(), -0.1));

			// elbow_left
			body.getChilds()[2]->getChilds()[0]->rotate(1.7 / duree_coup, glm::vec3(0.1, 0, 0));
		}
		if (t_boxe + nb_coups_elapsed * 4 * duree_coup + duree_coup <= t && t < t_boxe + nb_coups_elapsed * 4 * duree_coup + 2 * duree_coup) {
			// arm_left
			body.getChilds()[2]->translate(glm::vec3(0, body.getChilds()[2]->getWidth(), 0.1));
			body.getChilds()[2]->rotate(0.8 / duree_coup, glm::vec3(0.1, 0, 0));
			body.getChilds()[2]->translate(glm::vec3(0, -body.getChilds()[2]->getWidth(), -0.1));

			// elbow_left
			body.getChilds()[2]->getChilds()[0]->rotate(-1.7 / duree_coup, glm::vec3(0.1, 0, 0));
		}
		if (t_boxe + nb_coups_elapsed * 4 * duree_coup + 2 * duree_coup <= t && t < t_boxe + nb_coups_elapsed * 4 * duree_coup + 3 * duree_coup) {
			// arm_right
			body.getChilds()[3]->translate(glm::vec3(0, body.getChilds()[3]->getWidth(), 0.1));
			body.getChilds()[3]->rotate(0.8 / duree_coup, glm::vec3(-0.1, 0, 0));
			body.getChilds()[3]->translate(glm::vec3(0, -body.getChilds()[3]->getWidth(), -0.1));

			// elbow_right
			body.getChilds()[3]->getChilds()[0]->rotate(1.7 / duree_coup, glm::vec3(0.1, 0, 0));
		}
		if (t_boxe + nb_coups_elapsed * 4 * duree_coup + 3 * duree_coup <= t && t < t_boxe + nb_coups_elapsed * 4 * duree_coup + 4 * duree_coup) {
			// arm_right
			body.getChilds()[3]->translate(glm::vec3(0, body.getChilds()[3]->getWidth(), 0.1));
			body.getChilds()[3]->rotate(-0.8 / duree_coup, glm::vec3(-0.1, 0, 0));
			body.getChilds()[3]->translate(glm::vec3(0, -body.getChilds()[3]->getWidth(), -0.1));

			// elbow_right
			body.getChilds()[3]->getChilds()[0]->rotate(-1.7 / duree_coup, glm::vec3(0.1, 0, 0));
		}
		if (t == t_boxe + nb_coups_elapsed * 4 * duree_coup + 4 * duree_coup) {
			nb_coups_elapsed += 1;
		}
	}
	return nb_coups_elapsed;
}