#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_image.h>
#include "Shader.h"
#include "Geometry.h"
#include "Camera.h"
#include "BodyElement.h"



class Skybox {

private:
    GLuint vaoskybox;
    std::vector<const GLchar*> faces;
    GLuint cubemapTexture;

    float skyboxVertices[36 * 3] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    GLuint loadCubemap(std::vector<const char*> faces)
    {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        for (unsigned int i = 0; i < faces.size(); i++)
        {

            SDL_Surface* imgdata = IMG_Load(faces[i]);
            SDL_Surface* rgbImgdata = SDL_ConvertSurfaceFormat(imgdata, SDL_PIXELFORMAT_RGBA32, 0);
            SDL_FreeSurface(imgdata);

            if (rgbImgdata == nullptr) {
                printf("Error Loading texture");
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, rgbImgdata->w, rgbImgdata->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)rgbImgdata->pixels);

        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;

    }
public:
    Skybox(const GLchar* front, const GLchar* left, const GLchar* right, const GLchar* bottom, const GLchar* top, const GLchar* back) {

        glGenBuffers(1, &this->vaoskybox);
        glBindBuffer(GL_ARRAY_BUFFER, vaoskybox);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->skyboxVertices), NULL, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(this->skyboxVertices), this->skyboxVertices);

        this->faces.push_back(front);
        this->faces.push_back(left);
        this->faces.push_back(right);
        this->faces.push_back(bottom);
        this->faces.push_back(top);
        this->faces.push_back(back);

        this->cubemapTexture = loadCubemap(faces);
    }

    Skybox(std::vector<const GLchar*> faces) {

        this->faces = faces;
        this->cubemapTexture = loadCubemap(faces);

        glGenBuffers(1, &this->vaoskybox);
        glBindBuffer(GL_ARRAY_BUFFER, this->vaoskybox);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->skyboxVertices), NULL, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(this->skyboxVertices), this->skyboxVertices);

    }

    void Skybox::draw(Shader* shader, std::stack<glm::mat4> matrix, Camera camera, glm::mat4 projectionMatrix) {

        glDepthMask(GL_FALSE);
        glUseProgram(shader->getProgramID());
        {
            glBindBuffer(GL_ARRAY_BUFFER, this->vaoskybox);

            //VPosition Skybox

            GLint vskyposition = glGetAttribLocation(shader->getProgramID(), "vskyposition");
            glVertexAttribPointer(vskyposition, 3, GL_FLOAT, 0, 0, 0);
            glEnableVertexAttribArray(vskyposition);

            //Les matrices

            GLuint uViewMatrix = glGetUniformLocation(shader->getProgramID(), "uMViewMatrix");
            glm::mat4 cameraMatrix = glm::mat4(glm::mat3(camera.getViewMatrix()));
            glUniformMatrix4fv(uViewMatrix, 1, GL_FALSE, glm::value_ptr(cameraMatrix));

            GLuint uProjMatrix = glGetUniformLocation(shader->getProgramID(), "uProjMatrix");
            glUniformMatrix4fv(uProjMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


            //Texture

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapTexture);
            GLint uTextureSkybox = glGetUniformLocation(shader->getProgramID(), "uTextureSkybox");
            glUniform1i(this->cubemapTexture, 0);

            //Dessin

            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        glUseProgram(0);
        glDepthMask(GL_TRUE);

    }

};

#endif