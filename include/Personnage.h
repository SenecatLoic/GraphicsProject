#ifndef  Personnage_INC
#define  Personnage_INC
#include <BodyElement.h>
#include <glm/glm.hpp>

class Personnage {

public:
	Personnage(BodyElement& body, glm::vec3 position = glm::vec3(0,1,0));
	void move(glm::vec3 mouvement);

private: 
	BodyElement body;
	glm::vec3 position;

};

#endif
