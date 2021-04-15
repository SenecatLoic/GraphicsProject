//SDL Libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

//OpenGL Libraries
#include <GL/glew.h>
#include <GL/gl.h>

//GML libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Geometry.h"
#include "Sphere.h"
#include "Camera.h"
#include "logger.h"
#include "BodyElement.h"
#include "Cube.h"
#include <Decor.h>


#define WIDTH     800
#define HEIGHT    600
#define FRAMERATE 60
#define TIME_PER_FRAME_MS  (1.0f/FRAMERATE * 1e3)
#define INDICE_TO_PTR(x) ((void*)(x))

struct material {
    float ka;
    float kd;
    float ks;
    float alpha;
    glm::vec3 color;
};

struct ligth {
    glm::vec3 position;
    glm::vec3 color;
};

GLuint generate_vao(Geometry geo) {
    GLuint buffer;
    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, (3 * 2) * sizeof(float) * geo.getNbVertices(), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * geo.getNbVertices(), geo.getVertices());

    glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * geo.getNbVertices(), 3 * sizeof(float) * geo.getNbVertices(), geo.getNormals());
    //glBufferSubData(GL_ARRAY_BUFFER, geo.getNbVertices() * 2 * sizeof(float) * 3, geo.getNbVertices() * sizeof(float) * 3, geo.getUVs());


    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, INDICE_TO_PTR(3 * geo.getNbVertices() * sizeof(float)));
    glEnableVertexAttribArray(1);

    return vaoID;
}

