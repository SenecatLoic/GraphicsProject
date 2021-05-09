#include "CollisionElement.h"

CollisionElement::CollisionElement(BodyElement* objectPunch, int countAnimation, float speed){
	this->objectPunch = objectPunch;
	this->countAnimation = countAnimation;
	this->speed = speed;
	this->baseCountAnimation = countAnimation;
}

void  CollisionElement::addCollision(BodyElement* object) {
	collisionObjs.push_back(object);
}

int CollisionElement::getCountAnimation() {
	return countAnimation;
}

int CollisionElement::getBaseCountAnimation() {
	return baseCountAnimation;
}

void CollisionElement::setCountAnimation(int n) {
	countAnimation = n;
}

float CollisionElement::getSpeed() {
	return speed;
}
std::vector<BodyElement*> CollisionElement::getCollisionObjs() {
	return collisionObjs;
}
BodyElement* CollisionElement::getObjectPunch(){
	return objectPunch;
}