#include "Camera.h"


Camera::Camera(const float height,const float width, glm::vec3& position,glm::vec3& objectPosition, glm::vec3& direction, glm::vec3& cameraUp, float speed) {
	Camera::position = position;
	Camera::direction = direction;
    Camera::initDirection = direction;
	Camera::cameraUp = cameraUp;
	Camera::speed = speed;
    Camera::objectPosition = objectPosition;
	projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
    Camera::totalMov = glm::vec3(60,0,0); // permet de placer la caméra derrière ou devant l'objet suivi
    Camera::midX = width/2;
    Camera::midY = height/2;
}

const glm::vec3 Camera::getPosition() {
	return position;
}

glm::vec3 Camera::eventKeyboard(const SDL_Scancode key_event)
{
    glm::vec3 mouvement(0.f);
	switch (key_event) {
        case SDL_SCANCODE_LEFT:
            mouvement = glm::normalize(glm::cross(direction, cameraUp)) * (speed * -1);

            break;
        case SDL_SCANCODE_RIGHT:
            mouvement = glm::normalize(glm::cross(direction, cameraUp)) * speed;
            /*position += mouvement;
            objectPosition += mouvement;*/
            break;
        case SDL_SCANCODE_UP:
            glm::vec3 direct = direction;
            direct.y = 0;
            mouvement = direct * speed;

            break;
        case SDL_SCANCODE_DOWN:
            direct = direction;
            direct.y = 0;
            mouvement = (direct * speed * -1.f);

            break;
        default:
            break;
	}
    return mouvement;
}

void Camera::eventMouse(SDL_Event event) {
    float xoffset = event.motion.x - midX;
    float yoffset = midY - event.motion.y; // reversed since y-coordinates range from bottom to top
    float sensitivity = 2.f;
    glm::vec3 cam_mouvement(0.f);

        if (xoffset > 0) {

            totalMov.x += (sensitivity * abs(round(xoffset)));
        }
        else {
            totalMov.x -= (sensitivity * abs(round(xoffset)));
        }

        if (yoffset < 0) {
            totalMov.y -= 0.03;
        }
        else {
            totalMov.y += 0.03;
        }


        if (totalMov.y > 0.5) {
            totalMov.y = 0.5;
        }
        if (totalMov.y < -1) {
            totalMov.y = -1;
        }

        cam_mouvement.x = cos(glm::radians(totalMov.x)) * 3;
        cam_mouvement.z = sin(glm::radians(totalMov.x)) * 3;
        glm::vec3 direct_movement;
        direct_movement.x = -cam_mouvement.x;
        direct_movement.z = -cam_mouvement.z;

        position.x = objectPosition.x + cam_mouvement.x;
        position.y = objectPosition.y + totalMov.y;
        position.z = objectPosition.z + cam_mouvement.z;

        direction.x = initDirection.x + direct_movement.x;
        direction.z = initDirection.z + direct_movement.z;
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

void Camera::move(glm::vec3 mouvement) {
    position += mouvement;
    objectPosition += mouvement;
}