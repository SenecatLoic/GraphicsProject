#ifndef  CollisionElement_INC
#define  CollisionElement_INC
#include "BodyElement.h"
#include <glm/glm.hpp>

class CollisionElement {

public:
	CollisionElement(BodyElement* objectPunch, int countAnimation = 30, float speed = 0.01f);
	void addCollision(BodyElement* object);
	int getCountAnimation();
	int getBaseCountAnimation();
	void setCountAnimation(int n);
	float getSpeed();
	std::vector<BodyElement*> getCollisionObjs();
	BodyElement* getObjectPunch();
private:
	// objet en mouvement qui arrive sur d'autres objets 
	BodyElement* objectPunch;
	//objet qui on reçu le coup
	std::vector<BodyElement*> collisionObjs;
	//Nombre de pas pour l'animation
	int countAnimation;
	int baseCountAnimation;
	float speed;
};

#endif
