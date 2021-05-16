#include "CollisionElement.h"

CollisionElement::CollisionElement(BodyElement& objectPunch, BodyElement& object, int countAnimation,int countParticle, float speed):
	objectPunch(objectPunch), other_object(object){
	this->objectPunch = objectPunch;
	this->countAnimation = countAnimation;
	this->speed = speed;
	this->baseCountAnimation = countAnimation;
	this->countParticle = countParticle;
}

CollisionElement& CollisionElement::operator=(const CollisionElement& co) {
	CollisionElement c(co.objectPunch, co.other_object);
	
	return c;
}

void  CollisionElement::setCollision(BodyElement& object) {
	other_object = object;
}

int CollisionElement::getCountAnimation() {
	return countAnimation;
}

int CollisionElement::getBaseCountAnimation() {
	return baseCountAnimation;
}

int CollisionElement::getCountParticle() {
	return countParticle;
}

void CollisionElement::setCountAnimation(int n) {
	countAnimation = n;
}

void CollisionElement::setCountParticle(int n) {
	countParticle = n;
}

float CollisionElement::getSpeed() {
	return speed;
}
BodyElement& CollisionElement::getCollisionObj() {
	return other_object;
}
BodyElement& CollisionElement::getObjectPunch(){
	return objectPunch;
}