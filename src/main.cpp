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
#include "Personnage.h"
#include <stack>
#include <vector>
#include "Cone.h"

#define WIDTH     800
#define HEIGHT    600
#define FRAMERATE 60
#define TIME_PER_FRAME_MS  (1.0f/FRAMERATE * 1e3)
#define INDICE_TO_PTR(x) ((void*)(x))

GLuint generate_vao(Geometry geo) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, (3 + 3 + 2) * sizeof(float) * geo.getNbVertices(), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * geo.getNbVertices(), geo.getVertices());
    glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * geo.getNbVertices(), 3 * sizeof(float) * geo.getNbVertices(), geo.getNormals());
    glBufferSubData(GL_ARRAY_BUFFER,3 * 2 * sizeof(float) * geo.getNbVertices(), geo.getNbVertices() * sizeof(float) * 2, geo.getUVs());


    GLuint vaoID;

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, INDICE_TO_PTR(3 * geo.getNbVertices() * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, INDICE_TO_PTR(2 * 3 * geo.getNbVertices() * sizeof(float)));
    glEnableVertexAttribArray(2);

    return vaoID;
}

int main(int argc, char* argv[])
{
    ////////////////////////////////////////
    //SDL2 / OpenGL Context initialization : 
    ////////////////////////////////////////

    //Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
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

    FILE* vertFile = fopen("Shaders/color.vert", "r");
    FILE* fragFile = fopen("Shaders/color.frag", "r");

    Shader* shader = Shader::loadFromFiles(vertFile, fragFile);

    fclose(vertFile);
    fclose(fragFile);

    if (shader == NULL)
    {
        return EXIT_FAILURE;
    }

    material m;
    m.alpha = 32;
    m.ka = 0.2f;
    m.kd = 0.5f;
    m.ks = 0.4f;

    m.color = glm::vec3(1, 0, 0);

    Cube c;
    GLuint vao_chest = generate_vao(c);
    BodyElement chest(c, vao_chest, glm::vec3(0, 1, 0), m);
    chest.scale(glm::vec3(0.5,0.5,0.5));
    chest.translate(glm::vec3(0.5, 0, 0));
    
    Sphere s(32, 32);
    GLuint vao_head = generate_vao(s);
    BodyElement head(s, vao_head, glm::vec3(0, 1, 0), m);
    head.scale(glm::vec3(0.5,0.5,0.5));
    head.translate(glm::vec3(0.5,0,0));
    chest.addChilds(head);
    //Decor d((Geometry)c,vao_decor,glm::vec3(0,0,0));
    
    Cube arm_l;
    GLuint vao_arm_left = generate_vao(arm_l);
    BodyElement arm_left(arm_l, vao_arm_left, glm::vec3(0, 1, 0), m);
    arm_left.scale(glm::vec3(0.5,0.5,0.5));
    arm_left.translate(glm::vec3(0.5, 0, 0));
    chest.addChilds(arm_left);

    Cube arm_r;
    GLuint vao_arm_right = generate_vao(arm_r);
    BodyElement arm_right(arm_l, vao_arm_right, glm::vec3(0, 1, 0), m);
    arm_right.scale(glm::vec3(0.5,0.5,0.5));
    arm_right.translate(glm::vec3(0.5, 0, 0));
    chest.addChilds(arm_right);

    Cone forearm_l(10, 10);
    GLuint vao_forearm_left = generate_vao(forearm_l);
    BodyElement forearm_left(forearm_l, vao_forearm_left, glm::vec3(0, 1, 0), m);
    forearm_left.scale(glm::vec3(0.5,0.5,0.5));
    forearm_left.translate(glm::vec3(0.5,0,0));
    arm_left.addChilds(forearm_left);

    Cone forearm_r(10, 10);
    GLuint vao_forearm_right = generate_vao(forearm_r);
    BodyElement forearm_right(forearm_r, vao_forearm_right, glm::vec3(0, 1, 0), m);
    forearm_right.scale(glm::vec3(0.5,0.5,0.5));
    forearm_right.translate(glm::vec3(0.5,0,0));
    arm_right.addChilds(forearm_right);

    Cone thigh_r(20, 20);
    GLuint vao_thigh_right = generate_vao(thigh_r);
    BodyElement thigh_right(thigh_r, vao_thigh_right, glm::vec3(0, 1, 0), m);
    thigh_right.scale(glm::vec3(0.5,0.5,0.5));
    thigh_right.translate(glm::vec3(0.5,0,0));
    chest.addChilds(thigh_right);

    Cone thigh_l(20, 20);
    GLuint vao_thigh_left = generate_vao(thigh_l);
    BodyElement thigh_left(thigh_l, vao_thigh_left, glm::vec3(0, 1, 0), m);
    thigh_left.scale(glm::vec3(0.5,0.5,0.5));
    thigh_left.translate(glm::vec3(0.5,0,0));
    chest.addChilds(thigh_left);

    Cone leg_r(15, 15);
    GLuint vao_leg_right = generate_vao(leg_r);
    BodyElement leg_right(leg_r, vao_leg_right, glm::vec3(0, 1, 0), m);
    leg_right.scale(glm::vec3(0.5,0.5,0.5));
    leg_right.translate(glm::vec3(0.5,0,0));
    thigh_right.addChilds(leg_right);

    Cone leg_l(15, 15);
    GLuint vao_leg_left = generate_vao(leg_l);
    BodyElement leg_left(leg_l, vao_leg_left, glm::vec3(0, 1, 0), m);
    leg_left.scale(glm::vec3(0.5,0.5,0.5));
    leg_left.translate(glm::vec3(0.5,0,0));
    thigh_left.addChilds(leg_left);

    Personnage perso(chest);



    //GLuint vao_sphere = generate_vao(cube);


    light l;
    l.color = glm::vec3(1.f, 1.f, 1.f);
    l.position = glm::vec3(0.0, 0.5, 1.0);


    bool isOpened = true;

    glm::mat4 model(1.f);
    glm::mat4 model_light(1.f);
    model_light = glm::translate(model_light, l.position);
    model_light = glm::scale(model_light, glm::vec3(0.2f));
    float n = 0.01f;
    float const speed = 0.03;
    float lastX = 400;
    float lastY = 300;
    bool mouse_user = true;
    float x_direct = -90.f;
    float y_direct = 0;
    std::stack<glm::mat4> stack;

    Camera camera(HEIGHT, WIDTH, glm::vec3(0, 1, 2), glm::vec3(0, -0.5, -1));


    //Main application loop
    while (isOpened)
    {
        //Time in ms telling us when this frame started. Useful for keeping a fix framerate
        uint32_t timeBegin = SDL_GetTicks();

        //Fetch the SDL events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            case SDL_WINDOWEVENT:
                switch (event.window.event)
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

        glm::mat4 cameraMatrix = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();
        
        stack.push(projection * cameraMatrix);
        perso.draw(shader, stack, camera, l);

        //Display on screen (swap the buffer on screen and the buffer you are drawing on)
        SDL_GL_SwapWindow(window);

        //Time in ms telling us when this frame ended. Useful for keeping a fix framerate
        uint32_t timeEnd = SDL_GetTicks();

        //We want FRAMERATE FPS
        if (timeEnd - timeBegin < TIME_PER_FRAME_MS)
            SDL_Delay(TIME_PER_FRAME_MS - (timeEnd - timeBegin));
    }

    //Free everything
    if (context != NULL)
        SDL_GL_DeleteContext(context);
    if (window != NULL)
        SDL_DestroyWindow(window);

    return 0;
}
