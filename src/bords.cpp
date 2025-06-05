#include "pyramid.h"
#include "bordsBateau.h"
#include"shape.h"
#include <iostream>


Bords::Bords(Shader* shader_program) : Shape(shader_program), texture(nullptr)
{


    // Vos indices initiaux
    GLuint indices[] = {
        //face avant
        0, 1, 3,
        1, 3, 2,

        //face arrière 
        4, 5, 7,
        7, 5, 6,

        // côté gauche
        0, 1, 5,
        5, 0, 4,

        // fond
        1, 5, 2,
        2, 5, 6,

        // côté droit
        2, 6, 3,
        3, 6, 7,

        // dessus
        0, 3, 4,
        4, 3, 7
    };

    num_indices = sizeof(indices) / sizeof(GLuint);


    // Structure combinée
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoord;
    };

    std::vector<Vertex> vertices;

    // Face avant
    vertices.push_back({ {-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f} });// Coin supérieur gauche
    vertices.push_back({ {0.0f, 0.0f, -0.5f}, {0.5f, 0.0f} });// Centre bas
    vertices.push_back({ {0.2f, 0.0f, -0.5f}, {1.0f, 0.0f} });// Coin droit bas
    vertices.push_back({ {0.2f, 0.25f, -0.5f}, {1.0f, 1.0f} });// Coin supérieur droit

    // Face arrière (mêmes UV mais en décalé si nécessaire)
    vertices.push_back({ {-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f} });// Haut gauche arrière
    vertices.push_back({ {0.0f, 0.0f, 0.0f}, {0.25f, 0.5f} });// Centre bas arrière
    vertices.push_back({ {0.2f, 0.0f, 0.0f}, {0.75f, 0.5f} });// Centre droit arrière
    vertices.push_back({ {0.2f, 0.25f, 0.0f}, {1.0f, 0.0f} });// Haut droit arrière

    // Avant la boucle de rendu, recalculez les UV en fonction de la position 3D :
    for (auto& vertex : vertices) {
        // Étirement horizontal basé sur la position X
        vertex.texCoord.x = (vertex.position.x + 0.5f) / 2.0f;// Normalisé entre [0, 1]
        // Étirement vertical basé sur la position Z
        vertex.texCoord.y = vertex.position.z + 0.5f;// Normalisé entre [0, 1]
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


void Bords::loadTexture(const std::string& path) {

    if (texture) {
        delete texture; // Nettoyage si texture déjà existante
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
void Bords::addTexture(const std::string& texturePath) {
    try {
        textures.push_back(new Texture(texturePath));
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Texture loading error: " << e.what() << std::endl;
    }
}

void Bords::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(shader_program_);

    if (texture) {
        glActiveTexture(GL_TEXTURE3);
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
Bords::~Bords() {
    for (auto* tex : textures) {
        delete tex;
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, &VAO);
    glDeleteBuffers(2, &EBO);
}
