#include "Camera.h"

Camera::Camera(const float height,const float width, glm::vec3& position, glm::vec3& direction, glm::vec3& cameraUp, float speed) {
	Camera::position = position;
	Camera::direction = direction;
	Camera::cameraUp = cameraUp;
	Camera::speed = speed;
	projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
}

const glm::vec3 Camera::getPosition() {
	return position;
}

void Camera::eventKeyboard(const SDL_Scancode key_event)
{
    glm::vec3 mouvement(0, 0, 0);
	switch (key_event) {
        case SDL_SCANCODE_LEFT:
            position -= glm::normalize(glm::cross(direction, cameraUp)) * speed;

            break;
        case SDL_SCANCODE_RIGHT:
            position += glm::normalize(glm::cross(direction, cameraUp)) * speed;

            break;
        case SDL_SCANCODE_UP:
            glm::vec3 direct = direction;
            direct.y = 0;
            position += direct * speed;

            break;
        case SDL_SCANCODE_DOWN:
            glm::vec3 direct_d = direction;
            direct_d.y = 0;
            position -= direct_d * speed;
            break;
        default:
            break;
	}
}

const glm::mat4 Camera::getProjectionMatrix() {
	return projection;
}

const glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + direction, cameraUp);
}