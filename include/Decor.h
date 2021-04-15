#ifndef  Decor_INC
#define  Decor_INC

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Geometry.h"
#include <Shader.h>
#include "GraphicObject.h"

class Decor : public GraphicObject{
public:
	Decor(const Geometry& geometry, GLuint vao, glm::vec3 position);
	void draw(Shader shader);
};

#endif