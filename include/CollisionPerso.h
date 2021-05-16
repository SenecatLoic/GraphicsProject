#ifndef  CollisionPerso_INC
#define  CollisionPerso_INC

#include "CollisionElement.h"
#include "Particle.h"
#include <glm/glm.hpp>

class CollisionPerso {

public:
	CollisionPerso();
	void addCollision(CollisionElement& collision);
	bool contains(const BodyElement element);
	void computeAnimation();
	void computeParticle(Shader* shader, std::stack<glm::mat4> stack, Camera camera, struct light light);
	void setParticles(std::vector<Particle>& particles);
private:
	//objet en collision à animer
	std::vector<CollisionElement> collisions;
	std::vector<Particle> particles;
	
};

#endif