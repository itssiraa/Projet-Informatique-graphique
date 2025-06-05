#include "triangle.h"
#include <glm/glm.hpp> // nécessaire pour glm::vec3
#include <glm/gtc/type_ptr.hpp>
#include<iostream>


Triangle::Triangle(Shader* shader_program) : Shape(shader_program), color(glm::vec3(1.0f, 0.0f, 0.0f)) {



    GLfloat vertex_buffer_data[] = {
        -1.0f, -0.5f, 0.0f, // bas gauche
        -0.5f,  0.5f, 0.0f, // haut gauche
         0.5f,  0.5f, 0.0f, // haut droite
         1.0f, -0.5f, 0.0f  // bas droite
    };


    GLint indices[] = {
        0, 1, 2,
        2, 0, 3,
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // on laisse VAO bindé ici
}

Triangle::~Triangle() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    
}

void Triangle::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);
    glBindVertexArray(VAO);

    // Combine les matrices
    glm::mat4 mvp = projection * view * model;

    // Envoie la matrice MVP
    GLint mvpLoc = glGetUniformLocation(this->shader_program_, "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    // Envoie la couleur (blanc par défaut)
    this->color = glm::vec3(1.0f); // Force la couleur blanche
    GLint colorLoc = glGetUniformLocation(this->shader_program_, "fragColorUniform");
    if (colorLoc != -1) {
        glUniform3fv(colorLoc, 1, glm::value_ptr(this->color));
    }
    else {
        std::cerr << "Erreur: fragColorUniform introuvable!" << std::endl;
    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


//imposer une couleur aux voiles
void Triangle::setColor(glm::vec3 c) {
    color = c; 
}



void Triangle::key_handler(int key) {
    return;
}
