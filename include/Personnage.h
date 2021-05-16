#ifndef  Personnage_INC
#define  Personnage_INC
#include "BodyElement.h"
#include <glm/glm.hpp>
#include <stack>
#include <CollisionPerso.h>

class Personnage {

public:
	Personnage(BodyElement& body, CollisionPerso* collisionPerso );
	void move(glm::vec3 mouvement);
	void draw(Shader* shader,std::stack<glm::mat4> stack,Camera camera,struct light light);
	void setArms(BodyElement* arms);
	void setForearms(BodyElement* forearms);
	void setThighs(BodyElement* thighs);
	void setLegs(BodyElement* legs);
	bool collision(GraphicObject &object, glm::vec3 mouvement = glm::vec3(0, 0, 0));
	bool collision(Personnage& perso, glm::vec3 mouvement = glm::vec3(0, 0, 0));
	BodyElement* getBody();
	void setCollisionPerso(CollisionPerso* collision);
	void walk(float time);
	void enGarde(float t, float duree_garde, float t_enGarde);
	bool baisseEnGarde(float t, float duree_garde, float t_enGarde);
	int boxe(float t, float speed, float t_boxe, int nb_coups, int nb_coups_elapsed);
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
