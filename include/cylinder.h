#pragma once

#include "shape.h"
#include "texture.h"
#include <vector>



#include "shader.h"

#include <glm/glm.hpp>



#include "sphere.h"


class Cylinder : public Shape {
public:
    // Constructeur modifié pour inclure le chemin de la texture
    Cylinder(Shader* shader_program, float height, float radius, int slices,
        const std::string& texturePath = "");
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    GLuint getGLid() const { return glid_; }


private:
    unsigned int num_indices;
    GLuint VAO, VBO, EBO;
    GLuint buffers[2];

    GLuint glid_;
    GLuint textureID;  // Ajout pour la texture
    Texture* texture;  // Pointeur vers la texture

};
