#ifndef A3_IMAGETEXTURE_H
#define A3_IMAGETEXTURE_H

#include <core/log/a3Log.h>
#include <textures/a3Texture.h>
#include <core/a3Spectrum.h>
#include <core/image/a3ImageDecoder.h>

template<class T>
class a3ImageTexture:public a3Texture<T>
{
public:
    a3ImageTexture(const char* filePath)
    {
        if(decoder.load(filePath))
            a3Log::success("Image Texture Created Succeed\n");
    }

    virtual T evaluate(float u, float v) const
    {
        return decoder.lookup(u, v) / 255.0f;
    }

    a3ImageDecoder decoder;
};

a3ImageTexture<a3Spectrum>* a3CreateImageTexture(const char* filePath)
{
    a3ImageTexture<a3Spectrum>* texture = new a3ImageTexture<a3Spectrum>(filePath);

    return texture;
}

#endif