#include "TextureStore.h"
#include "stb_image.h"

std::vector<Texture> TextureStore::textures;
std::vector<NormalMap> TextureStore::normals;

GLuint TextureStore::RetrieveTexture(std::string location)
{
    if(location == "") return GLuint();

    for (auto& texture : textures)
    {
        if (!texture.active) continue;
        if (texture.name == location)
        {
            return texture.texture;
        }
    }

    
    Texture textureInstance;
    if (textureInstance.InitialiseTexture(location))
    {
        
        for (int i = 0; i < textures.size(); i++)
        {
            if (textures.at(i).active)continue;
            textures.at(i) = textureInstance;

            return textures.at(i).texture;
        }

        
        textures.emplace_back(textureInstance);

        return textures.at(textures.size() - 1).texture;
    }

    return GLuint();
}

GLuint TextureStore::RetrieveNormals(std::string location)
{
    if (location == "") return GLuint();

    for (auto& normal : normals)
    {
        if (!normal.active) continue;
        if (normal.name == location)
        {
            return normal.normal;
        }
    }

    
    NormalMap normalInstance;
    if (normalInstance.InitialiseNormal(location))
    {
        
        for (int i = 0; i < normals.size(); i++)
        {
            if (normals.at(i).active)continue;
            normals.at(i) = normalInstance;

            return normals.at(i).normal;
        }

        
        normals.emplace_back(normalInstance);

        return normals.at(normals.size() - 1).normal;
    }

    return GLuint();
}

bool Texture::InitialiseTexture(std::string location)
{
    name = location;
    int width, height, channels; unsigned char* data = stbi_load(location.c_str(), &width, &height, &channels, 4);

    if (!data)
    {
        std::cout << "DATA FOR IMAGE COULD NOT BE LOADED\n";
        active = false;
        return false;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    active = true;

    return true;
}

bool NormalMap::InitialiseNormal(std::string location)
{
    name = location;
    glGenTextures(1, &normal);
    glBindTexture(GL_TEXTURE_2D, normal);

    int w, h, c;
    unsigned char* hData = stbi_load(location.c_str(), &w, &h, &c, 1);

    if (!hData)
    {
        active = false;
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, hData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(hData);

    active = true;
    return true;
}
