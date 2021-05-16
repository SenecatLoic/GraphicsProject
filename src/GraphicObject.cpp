#include "GraphicObject.h"
#include <SDL2/SDL_image.h>
#include <string>

GraphicObject::GraphicObject(const Geometry& geometry, GLuint vao, glm::vec3 position,struct material material) : Geometry(geometry)
{
	this->vao = vao;
	this->position = position;
	this->material = material;
	this->model = glm::mat4(1.f);
	this->localTransformation = glm::mat4(1.f);
    this->width = 0.5;
    this->height = 0.5;
    this->depth = 0.5;
    this->angle = 0;
    this->last_rotation = glm::vec3(0.f);

    this->min = glm::vec3(position.x - width,position.y - height,position.z - depth);
    this->max = glm::vec3(position.x + width, position.y + height, position.z + depth);
}

GraphicObject::GraphicObject() {
	
}

void GraphicObject::translate(glm::vec3 move) {
	model = glm::translate(model,move);
    position += move;
    max += move;
    min += move;
}

void GraphicObject::movePosition(glm::vec3 move) {
    position += move;
    
    max += move;
    min += move;

}

void GraphicObject::rotate(float angle, glm::vec3 axes) {
    model = glm::rotate(model, angle, axes);
    
    this->angle += angle;
    glm::mat3 q(0.f);
    glm::mat3 I(1.f);
    q[0][1] = -axes.z;
    q[0][2] = axes.y;
    q[1][0] = axes.z;
    q[1][2] = -axes.x;
    q[2][0] = -axes.y;
    q[2][1] = axes.x;
    glm::mat3 rotation = I + sin(this->angle) * q + (1 - cos(this->angle)) * (q * q);

    glm::vec3 current_rotation;
    current_rotation.x = rotation[0][0] + rotation[0][1] + rotation[0][2];
    current_rotation.y = rotation[1][0] + rotation[1][1] + rotation[1][2];
    current_rotation.z = rotation[2][0] + rotation[2][1] + rotation[2][2];

    float v = abs(current_rotation.x) - 1 - last_rotation.x;
    float y = abs(current_rotation.y) - 1 - last_rotation.y;
    float z = abs(current_rotation.z) - 1 - last_rotation.z;

    position.x += v;
    position.y += y;
    position.z += z;

    max.x += v;
    max.y += y;
    max.z += z;

    min.x += v;
    min.y += y;
    min.z += z;

    last_rotation.x = abs(current_rotation.x) - 1;
    last_rotation.y = abs(current_rotation.y) - 1;
    last_rotation.z = abs(current_rotation.z) - 1;  
}

void GraphicObject::scale(glm::vec3 v) {
	localTransformation = glm::scale(localTransformation,v);
    width *= v.x;
    height *= v.y;
    depth *= v.z;

    this->min = glm::vec3(position.x - width, position.y - height, position.z - depth);
    this->max = glm::vec3(position.x + width, position.y + height, position.z + depth);
}

const glm::vec3 GraphicObject::getPosition() {
    return position;
}

void GraphicObject::loadingTexture(const char* path){
	SDL_Surface* img = IMG_Load(path);
    SDL_Surface* rgbImg = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(img);

    if (rgbImg == nullptr) {
        printf("Error Loading texture");
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rgbImg->w, rgbImg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)rgbImg->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool GraphicObject::collision(GraphicObject &object,glm::vec3 mouvement) {

    return collisionX(object, mouvement) && collisionY(object,mouvement) && collisionZ(object, mouvement);
}

bool GraphicObject::collisionX(GraphicObject& object, glm::vec3 mouvement) {
    float maxPosX = mouvement.x + max.x;
    float minPosX = mouvement.x + min.x;

    return (minPosX <= object.max.x && maxPosX >= object.min.x);
}

bool GraphicObject::collisionY(GraphicObject& object, glm::vec3 mouvement) {
    float maxPosY = mouvement.y + max.y;
    float minPosY = mouvement.y + min.y;

    return (minPosY <= object.max.y && maxPosY >= object.min.y);
}

bool GraphicObject::collisionZ(GraphicObject& object, glm::vec3 mouvement) {
    float maxPosZ = mouvement.z + max.z;
    float minPosZ = mouvement.z + min.z;

    return (minPosZ <= object.max.z && maxPosZ >= object.min.z);
}

float GraphicObject::getWidth(){
    return width;
}
float GraphicObject::getHeight(){
    return height;
}

glm::vec3 GraphicObject::getMin() {
    return min;
}

glm::vec3 GraphicObject::getMax() {
    return max;
}