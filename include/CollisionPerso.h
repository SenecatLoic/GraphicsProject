#ifndef  CollisionPerso_INC
#define  CollisionPerso_INC

#include "CollisionElement.h"
#include <glm/glm.hpp>
#include <vector>

class CollisionPerso {

public:
	CollisionPerso();
	void addCollision(CollisionElement collision);

	void computeAnimation();
private:
	//objet en collision à animer
	std::vector<CollisionElement> collisions;
};

#endif