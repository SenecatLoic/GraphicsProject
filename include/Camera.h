#ifndef  Camera_INC
#define  Camera_INC

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_keyboard.h>

class Camera {
public:
	Camera(const float height, const float width,glm::vec3& position = glm::vec3(0,0,1),glm::vec3& direction = glm::vec3(0,0,-1),
		glm::vec3& cameraUp = glm::vec3(0,1,0), const float speed = 0.03f);
	
	const glm::mat4 getViewMatrix();

	const glm::mat4 getProjectionMatrix();

	const glm::vec3 getPosition();

	void eventKeyboard(const SDL_Scancode key_event);

private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 cameraUp;
	glm::mat4 projection;
	float speed;
};

#endif