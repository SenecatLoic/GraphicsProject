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
}

GraphicObject::GraphicObject() {
	
}

void GraphicObject::translate(glm::vec3 move) {
	model = glm::translate(model,move);
    position += move;
}

void GraphicObject::movePosition(glm::vec3 move) {
    position += move;
}

void GraphicObject::rotate(float angle, glm::vec3 axes) {
	model = glm::rotate(model,angle,axes);
}

void GraphicObject::scale(glm::vec3 v) {
	localTransformation = glm::scale(localTransformation,v);
    width *= v.x;
    height *= v.y;
    depth *= v.z;
}

glm::vec3 GraphicObject::getPosition() {
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
    float posX = mouvement.x + position.x;

    /*printf("first %f ", object.position.x);
    printf("%f \n", position.x);*/


    return ((posX + width) >= (object.position.x - object.width) &&
        (object.position.x - object.width) >= (posX - width)) ||
        ((posX + width) >= (object.position.x + object.width) &&
            (object.position.x + object.width) >= (posX - width));
}

bool GraphicObject::collisionY(GraphicObject& object, glm::vec3 mouvement) {
    float posY = mouvement.y + position.y;
    
    /*printf(" first %f ", object.position.y);
    printf("%f \n", position.y);*/


    return ((posY + height) >= (object.position.y - object.height) &&
        (object.position.y - object.height) >= (posY - height)) ||
        ((posY + height) >= (object.position.y + object.height) &&
            (object.position.y + object.height) >= (posY - height));
}

bool GraphicObject::collisionZ(GraphicObject& object, glm::vec3 mouvement) {
    float posZ = mouvement.z + position.z;
    /*printf(" first %f ", object.position.z);
    printf("%f \n", position.z);*/
    return ((posZ + depth) >= (object.position.z - object.depth) &&
        (object.position.z - object.depth) >= (posZ - depth)) ||
        ((posZ + depth) >= (object.position.z + object.depth) &&
            (object.position.z + object.depth) >= (posZ - depth));
}

float GraphicObject::getWidth(){
    return width;
}
float GraphicObject::getHeight(){
    return height;
}