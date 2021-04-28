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
}

GraphicObject::GraphicObject() {
	
}

void GraphicObject::translate(glm::vec3 move) {
	model = glm::translate(model,move);
}

void GraphicObject::rotate(float angle, glm::vec3 axes) {
	model = glm::rotate(model,angle,axes);
}

void GraphicObject::scale(glm::vec3 v) {
	localTransformation = glm::scale(localTransformation,v);
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
