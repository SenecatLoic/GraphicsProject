#include "CollisionPerso.h"

CollisionPerso::CollisionPerso(){}

void CollisionPerso::addCollision(CollisionElement collision) {
	collisions.push_back(collision);
}

void CollisionPerso::computeAnimation() {
	std::vector<int> deletes;
	int index = 0;
	
	for (CollisionElement& element: collisions) {
		//direction du coup
		glm::vec3 direction = element.getObjectPunch()->getPosition();
		int count = element.getCountAnimation();
		float angle;
		if (count < 0) {
			angle = -0.02;
		}else {
			angle = 0.02;
		}

		for (BodyElement* bodyElement:element.getCollisionObjs()) {
			bodyElement->translate(glm::vec3(-bodyElement->getWidth(),bodyElement->getHeight(),0));
			bodyElement->rotate(angle,glm::vec3(0,0,1));
			bodyElement->translate(glm::vec3(bodyElement->getWidth(), -bodyElement->getHeight(), 0));
		}
		count--;
		
		if (count <= -element.getBaseCountAnimation()) {
			deletes.push_back(index);
		}else{ 			
			element.setCountAnimation(count); 
			
		}
		
		index++;
	}

	//on enlève toute les animations fini
	for (int index : deletes) {
		collisions.erase(collisions.begin() + index);
	}
}
