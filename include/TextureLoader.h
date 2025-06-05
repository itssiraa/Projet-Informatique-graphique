#pragma once

#include <iostream>
#include <stdexcept>
#include <GL/glew.h>


class TextureLoader
{
    static GLenum getFormat(const unsigned& nrChannels) {
        if (nrChannels == 1)
            return GL_RED;
        else if (nrChannels == 3)
            return GL_RGB;
        else if (nrChannels == 4)
            return GL_RGBA;
        else
            throw std::runtime_error("Format image inconnu (nombre de canaux)");
    }

public:
	static void loadTexture(unsigned& textureId, const std::string& fileName);
};


