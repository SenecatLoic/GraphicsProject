#ifndef  Personnage_INC
#define  Personnage_INC
#include "BodyElement.h"
#include <glm/glm.hpp>
#include <stack>

class Personnage {

public:
	Personnage(BodyElement& body);
	void move(glm::vec3 mouvement);
	void draw(Shader* shader,std::stack<glm::mat4> stack,Camera camera,struct light light);

private: 
	BodyElement body;
	glm::vec3 position;

};

#endif
