//
// Created by lukas on 06.01.22.
//

#ifndef OPENCRAFT_TEXTUREARRAY_H
#define OPENCRAFT_TEXTUREARRAY_H

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>
#include <iostream>

class TextureArray {
public:
    TextureArray() {
        /*glGenTextures( 1, &array );
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, array);

        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, 16, 16, 100);*/

        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &array);

        glTextureParameteri(array, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTextureParameteri(array, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTextureParameteri(array, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(array, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureStorage3D(array, 1, GL_RGB8, 16, 16, 100);
    }

    void AddTexture(const std::string& path) {
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            /*glTexSubImage3D( GL_TEXTURE_2D_ARRAY,
                             0,                     //Mipmap number
                             0,0,0,                 //xoffset, yoffset, zoffset
                             16,16,1,                 //width, height, depth
                             GL_RGB,                //format
                             GL_UNSIGNED_BYTE,      //type
                             data);*/
            glTextureSubImage3D(array, 0, 0, 0, index, 16, 16, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
            index++;
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    void Bind() {
        glBindTexture(GL_TEXTURE_2D_ARRAY, array);
    }

    uint32_t GetID() { return array; }

private:
    uint32_t array;
    uint32_t index = 0;
};


#endif //OPENCRAFT_TEXTUREARRAY_H
