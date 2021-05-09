#ifndef  Personnage_INC
#define  Personnage_INC
#include "BodyElement.h"
#include <glm/glm.hpp>
#include <stack>
#include <CollisionPerso.h>

class Personnage {

public:
	Personnage(BodyElement& body);
	void move(glm::vec3 mouvement);
	void draw(Shader* shader,std::stack<glm::mat4> stack,Camera camera,struct light light);
	void setArms(BodyElement* arms);
	void setForearms(BodyElement* forearms);
	void setThighs(BodyElement* thighs);
	void setLegs(BodyElement* legs);
	bool collision(GraphicObject &object, glm::vec3 mouvement = glm::vec3(0, 0, 0));
	bool collision(Personnage perso, glm::vec3 mouvement = glm::vec3(0, 0, 0));
	BodyElement* getBody();
	void setCollisionPerso(CollisionPerso* collision);

private: 
	BodyElement body;
	glm::vec3 position;
	BodyElement* arms;
	BodyElement* forearms;
	BodyElement* thighs;
	BodyElement* legs;
	CollisionPerso* collisionPerso;
};

#endif
