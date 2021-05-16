#include "Particle.h"
#include <Camera.h>
#include <queue>

Particle::Particle() {
}

Particle::Particle(Geometry geometry, GLuint vao, glm::vec3 position, struct material material)
    : GraphicObject(geometry, vao, position, material)
{

}

void Particle::draw(Shader* shader, std::stack<glm::mat4> stack, glm::mat4 parentModel, Camera camera, struct light light) {
    stack.push(stack.top() * model);

    glm::mat4 mat = stack.top();
    mat = mat * localTransformation;

    glm::mat4 matrixModelView = parentModel * model;
    //glm::mat4 mat = stack.top() * model * localTransformation;
    glm::mat3 inv_model = glm::inverse(glm::mat3(matrixModelView));

    glUseProgram(shader->getProgramID());
    {
        glBindVertexArray(vao);

        //uMVP
        GLint uMVP = glGetUniformLocation(shader->getProgramID(), "uMVP");
        glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(mat));

        //Les matrices
        GLint uModelViewMatrix = glGetUniformLocation(shader->getProgramID(), "uModelViewMatrix");
        glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(matrixModelView));
        GLint inv_uModelViewMatrix = glGetUniformLocation(shader->getProgramID(), "inv_uModelViewMatrix");
        glUniformMatrix3fv(inv_uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(inv_model));

        //Lights
        GLint ka = glGetUniformLocation(shader->getProgramID(), "ka");
        glUniform1f(ka, material.ka);
        GLint kd = glGetUniformLocation(shader->getProgramID(), "kd");
        glUniform1f(kd, material.kd);
        GLint ks = glGetUniformLocation(shader->getProgramID(), "ks");
        glUniform1f(ks, material.ks);
        GLint alpha = glGetUniformLocation(shader->getProgramID(), "alpha");
        glUniform1f(alpha, material.alpha);
        GLint ligth_color = glGetUniformLocation(shader->getProgramID(), "light_color");
        glUniform3fv(ligth_color, 1, glm::value_ptr(light.color));
        GLint ligth_pos = glGetUniformLocation(shader->getProgramID(), "light_pos");
        glUniform3fv(ligth_pos, 1, glm::value_ptr(light.position));
        GLint mat_color = glGetUniformLocation(shader->getProgramID(), "mat_color");
        glUniform3fv(mat_color, 1, glm::value_ptr(material.color));
        GLint camera_pos = glGetUniformLocation(shader->getProgramID(), "camera_pos");
        glUniform3fv(camera_pos, 1, glm::value_ptr(camera.getPosition()));
        glDrawArrays(GL_TRIANGLES, 0, getNbVertices());
        stack.pop();
    }
    glUseProgram(0);
}