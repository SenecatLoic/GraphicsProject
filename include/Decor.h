#ifndef  Decor_INC
#define  Decor_INC

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Geometry.h"
#include <Shader.h>

class Decor : public Geometry {
public:
	Decor(Geometry geometry, GLuint vao, glm::vec3 position);
	void draw(Shader shader);
private:
	glm::vec3 position;
	GLuint vao;
};

#endif