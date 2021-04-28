#ifndef  GraphicObject_INC
#define  GraphicObject_INC

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Geometry.h"
#include <Shader.h>
#include <stack>
#include <Camera.h>

struct material {
	float ka;
	float kd;
	float ks;
	float alpha;
	glm::vec3 color;
};

struct light {
	glm::vec3 position;
	glm::vec3 color;
};

class GraphicObject : public Geometry {
public:
	GraphicObject();
	GraphicObject(const Geometry& geometry, GLuint vao, glm::vec3 position,material material);
	virtual void draw(Shader* shader, std::stack<glm::mat4> stack, glm::mat4 parentModel, Camera camera, struct light light) = 0;
	void translate(glm::vec3 move);
	void rotate(float angle,glm::vec3 axes);
	void scale(glm::vec3 v);
	void loadingTexture(const char* path);
protected:
	glm::vec3 position;
	GLuint vao;
	material material;
	glm::mat4 model;
	glm::mat4 localTransformation;
	GLuint textureID;
};

#endif