#ifndef  Particle_INC
#define  Particle_INC
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

//GML libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include "Geometry.h"
#include "Shader.h"
#include "GraphicObject.h"

class Particle : public GraphicObject {

public:
	Particle();
	Particle(Geometry geometry, GLuint vao, glm::vec3 position, struct material material);
	void draw(Shader* shader, std::stack<glm::mat4> stack, glm::mat4 parentModel, Camera camera, struct light light);

};

#endif