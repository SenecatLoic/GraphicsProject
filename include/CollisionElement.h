#ifndef  CollisionElement_INC
#define  CollisionElement_INC
#include "BodyElement.h"
#include <glm/glm.hpp>

class CollisionElement {

public:
	CollisionElement(BodyElement& objectPunch, BodyElement& object, int countAnimation = 5,int countParticle = 10, float speed = 0.01f);
	void setCollision(BodyElement& object);
	int getCountAnimation();
	int getCountParticle();
	int getBaseCountAnimation();
	void setCountAnimation(int n);
	void setCountParticle(int n);
	float getSpeed();
	BodyElement& getCollisionObj();
	BodyElement& getObjectPunch();
	CollisionElement& operator=(const CollisionElement&);
private:
	// objet en mouvement qui arrive sur d'autres objets 
	BodyElement& objectPunch;
	//objet qui on reçu le coup
	BodyElement& other_object;
	//Nombre de pas pour l'animation
	int countAnimation;
	int baseCountAnimation;
	int countParticle;
	float speed;
};

#endif
