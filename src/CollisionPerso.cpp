#include "CollisionPerso.h"
#include <Cube.h>

CollisionPerso::CollisionPerso(){}

void CollisionPerso::addCollision(CollisionElement& collision) {
	collisions.push_back(collision);
}

void CollisionPerso::setParticles(std::vector<Particle>& particles) {
	this->particles = particles;
}

bool CollisionPerso::contains(const BodyElement element) {
	bool result = false;
	for(CollisionElement& collision :collisions)
	{
		if (collision.getObjectPunch().id == element.id) {
			result = true;
			break;
		}
	}
	return result;
}
 
void CollisionPerso::computeAnimation() {
	std::vector<int> deletes;
	int index = 0;
	
	for (CollisionElement& element: collisions) {
		//direction du coup
		int count = element.getCountAnimation();
		float angle;

		if (count < 0) {
			angle = -0.02;
		}else {
			angle = 0.02;
		}

		BodyElement& other_element = element.getCollisionObj();
				
		glm::vec3 difference = other_element.getPosition() - element.getObjectPunch().getPosition();
		glm::vec3 direction(0.f);
		float width = 0;

		if (abs(difference.z) > abs(difference.x) && abs(difference.z) > abs(difference.y)) {
			direction = glm::vec3(1, 0, 0);
		}else {
			direction = glm::vec3(0, 0, 1);
			if (other_element.getPosition().x < element.getObjectPunch().getPosition().x) {
				width = -other_element.getWidth();
			}
			else {
				width = other_element.getWidth();
				angle *= -1.f;
			}
		}
			
		other_element.translate(glm::vec3(width, other_element.getHeight(), 0));
		other_element.rotate(-angle, direction);								
		other_element.translate(glm::vec3(width * -1.f, -other_element.getHeight(), 0));
				
		count--;
		
		if (count <= -element.getBaseCountAnimation()) {
			deletes.push_back(index);
			
		}else{ 			
			element.setCountAnimation(count); 
		}
		index++;
	}
	
	int suppr = 0;
	//on enlève toute les animations fini
	for (int i : deletes) {

		collisions.erase(collisions.begin() + i - suppr);
		suppr++;
	}
}

void CollisionPerso::computeParticle(Shader* shader, std::stack<glm::mat4> stack, Camera camera, struct light light) {

	for (CollisionElement& element : collisions) {		
		if (element.getCountParticle() > 0) {
			BodyElement object_punch = element.getObjectPunch();
			BodyElement other_object = element.getCollisionObj();
			
			glm::mat4 transform(1.f);
			transform = glm::translate(transform, glm::vec3(object_punch.getPosition().x, 
				object_punch.getPosition().y-1, object_punch.getPosition().z));
			stack.push(stack.top() * transform);
		
			if (object_punch.getPosition().x < other_object.getPosition().x) {
			}
			else {

			}

			if (object_punch.getPosition().z < other_object.getPosition().z) {

			}

			for (Particle particle:particles) {
				particle.draw(shader,stack,glm::mat4(1.f),camera,light);
				
			}
			
			element.setCountParticle(element.getCountParticle() - 1);
		}
	}
}