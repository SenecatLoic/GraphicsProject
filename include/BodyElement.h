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

extern int id_index;

class BodyElement : public GraphicObject {

public:
	BodyElement();
	BodyElement(Geometry geometry, GLuint vao, glm::vec3 position,struct material material);
	std::vector<BodyElement*> getChilds();
	void addChilds(BodyElement* child);
	void draw(Shader* shader, std::stack<glm::mat4> stack,glm::mat4 parentModel,Camera camera, struct light light);
	glm::mat4 getModel();
	virtual void translate(glm::vec3 move);
	int id;

private:
	std::vector<BodyElement*> childs;
	
};

#endif