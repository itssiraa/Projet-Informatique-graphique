#include "pyramid.h"
#include"shape.h"
#include <iostream>
#include "TextureUnits.h"

Pyramid::Pyramid(Shader* shader_program) : Shape(shader_program), texture(nullptr)
{


    // Vos indices initiaux
    GLuint indices[] = {
        //face avant
        0, 1, 2,
        2, 0, 3,

        //face arri�re 
        4, 5, 6,
        6, 7, 4,

        // c�t� gauche
        0, 1, 5,
        5, 0, 4,

        // fond
        1, 5, 2,
        2, 5, 6,

        // c�t� droit
        2, 6, 3,
        3, 6, 7,

        // dessus
        0, 3, 4,
        4, 3, 7
    };

    num_indices = sizeof(indices) / sizeof(GLuint);


    // Structure combin�e
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoord;
    };

    std::vector<Vertex> vertices;

    // Face avant
    vertices.push_back({ {-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f} });  // Coin sup�rieur gauche
    vertices.push_back({ {0.0f, 0.0f, -0.5f}, {0.5f, 0.0f} });    // Centre bas
    vertices.push_back({ {1.0f, 0.0f, -0.5f}, {1.0f, 0.0f} });    // Coin droit bas
    vertices.push_back({ {1.5f, 0.5f, -0.5f}, {1.0f, 1.0f} });    // Coin sup�rieur droit

    // Face arri�re (m�mes UV mais en d�cal� si n�cessaire)
    vertices.push_back({ {-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f} });   // Haut gauche arri�re
    vertices.push_back({ {0.0f, 0.0f, 0.0f}, {0.25f, 0.5f} });   // Centre bas arri�re
    vertices.push_back({ {1.0f, 0.0f, 0.0f}, {0.75f, 0.5f} });    // Centre droit arri�re
    vertices.push_back({ {1.5f, 0.5f, 0.0f}, {1.0f, 0.0f} });    // Haut droit arri�re

    // Avant la boucle de rendu, recalculez les UV en fonction de la position 3D :
    for (auto& vertex : vertices) {
        // �tirement horizontal bas� sur la position X
        vertex.texCoord.x = (vertex.position.x + 0.5f) / 2.0f;  // Normalis� entre [0, 1]
        // �tirement vertical bas� sur la position Z
        vertex.texCoord.y = vertex.position.z + 0.5f;           // Normalis� entre [0, 1]
    }



    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Buffer unique
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Positions (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Texture (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


}


void Pyramid::loadTexture(const std::string& path) {

    if (texture) {
        delete texture; // Nettoyage si texture d�j� existante
    }
    try {
        texture = new Texture(path);
        glBindTexture(GL_TEXTURE_2D, texture->getGLid());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load texture: " << e.what() << std::endl;
        texture = nullptr;
    }
}
void Pyramid::addTexture(const std::string& texturePath) {
    try {
        textures.push_back(new Texture(texturePath));
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Texture loading error: " << e.what() << std::endl;
    }
}

void Pyramid::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(shader_program_);

    if (texture) {
        glActiveTexture(GL_TEXTURE0 + TEX_UNIT_OBJECT_DIFF);
        glBindTexture(GL_TEXTURE_2D, texture->getGLid());
        glUniform1i(glGetUniformLocation(shader_program_, "diffuse_map"), 0);
        GLint texLoc = glGetUniformLocation(shader_program_, "diffuse_map");
        if (texLoc != -1) {
            glUniform1i(texLoc, 0);
        }
        else {
            std::cerr << "Warning: diffuse_map uniform not found!" << std::endl;
        }
    }



    Shape::draw(model, view, projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
Pyramid::~Pyramid() {
    for (auto* tex : textures) {
        delete tex;
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, buffers);
}
