#include "demi_sphere.h"
#include "sphere.h"
#include "textured_sphere.h"
#include <GL/glew.h>
#include <cmath>

Demi_Sphere::Demi_Sphere(Shader* shader_program, int sector_count, int stack_count) :
    Shape(shader_program) 
{
    //pour texture
    loc_diffuse_map = glGetUniformLocation(this->shader_program_, "diffuse_map");



    this->sector_count = sector_count;
    this->stack_count = stack_count;

    buildVertices();
    buildIndices();
   

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(3, &buffers[0]);

    // Position attribute
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Normal attribute
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    num_indices = static_cast<unsigned int>(indices.size());
}

void Demi_Sphere::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);
    



    glBindVertexArray(VAO);
    Shape::draw(model, view, projection);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);


}

void Demi_Sphere::buildVertices()
{
    vertices.clear();
    normals.clear();

    float radius = 0.5f;
    float x = 0.0f, y = 0.0f, z = 0.0f;

    float sector_step = 2.0f * M_PI / sector_count;
    float stack_step = (M_PI / 2.0f) / stack_count;

    for (int i = 0; i <= stack_count; i++) {
        float stack_angle = M_PI / 2.0f - i * stack_step; // from pi/2 to 0
        float xy = radius * std::cos(stack_angle);
        z = radius * std::sin(stack_angle);

        for (int j = 0; j <= sector_count; j++) {
            float sector_angle = j * sector_step;
            x = xy * std::cos(sector_angle);
            y = xy * std::sin(sector_angle);
            glm::vec3 vertex(x, y, z);
            glm::vec3 normal = glm::normalize(vertex);
            vertices.push_back(vertex);
            normals.push_back(normal);
        }
    }
}

void Demi_Sphere::buildIndices()
{
    indices.clear();
    int k1, k2;
    for (int i = 0; i < stack_count; i++) {
        k1 = i * (sector_count + 1);
        k2 = k1 + sector_count + 1;

        for (int j = 0; j < sector_count; j++, k1++, k2++) {
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }
}

void Demi_Sphere::key_handler(int key) {
    return;
}


