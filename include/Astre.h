#ifndef  ASTRONOMICAL_INC
#define  ASTRONOMICAL_INC

#include "Geometry.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

class Astronomical : public Geometry
{
public:
	Astronomical(float * vertices,int nbVertices,float* normals,glm::vec3 position,float* uvs = NULL);

	void draw(Camera camera);

protected:
	GLuint vbo;
	std::vector<Astronomical> childs;
};

#endif