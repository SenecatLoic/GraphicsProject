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

glm::vec3 Camera::eventKeyboard(const SDL_Scancode key_event)
{
    glm::vec3 mouvement;
	switch (key_event) {
        case SDL_SCANCODE_LEFT:
            mouvement = glm::normalize(glm::cross(direction, cameraUp)) * (speed * -1);
            position += mouvement;
            break;
        case SDL_SCANCODE_RIGHT:
            mouvement = glm::normalize(glm::cross(direction, cameraUp)) * speed;
            position += mouvement;
            break;
        case SDL_SCANCODE_UP:
            glm::vec3 direct = direction;
            direct.y = 0;
            mouvement = direct * speed;
            position += mouvement;

            break;
        case SDL_SCANCODE_DOWN:
            direct = direction;
            direct.y = 0;
            mouvement = (direct * speed * -1.f);
            position += mouvement;
            break;
        default:
            break;
	}
    return mouvement;
}

const glm::mat4 Camera::getProjectionMatrix() {
	return projection;
}

const glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + direction, cameraUp);
}

void Camera::setDirection(glm::vec3 direction) {
    this->direction = direction;
}

const glm::vec3 Camera::getDirection() {
    return direction;
}