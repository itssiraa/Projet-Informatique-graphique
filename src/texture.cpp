#include "texture.h"
#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture(const std::string& tex_file, GLenum wrap_mode, GLenum min_filter, GLenum mag_filter) {
    // Vérification du fichier


    // Chargement de l'image
    stbi_set_flip_vertically_on_load(true); // Important pour OpenGL
    int width, height, num_channels;
    unsigned char* data = stbi_load(tex_file.c_str(), &width, &height, &num_channels, 0);

    if (!data) {
        std::string error = "STB_IMAGE ERROR: ";
        error += stbi_failure_reason();
        throw std::runtime_error(error);
    }

    // Création de la texture OpenGL
    glGenTextures(1, &glid_);
    glBindTexture(GL_TEXTURE_2D, glid_);

    GLenum format = (num_channels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Paramètres de texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

    std::cout << "Chargement texture bateau depuis : " << tex_file << std::endl;
    std::cout << "→ OpenGL ID = " << glid_ << std::endl;


    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &glid_);
}

void Texture::bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, glid_);
}