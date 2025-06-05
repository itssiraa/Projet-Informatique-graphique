#pragma once

#include "shape.h"
#include "shader.h"
#include <GL/glew.h>

class Triangle : public Shape {
public:
    Triangle(Shader *shader_program);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void key_handler(int key);
    virtual ~Triangle();
    //rajout de couleur sur les voiles
    void setColor(glm::vec3 c);





   


private:
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;

    
    //initialisation de la couleur qui sera attribuée
    glm::vec3& color;
    
};