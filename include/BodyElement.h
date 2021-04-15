#ifndef  BodyElement_INC
#define  BodyElement_INC
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

class BodyElement : public GraphicObject {

public:
	BodyElement();
	BodyElement(Geometry geometry, GLuint vao, glm::vec3 position);
	const std::vector<BodyElement> getChilds();
	void addChilds(BodyElement& child);
	void draw(Shader shader);

private:
	std::vector<BodyElement> childs;

};

#endif