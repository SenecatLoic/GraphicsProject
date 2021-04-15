#ifndef  GraphicObject_INC
#define  GraphicObject_INC

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Geometry.h"
#include <Shader.h>

class GraphicObject : public Geometry {
public:
	GraphicObject();
	GraphicObject(const Geometry& geometry, GLuint vao, glm::vec3 position);
	virtual void draw(Shader shader) = 0;
protected:
	glm::vec3 position;
	GLuint vao;
};

#endif