int main(int argc, char *argv[])
{
    ////////////////////////////////////////
    //SDL2 / OpenGL Context initialization : 
    ////////////////////////////////////////
    
    //Initialize SDL2
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        ERROR("The initialization of the SDL failed : %s\n", SDL_GetError());
        return 0;
    }

    //Create a Window
    SDL_Window* window = SDL_CreateWindow("VR Camera",                           //Titre
                                          SDL_WINDOWPOS_UNDEFINED,               //X Position
                                          SDL_WINDOWPOS_UNDEFINED,               //Y Position
                                          WIDTH, HEIGHT,                         //Resolution
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN); //Flags (OpenGL + Show)

    //Initialize OpenGL Version (version 3.0)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    //Initialize the OpenGL Context (where OpenGL resources (Graphics card resources) lives)
    SDL_GLContext context = SDL_GL_CreateContext(window);

    //Tells GLEW to initialize the OpenGL function with this version
    glewExperimental = GL_TRUE;
    glewInit();


    //Start using OpenGL to draw something on screen
    glViewport(0, 0, WIDTH, HEIGHT); //Draw on ALL the screen

    //The OpenGL background color (RGBA, each component between 0.0f and 1.0f)
    glClearColor(0.0, 0.0, 0.0, 1.0); //Full Black

    glEnable(GL_DEPTH_TEST); //Active the depth test

    //TODO
    //From here you can load your OpenGL objects, like VBO, Shaders, etc.
    //TODO

    FILE* vertFile = fopen("Shaders/color.vert", "r");
    FILE* fragFile = fopen("Shaders/color.frag", "r");

    Shader* shader = Shader::loadFromFiles(vertFile, fragFile);

    fclose(vertFile);
    fclose(fragFile);

    if (shader == NULL)
    {
        return EXIT_FAILURE;
    }

    Sphere cube(32, 32);
    Cube c;
    GLuint vao_decor = generate_vao(c);
    BodyElement torse();
    Decor d((Geometry)c,vao_decor,glm::vec3(0,0,0));

    GLuint vao_sphere = generate_vao(cube);
    material m;
    m.alpha = 32;
    m.ka = 0.2f;
    m.kd = 0.5f;
    m.ks = 0.4f;
    m.color = glm::vec3(1, 0, 0);

    ligth l;
    l.color = glm::vec3(1.f, 1.f, 1.f);
    l.position = glm::vec3(0.0, 1.0, 2.0);


    bool isOpened = true;

    glm::mat4 model(1.f);
    glm::mat4 model_ligth(1.f);
    model_ligth = glm::translate(model_ligth, l.position);
    model_ligth = glm::scale(model_ligth, glm::vec3(0.2f));
    float n = 0.01f;
    float const speed = 0.03;
    float lastX = 400;
    float lastY = 300;
    bool mouse_user = true;
    float x_direct = -90.f;
    float y_direct = 0;

    Camera camera(HEIGHT,WIDTH,glm::vec3(0, 1, 2),glm::vec3(0,-0.5, -1));


    //Main application loop
    while(isOpened)
    {
        //Time in ms telling us when this frame started. Useful for keeping a fix framerate
        uint32_t timeBegin = SDL_GetTicks();

        //Fetch the SDL events
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                
                case SDL_WINDOWEVENT:
                    switch(event.window.event)
                    {
                        case SDL_WINDOWEVENT_CLOSE:
                            isOpened = false;
                            break;
                        default:
                            break;
                    }
                    break;

                case SDL_KEYDOWN:
                    camera.eventKeyboard(event.key.keysym.scancode);
                    break;
                //We can add more event, like listening for the keyboard or the mouse. See SDL_Event documentation for more details
            }
        }

        //Clear the screen : the depth buffer and the color buffer
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


        //avoir un vao par type d'objet vertex

        glm::mat4 projection = camera.getProjectionMatrix();

        //view
        glm::mat4 view_mat = camera.getViewMatrix();
        //TODO rendering
        
        
        model = glm::rotate(model, 0.01f, glm::vec3(0, 1, 0));

        glm::mat4 mat = projection * view_mat * model;
        glm::mat3 inv_model = glm::inverse(glm::mat3(model));



        glUseProgram(shader->getProgramID());
        {
            glBindVertexArray(vao_sphere);

            GLint uMVP = glGetUniformLocation(shader->getProgramID(), "uMVP");
            glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(mat));

            GLint uModelViewMatrix = glGetUniformLocation(shader->getProgramID(), "uModelViewMatrix");
            glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(model));

            GLint inv_uModelViewMatrix = glGetUniformLocation(shader->getProgramID(), "inv_uModelViewMatrix");
            glUniformMatrix3fv(inv_uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(inv_model));

            GLint ka = glGetUniformLocation(shader->getProgramID(), "ka");
            glUniform1f(ka, m.ka);
            GLint kd = glGetUniformLocation(shader->getProgramID(), "kd");
            glUniform1f(kd, m.kd);
            GLint ks = glGetUniformLocation(shader->getProgramID(), "ks");
            glUniform1f(ks, m.ks);
            GLint alpha = glGetUniformLocation(shader->getProgramID(), "alpha");
            glUniform1f(alpha, m.alpha);

            GLint ligth_color = glGetUniformLocation(shader->getProgramID(), "light_color");
            glUniform3fv(ligth_color, 1, glm::value_ptr(l.color));

            GLint ligth_pos = glGetUniformLocation(shader->getProgramID(), "light_pos");
            glUniform3fv(ligth_pos, 1, glm::value_ptr(l.position));

            GLint mat_color = glGetUniformLocation(shader->getProgramID(), "mat_color");
            glUniform3fv(mat_color, 1, glm::value_ptr(m.color));

            GLint camera_pos = glGetUniformLocation(shader->getProgramID(), "camera_pos");
            glUniform3fv(camera_pos, 1, glm::value_ptr(camera.getPosition()));

            glDrawArrays(GL_TRIANGLES, 0, cube.getNbVertices());

            glBindVertexArray(0);
        }
        glUseProgram(0);
        

        //Display on screen (swap the buffer on screen and the buffer you are drawing on)
        SDL_GL_SwapWindow(window);

        //Time in ms telling us when this frame ended. Useful for keeping a fix framerate
        uint32_t timeEnd = SDL_GetTicks();

        //We want FRAMERATE FPS
        if(timeEnd - timeBegin < TIME_PER_FRAME_MS)
            SDL_Delay(TIME_PER_FRAME_MS - (timeEnd - timeBegin));
    }
    
    //Free everything
    if(context != NULL)
        SDL_GL_DeleteContext(context);
    if(window != NULL)
        SDL_DestroyWindow(window);

    return 0;
}
