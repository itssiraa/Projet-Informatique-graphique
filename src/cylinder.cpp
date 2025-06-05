#include "cylinder.h"
#include "shape.h"
#include "texture.h"

#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

Cylinder::Cylinder(Shader* shader_program, float height, float radius, int slices, const std::string& texturePath)
    : Shape(shader_program), texture(nullptr) {


    // Chargement texture

    if (!texturePath.empty()) {
        try {
            texture = new Texture(texturePath);
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur chargement texture: " << e.what() << std::endl;
        }
    }

    // Génération géométrie

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;

    // Vertices + coordonnées texture
    for (int i = 0; i <= slices; i++) {
        float theta = 2.0f * glm::pi<float>() * i / slices;
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        // Vertices haut/bas
        vertices.push_back(glm::vec3(x, y, height / 2));
        vertices.push_back(glm::vec3(x, y, -height / 2));

        // Coordonnées texture (mapping cylindrique)
        float u = static_cast<float>(i) / slices;
        texCoords.push_back(glm::vec2(u, 1.0f)); // Haut
        texCoords.push_back(glm::vec2(u, 0.0f)); // Bas
    }

    // Indices
    for (unsigned int i = 0; i < slices; i++) {
        indices.push_back(2 * i);
        indices.push_back(2 * i + 1);
        indices.push_back(2 * i + 2);

        indices.push_back(2 * i + 1);
        indices.push_back(2 * i + 3);
        indices.push_back(2 * i + 2);
    }


    // Configuration OpenGL

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Buffer combiné (vertices + texCoords)
    std::vector<float> combined;
    for (size_t i = 0; i < vertices.size(); i++) {
        combined.push_back(vertices[i].x);
        combined.push_back(vertices[i].y);
        combined.push_back(vertices[i].z);
        combined.push_back(texCoords[i].x);
        combined.push_back(texCoords[i].y);
    }

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, combined.size() * sizeof(float), combined.data(), GL_STATIC_DRAW);

    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Attributs
    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    // Texture
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    num_indices = indices.size();
}

void Cylinder::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);

    // Activation texture
    if (texture) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture->getGLid());
        glUniform1i(glGetUniformLocation(this->shader_program_, "diffuse_map"), 0);
    }

    Shape::draw(model, view, projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);


    // draw code

}