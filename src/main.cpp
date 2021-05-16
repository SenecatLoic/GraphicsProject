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

#include <SDL2/SDL_image.h>

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
#include "Cylinder.h"
#include <CollisionPerso.h>
#include <Skybox.h>

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
    glBufferSubData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float) * geo.getNbVertices(), geo.getNbVertices() * sizeof(float) * 2, geo.getUVs());


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

GLuint generate_particle_vao(Geometry geo) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, (3 + 3) * sizeof(float) * geo.getNbVertices(), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * geo.getNbVertices(), geo.getVertices());
    glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * geo.getNbVertices(), 3 * sizeof(float) * geo.getNbVertices(), geo.getNormals());


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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

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
        std::cerr << "Could not load shader";
        return EXIT_FAILURE;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        ERROR("Could not load SDL2 image with PNG files\n");
        return EXIT_FAILURE;
    }

    vertFile = fopen("Shaders/particle.vert", "r");
    fragFile = fopen("Shaders/particle.frag", "r");

    Shader* particleShader = Shader::loadFromFiles(vertFile, fragFile);


    if (particleShader == NULL)
    {
        std::cerr << "Could not load shader";
        return EXIT_FAILURE;
    }

    FILE* skyvertFile = fopen("Shaders/skybox.vert", "r");
    FILE* skyfragFile = fopen("Shaders/skybox.frag", "r");

    Shader* skyshader = Shader::loadFromFiles(skyvertFile, skyfragFile);


    material m;
    m.alpha = 32;
    m.ka = 0.2f;
    m.kd = 0.5f;
    m.ks = 0.4f;
    m.color = glm::vec3(1, 0, 0);

    std::vector<const GLchar*> faces = { "texture/front.png" , "texture/left.png" , "texture/right.png", "texture/top.png", "texture/bottom.png", "texture/back.png" };

    Skybox skybox = Skybox(faces);
    
    // PERSO 1

    Cube c;
    GLuint vao_chest = generate_vao(c);
    BodyElement chest(c, vao_chest, glm::vec3(0, 1, 0), m);
    chest.loadingTexture("texture/beton.png");

    Sphere s(32, 32);
    GLuint vao_head = generate_vao(s);
    BodyElement head(s, vao_head, glm::vec3(0, 1, 0), m);
    chest.addChilds(&head);
    head.loadingTexture("texture/head.png");

    Cylinder neck_top(32);
    GLuint vao_neck = generate_vao(neck_top);
    BodyElement neck(neck_top, vao_neck, glm::vec3(0, 1, 0), m);

    chest.addChilds(&neck);
    neck.loadingTexture("texture/skin.png");

    Cube arm_l;
    GLuint vao_arm_left = generate_vao(arm_l);
    BodyElement arm_left(arm_l, vao_arm_left, glm::vec3(0, 1, 0), m);
    chest.addChilds(&arm_left);
    arm_left.loadingTexture("texture/bleu.png");

    Cube arm_r;
    GLuint vao_arm_right = generate_vao(arm_r);
    BodyElement arm_right(arm_l, vao_arm_right, glm::vec3(0, 1, 0), m);
    chest.addChilds(&arm_right);
    arm_right.loadingTexture("texture/bleu.png");

    Sphere elbow_l(32, 32);
    GLuint vao_elbow_left = generate_vao(elbow_l);
    BodyElement elbow_left(elbow_l, vao_elbow_left, glm::vec3(0, 1, 0), m);

    arm_left.addChilds(&elbow_left);
    elbow_left.loadingTexture("texture/skin.png");

    Sphere elbow_r(32, 32);
    GLuint vao_elbow_right = generate_vao(elbow_r);
    BodyElement elbow_right(elbow_r, vao_elbow_right, glm::vec3(0, 1, 0), m);

    arm_right.addChilds(&elbow_right);
    elbow_right.loadingTexture("texture/skin.png");

    Cylinder forearm_l(32);
    GLuint vao_forearm_left = generate_vao(forearm_l);
    BodyElement forearm_left(forearm_l, vao_forearm_left, glm::vec3(0, 1, 0), m);

    elbow_left.addChilds(&forearm_left);
    forearm_left.loadingTexture("texture/skin.png");

    Cylinder forearm_r(32);
    GLuint vao_forearm_right = generate_vao(forearm_r);
    BodyElement forearm_right(forearm_r, vao_forearm_right, glm::vec3(0, 1, 0), m);

    elbow_right.addChilds(&forearm_right);
    forearm_right.loadingTexture("texture/skin.png");

    Sphere hip_l(32, 32);
    GLuint vao_hip_left = generate_vao(hip_l);
    BodyElement hip_left(hip_l, vao_hip_left, glm::vec3(0, 1, 0), m);
    chest.addChilds(&hip_left);
    hip_left.loadingTexture("texture/bleu.png");

    Sphere hip_r(32, 32);
    GLuint vao_hip_right = generate_vao(hip_r);
    BodyElement hip_right(hip_r, vao_hip_right, glm::vec3(0, 1, 0), m);
    chest.addChilds(&hip_right);
    hip_right.loadingTexture("texture/bleu.png");

    Cylinder thigh_r(32);
    GLuint vao_thigh_right = generate_vao(thigh_r);
    BodyElement thigh_right(thigh_r, vao_thigh_right, glm::vec3(0, 1, 0), m);
    hip_right.addChilds(&thigh_right);
    thigh_right.loadingTexture("texture/bleu.png");

    Cylinder thigh_l(32);
    GLuint vao_thigh_left = generate_vao(thigh_l);
    BodyElement thigh_left(thigh_l, vao_thigh_left, glm::vec3(0, 1, 0), m);
    hip_left.addChilds(&thigh_left);
    thigh_left.loadingTexture("texture/bleu.png");

    Sphere knee_l(32, 32);
    GLuint vao_knee_left = generate_vao(knee_l);
    BodyElement knee_left(knee_l, vao_knee_left, glm::vec3(0, 1, 0), m);

    thigh_left.addChilds(&knee_left);
    knee_left.loadingTexture("texture/skin.png");

    Sphere knee_r(32, 32);
    GLuint vao_knee_right = generate_vao(knee_r);
    BodyElement knee_right(knee_r, vao_knee_right, glm::vec3(0, 1, 0), m);

    thigh_right.addChilds(&knee_right);
    knee_right.loadingTexture("texture/skin.png");



    Cylinder leg_r(32);
    GLuint vao_leg_right = generate_vao(leg_r);
    BodyElement leg_right(leg_r, vao_leg_right, glm::vec3(0, 1, 0), m);

    knee_right.addChilds(&leg_right);
    leg_right.loadingTexture("texture/skin.png");

    Cylinder leg_l(32);
    GLuint vao_leg_left = generate_vao(leg_l);
    BodyElement leg_left(leg_l, vao_leg_left, glm::vec3(0, 1, 0), m);

    knee_left.addChilds(&leg_left);
    leg_left.loadingTexture("texture/skin.png");

    Cube foot_l;
    GLuint vao_foot_left = generate_vao(foot_l);
    BodyElement foot_left(foot_l, vao_foot_left, glm::vec3(0, 1, 0), m);

    leg_left.addChilds(&foot_left);
    foot_left.loadingTexture("texture/bleu.png");

    Cube foot_r;
    GLuint vao_foot_right = generate_vao(foot_r);
    BodyElement foot_right(foot_r, vao_foot_right, glm::vec3(0, 1, 0), m);

    leg_right.addChilds(&foot_right);
    foot_right.loadingTexture("texture/bleu.png");

    Sphere hand_r(32, 32);
    GLuint vao_hand_right = generate_vao(hand_r);
    BodyElement hand_right(hand_r, vao_hand_right, glm::vec3(0, 1, 0), m);

    forearm_right.addChilds(&hand_right);
    hand_right.loadingTexture("texture/skin.png");

    Sphere hand_l(32, 32);
    GLuint vao_hand_left = generate_vao(hand_l);
    BodyElement hand_left(hand_l, vao_hand_left, glm::vec3(0, 1, 0), m);

    forearm_left.addChilds(&hand_left);
    hand_left.loadingTexture("texture/skin.png");

    chest.scale(glm::vec3(0.5, 0.7, 0.3));
    neck.scale(glm::vec3(0.2, 0.2, 0.15));
    head.scale(glm::vec3(0.5, 0.5, 0.5));
    arm_left.scale(glm::vec3(0.18, 0.3, 0.2));
    arm_right.scale(glm::vec3(0.18, 0.3, 0.2));
    elbow_left.scale(glm::vec3(0.2015, 0.2015, 0.2015));
    elbow_right.scale(glm::vec3(0.2015, 0.2015, 0.2015));
    forearm_left.scale(glm::vec3(0.15, 0.17, 0.32));
    forearm_right.scale(glm::vec3(0.15, 0.17, 0.32));
    hip_left.scale(glm::vec3(0.2015, 0.2015, 0.2015));
    hip_right.scale(glm::vec3(0.2015, 0.2015, 0.2015));
    thigh_right.scale(glm::vec3(0.2, 0.2, 0.5));
    thigh_left.scale(glm::vec3(0.2, 0.2, 0.5));
    knee_left.scale(glm::vec3(0.2, 0.2, 0.2));
    knee_right.scale(glm::vec3(0.2, 0.2, 0.2));
    leg_right.scale(glm::vec3(0.20, 0.15, 0.4));
    leg_left.scale(glm::vec3(0.20, 0.15, 0.4));
    foot_left.scale(glm::vec3(0.18, 0.2, 0.15));
    foot_right.scale(glm::vec3(0.18, 0.2, 0.15));
    hand_right.scale(glm::vec3(0.14, 0.14, 0.14));
    hand_left.scale(glm::vec3(0.14, 0.14, 0.14));

    chest.translate(glm::vec3(0.5, 0, 0));
    neck.translate(glm::vec3(0, 0.4, 0));
    neck.rotate(1.5, glm::vec3(1, 0, 0));
    head.translate(glm::vec3(0, 0.625, 0));
    head.rotate(3, glm::vec3(0, 1, 0));   
    arm_left.translate(glm::vec3(0.35, 0.1, 0)); 
    arm_right.translate(glm::vec3(-0.35, 0.1, 0));   
    elbow_left.translate(glm::vec3(0, -0.13, 0));  
    elbow_right.translate(glm::vec3(0, -0.13, 0));   
    forearm_left.translate(glm::vec3(0, -0.2, 0.1));
    forearm_left.rotate(-1.9, glm::vec3(1, 0, 0));   
    forearm_right.translate(glm::vec3(0, -0.2, 0.1));
    forearm_right.rotate(-1.9, glm::vec3(1, 0, 0));   
    hip_left.translate(glm::vec3(0.12, -0.4, 0));
    hip_right.translate(glm::vec3(-0.12, -0.4, 0));    
    thigh_right.translate(glm::vec3(0, -0.25, -0.03));
    thigh_right.rotate(1.7, glm::vec3(1, 0, 0));   
    thigh_left.translate(glm::vec3(0, -0.25, -0.03));
    thigh_left.rotate(1.7, glm::vec3(1, 0, 0));   
    knee_left.translate(glm::vec3(0, 0, 0.3));
    knee_right.translate(glm::vec3(0, 0, 0.3));
    leg_right.translate(glm::vec3(0, 0.01, 0.18));
    leg_left.translate(glm::vec3(0, 0.01, 0.18));
    foot_left.translate(glm::vec3(0, 0.1, 0.3));
    foot_right.translate(glm::vec3(0, 0.1, 0.3));
    hand_right.translate(glm::vec3(0, 0, -0.15));
    hand_left.translate(glm::vec3(0, 0, -0.15));

    //Perso 2
    Cube c2;
    GLuint vao_chest2 = generate_vao(c);
    BodyElement chest2(c2, vao_chest2, glm::vec3(0, 1, 0), m);
    chest2.loadingTexture("texture/beton.png");

    Sphere s2(32, 32);
    GLuint vao_head2 = generate_vao(s2);
    BodyElement head2(s2, vao_head2, glm::vec3(0, 1, 0), m);
    chest2.addChilds(&head2);
    head2.loadingTexture("texture/head.png");

    Cylinder neck_top2(32);
    GLuint vao_neck2 = generate_vao(neck_top2);
    BodyElement neck2(neck_top2, vao_neck2, glm::vec3(0, 1, 0), m);
    chest2.addChilds(&neck2);
    neck2.loadingTexture("texture/skin.png");

    Cube arm_l2;
    GLuint vao_arm_left2 = generate_vao(arm_l2);
    BodyElement arm_left2(arm_l2, vao_arm_left2, glm::vec3(0, 1, 0), m);
    chest2.addChilds(&arm_left2);
    arm_left2.loadingTexture("texture/bleu.png");

    Cube arm_r2;
    GLuint vao_arm_right2 = generate_vao(arm_r2);
    BodyElement arm_right2(arm_r2, vao_arm_right2, glm::vec3(0, 1, 0), m);
    chest2.addChilds(&arm_right2);
    arm_right2.loadingTexture("texture/bleu.png");

    Sphere elbow_l2(32, 32);
    GLuint vao_elbow_left2 = generate_vao(elbow_l2);
    BodyElement elbow_left2(elbow_l2, vao_elbow_left2, glm::vec3(0, 1, 0), m);
    arm_left2.addChilds(&elbow_left2);
    elbow_left2.loadingTexture("texture/skin.png");

    Sphere elbow_r2(32, 32);
    GLuint vao_elbow_right2 = generate_vao(elbow_r2);
    BodyElement elbow_right2(elbow_r2, vao_elbow_right2, glm::vec3(0, 1, 0), m);

    arm_right2.addChilds(&elbow_right2);
    elbow_right2.loadingTexture("texture/skin.png");

    Cylinder forearm_l2(32);
    GLuint vao_forearm_left2 = generate_vao(forearm_l2);
    BodyElement forearm_left2(forearm_l2, vao_forearm_left2, glm::vec3(0, 1, 0), m);

    elbow_left2.addChilds(&forearm_left2);
    forearm_left2.loadingTexture("texture/skin.png");

    Cylinder forearm_r2(32);
    GLuint vao_forearm_right2 = generate_vao(forearm_r2);
    BodyElement forearm_right2(forearm_r2, vao_forearm_right2, glm::vec3(0, 1, 0), m);

    elbow_right2.addChilds(&forearm_right2);
    forearm_right2.loadingTexture("texture/skin.png");

    Cylinder thigh_r2(32);
    GLuint vao_thigh_right2 = generate_vao(thigh_r2);
    BodyElement thigh_right2(thigh_r2, vao_thigh_right2, glm::vec3(0, 1, 0), m);
    chest2.addChilds(&thigh_right2);
    thigh_right2.loadingTexture("texture/bleu.png");

    Cylinder thigh_l2(32);
    GLuint vao_thigh_left2 = generate_vao(thigh_l2);
    BodyElement thigh_left2(thigh_l2, vao_thigh_left2, glm::vec3(0, 1, 0), m);
    chest2.addChilds(&thigh_left2);
    thigh_left2.loadingTexture("texture/bleu.png");

    Sphere knee_l2(32, 32);
    GLuint vao_knee_left2 = generate_vao(knee_l2);
    BodyElement knee_left2(knee_l2, vao_knee_left2, glm::vec3(0, 1, 0), m);

    thigh_left2.addChilds(&knee_left2);
    knee_left2.loadingTexture("texture/skin.png");

    Sphere knee_r2(32, 32);
    GLuint vao_knee_right2 = generate_vao(knee_r2);
    BodyElement knee_right2(knee_r2, vao_knee_right2, glm::vec3(0, 1, 0), m);

    thigh_right2.addChilds(&knee_right2);
    knee_right2.loadingTexture("texture/skin.png");

    Cylinder leg_r2(32);
    GLuint vao_leg_right2 = generate_vao(leg_r2);
    BodyElement leg_right2(leg_r2, vao_leg_right2, glm::vec3(0, 1, 0), m);

    knee_right2.addChilds(&leg_right2);
    leg_right2.loadingTexture("texture/skin.png");

    Cylinder leg_l2(32);
    GLuint vao_leg_left2 = generate_vao(leg_l2);
    BodyElement leg_left2(leg_l2, vao_leg_left2, glm::vec3(0, 1, 0), m);

    knee_left2.addChilds(&leg_left2);
    leg_left2.loadingTexture("texture/skin.png");


    Cube foot_l2;
    GLuint vao_foot_left2 = generate_vao(foot_l2);
    BodyElement foot_left2(foot_l2, vao_foot_left2, glm::vec3(0, 1, 0), m);
    leg_left2.addChilds(&foot_left2);
    foot_left2.loadingTexture("texture/bleu.png");

    Cube foot_;
    GLuint vao_foot_right2 = generate_vao(foot_);
    BodyElement foot_right2(foot_, vao_foot_right2, glm::vec3(0, 1, 0), m);
    leg_right2.addChilds(&foot_right2);
    foot_right2.loadingTexture("texture/bleu.png");

    hand_r = Sphere(32, 32);
    GLuint vao_hand_right2 = generate_vao(hand_r);
    BodyElement hand_right2(hand_r, vao_hand_right2, glm::vec3(0, 1, 0), m);
    forearm_right2.addChilds(&hand_right2);
    hand_right2.loadingTexture("texture/skin.png");

    hand_l = Sphere(32, 32);
    GLuint vao_hand_left2 = generate_vao(hand_l);
    BodyElement hand_left2(hand_l, vao_hand_left2, glm::vec3(0, 1, 0), m);
    forearm_left2.addChilds(&hand_left2);
    hand_left2.loadingTexture("texture/skin.png");

    chest2.scale(glm::vec3(0.5, 0.7, 0.3));
    arm_right2.scale(glm::vec3(0.18, 0.3, 0.2));
    head2.scale(glm::vec3(0.5, 0.5, 0.5));
    arm_left2.scale(glm::vec3(0.18, 0.3, 0.2));
    thigh_right2.scale(glm::vec3(0.2, 0.2, 0.5));
    thigh_left2.scale(glm::vec3(0.2, 0.2, 0.5));
    neck2.scale(glm::vec3(0.2, 0.2, 0.15));
    elbow_left2.scale(glm::vec3(0.2015, 0.2015, 0.2015));
    elbow_right2.scale(glm::vec3(0.2015, 0.2015, 0.2015));
    forearm_left2.scale(glm::vec3(0.15, 0.17, 0.32));
    forearm_right2.scale(glm::vec3(0.15, 0.17, 0.32));
    knee_left2.scale(glm::vec3(0.2, 0.2, 0.2));
    knee_right2.scale(glm::vec3(0.2, 0.2, 0.2));
    leg_right2.scale(glm::vec3(0.20, 0.15, 0.4));
    leg_left2.scale(glm::vec3(0.20, 0.15, 0.4));
    foot_left2.scale(glm::vec3(0.18, 0.2, 0.15));
    foot_right2.scale(glm::vec3(0.18, 0.2, 0.15));
    hand_right2.scale(glm::vec3(0.14, 0.14, 0.14));
    hand_left2.scale(glm::vec3(0.14, 0.14, 0.14));
    chest2.translate(glm::vec3(-0.5, 0, 2));
    head2.translate(glm::vec3(0, 0.625, 0));
    arm_left2.translate(glm::vec3(0.35, 0.1, 0));   
    arm_right2.translate(glm::vec3(-0.35, 0.1, 0));
    thigh_right2.translate(glm::vec3(-0.12, -0.6, 0));
    thigh_right2.rotate(1.7, glm::vec3(1, 0, 0));
    thigh_left2.translate(glm::vec3(0.12, -0.6, 0));
    thigh_left2.rotate(1.7, glm::vec3(1, 0, 0));
    neck2.translate(glm::vec3(0, 0.4, 0));
    neck2.rotate(1.5, glm::vec3(1, 0, 0));
    elbow_left2.translate(glm::vec3(0, -0.13, 0));
    elbow_right2.translate(glm::vec3(0, -0.13, 0));
    forearm_left2.translate(glm::vec3(0, -0.2, -0.1));
    forearm_left2.rotate(1.9, glm::vec3(1, 0, 0));
    forearm_right2.translate(glm::vec3(0, -0.2, -0.1));
    forearm_right2.rotate(1.9, glm::vec3(1, 0, 0));
    knee_left2.translate(glm::vec3(0, 0, 0.3));
    knee_right2.translate(glm::vec3(0, 0, 0.3));
    leg_right2.translate(glm::vec3(0, 0.01, 0.18));
    leg_left2.translate(glm::vec3(0, 0.01, 0.18));
    foot_left2.translate(glm::vec3(0,-0.07, 0.3));
    foot_right2.translate(glm::vec3(0,-0.07, 0.3));
    hand_right2.translate(glm::vec3(0, 0, -0.15));
    hand_left2.translate(glm::vec3(0, 0, -0.15));

    
    //initialisation des objets représentant des particules
    material particle_m;
    particle_m.alpha = 32;
    particle_m.ka = 0.2f;
    particle_m.kd = 0.5f;
    particle_m.ks = 0.4f;
    particle_m.color = glm::vec3(0.5, 0.5, 0.5);
    std::vector<Particle> particles;
    
    for (int i = 0; i < 10; i++) {
        Cube c;
        Particle particle(c, generate_particle_vao(c), glm::vec3(0, 0, 0), particle_m);
        particle.scale(glm::vec3(0.1,0.1,0.1));
        particles.push_back(particle);
    }
    //collision
    CollisionPerso collisionPerso;
    collisionPerso.setParticles(particles);
    Personnage perso(chest, &collisionPerso);
    Personnage perso2(chest2,NULL);
    perso2.move(glm::vec3(1,0,0));

    light l;
    l.color = glm::vec3(1.f, 1.f, 1.f);
    l.position = glm::vec3(0, 3, 2);


    bool isOpened = true;

    glm::mat4 model(1.f);
    glm::mat4 model_light(1.f);
    model_light = glm::translate(model_light, l.position);
    model_light = glm::scale(model_light, glm::vec3(0.2f));
    float n = 0.01f;
    float const speed = 0.03;
    bool mouse_user = true;
    float x_direct = -90.f;
    float y_direct = 0;
    std::stack<glm::mat4> stack;
    int count;

    float n_animation = 0;
    float time = 0.02;
    //temps walk
    float tw = 0;
    float nb_walk = 15;
    float x = 1;

    float t = 0;
    int t_enGarde = 30;
    int t_boxe = 60;
    int nb_coups = 8;
    int nb_coups_elapsed = 0;
    float duree_coup = 10;
    bool coup = false;
    bool walk = false;
    bool baisse_garde = false;
    Camera camera(HEIGHT, WIDTH, glm::vec3(0.5, 1.5, -2.15), glm::vec3(0.5, 1.5, 0), glm::vec3(0, -0.5, 1));

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
                glm::vec3 mouvement = camera.eventKeyboard(event.key.keysym.scancode);

                if (perso.collision(perso2,mouvement)) {
                    mouvement = glm::vec3(0);
                }
                perso.move(mouvement);
                camera.move(mouvement);  
                if (!walk) {
                    walk = true;
                    tw = 0;
                    nb_walk = 15;
                    x = 1;
                }         
                break;
            case SDL_MOUSEMOTION:
                camera.eventMouse(event);
                SDL_WarpMouseInWindow(window, 400, 300);
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (!coup) {
                    t = 0;
                    /*t_enGarde = 30;
                    t_boxe = 60;
                    nb_coups = 8;*/
                    t_enGarde = 15;
                    t_boxe = 30;
                    nb_coups = 4;
                    nb_coups_elapsed = 0;
                    n_animation = 0;
                    coup = true;
                    baisse_garde = false;
                }
                break;
            }
        }

        //Clear the screen : the depth buffer and the color buffer
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        collisionPerso.computeAnimation();

        // ANIMATION
        if (coup) {


            perso.enGarde(t, t_boxe - t_enGarde, t_enGarde);

            nb_coups_elapsed = perso.boxe(t, duree_coup, t_boxe, nb_coups, nb_coups_elapsed);

            if (n_animation > 15) {
                n_animation = -15;

            }

            if (nb_coups_elapsed == nb_coups) {

                if (perso.baisseEnGarde(t, t_boxe - t_enGarde, t_boxe + nb_coups_elapsed * 4 * duree_coup + 4 * duree_coup)) {
                    baisse_garde = true;
                    
                }
                else if (baisse_garde) {
                    coup = false;
                }
            }

            t += 1;
            n_animation += 0.4;
            
        }else if (walk) {            
            perso.walk(time);
            if (tw > nb_walk) {
                time *= -1;
                x = -1;
            }
            tw += x;
            if (tw < 0) {
                walk = false;
            }
            
        }

        glm::mat4 cameraMatrix = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();
      
        stack.push(projection * cameraMatrix);
        //collisionPerso.computeParticle(particleShader, stack, camera, l);
        skybox.draw(skyshader, stack, camera, projection);
        perso.draw(shader, stack, camera, l);
        perso2.draw(shader, stack, camera, l);

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
